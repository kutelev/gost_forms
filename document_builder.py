from abc import ABC, abstractmethod
from builtins import print as original_print
from os import walk, stat, makedirs, listdir
from os.path import dirname, abspath, realpath, join as path_join, exists, basename
from subprocess import check_call
from sys import stderr
from typing import List, Tuple
from shutil import rmtree, copyfile
from multiprocessing.pool import ThreadPool

pool = ThreadPool()


# noinspection PyShadowingBuiltins
def print(*args) -> None:
    original_print(*args, file=stderr, flush=True)


class DocumentBuilderException(Exception):
    pass


class DocumentBuilder(ABC):
    script_path = dirname(abspath(realpath(__file__)))

    def __init__(self, project: str, document: str):
        self.project = project
        self.document = document
        self.document_dir = path_join(project, document)
        self.document_path = path_join(self.document_dir, document) + '.pdf'
        self.sheet_count = 0

    def _tmp_data_dir(self) -> str:
        return path_join(self.document_dir, 'tmp_data')

    def _tmp_form_dir(self) -> str:
        return path_join(self.document_dir, 'tmp_form')

    def _tmp_overlay_dir(self) -> str:
        return path_join(self.document_dir, 'tmp_overlay')

    def build(self) -> None:
        if self.is_document_up_to_date():
            print(f'Document "{self.document}" of project "{self.project}" is up to date. Nothing to do.')
            return
        for tmp_dir in (self._tmp_data_dir(), self._tmp_form_dir(), self._tmp_overlay_dir()):
            if exists(tmp_dir):
                rmtree(tmp_dir)
            makedirs(tmp_dir)
        print('Generating data sheets ...')
        self._build_data_sheets()
        self._generate_list_of_changes()
        self._calculate_sheet_count()
        if self.sheet_count == 0:
            raise DocumentBuilderException('No data has been generated')
        print(f'{self.sheet_count} sheet(s) has/have been generated.')
        print('Generating form sheets ...')
        self._build_form()
        print('Converting generated SVG files to corresponding PDF ones ...')
        self._convert_svg_to_pdf()
        print('Overlaying form sheets over data sheets ...')
        self._overlay_form()
        print('Combining individual sheets into a resulting document ...')
        self._combine_sheets()

    @abstractmethod
    def _build_data_sheets(self) -> None:
        pass

    def default_prerequisites(self) -> List[str]:
        prerequisites = []
        for root, _, files in walk(self.document_dir):
            prerequisites.extend([abspath(path_join(root, file_name)) for file_name in files if file_name.endswith('.kxg')])
        return prerequisites

    # noinspection PyMethodMayBeStatic
    def specific_prerequisites(self) -> List[str]:
        return []

    def prerequisites(self) -> List[str]:
        return self.default_prerequisites() + self.specific_prerequisites()

    def is_document_up_to_date(self) -> bool:
        if not exists(self.document_path):
            return False
        target_mtime = stat(self.document_path).st_mtime_ns
        return all(stat(dependency).st_mtime_ns < target_mtime for dependency in self.prerequisites())

    @staticmethod
    def _tool_path(tool_name: str) -> str:
        return path_join(DocumentBuilder.script_path, 'tools', 'apps', tool_name, tool_name)

    def _default_data(self) -> Tuple[str, str]:
        """
        :return: Tuple containing two paths: data file (*.kxg), form fill data (form.kxg)
        """
        data = sorted(self.default_prerequisites(), key=lambda x: basename(x) == 'form.kxg')
        if len(data) != 2 or basename(data[1]) != 'form.kxg':
            raise DocumentBuilderException('Invalid input data')
        return data[0], data[1]

    def _generate_list_of_changes(self) -> None:
        # TODO: Make possible generation of documents without this sheet.
        self._calculate_sheet_count()
        copyfile(path_join(DocumentBuilder.script_path, 'tools', 'common', 'list_of_changes.svg'),
                 path_join(self._tmp_data_dir(), f'Sheet_{self.sheet_count + 1:02}.svg'))

    def _calculate_sheet_count(self) -> None:
        self.sheet_count = len([file_name for file_name in listdir(self._tmp_data_dir()) if file_name.endswith('.svg')])

    def _build_form(self) -> None:
        check_call([self._tool_path('formgen'), '-i', self._default_data()[1], '-p', str(self.sheet_count)], cwd=self._tmp_form_dir())

    def _convert_svg_to_pdf(self) -> None:
        def convert(svg_file_path) -> None:
            pdf_file_path = svg_file_path[:-4] + '.pdf'
            check_call(['dbus-run-session', 'inkscape', '--export-pdf', pdf_file_path, svg_file_path])

        svg_files = []
        for tmp_dir in (self._tmp_data_dir(), self._tmp_form_dir()):
            svg_files.extend([path_join(tmp_dir, f'Sheet_{sheet_num:02}.svg') for sheet_num in range(1, self.sheet_count + 1)])
        pool.map(convert, svg_files, 1)

    def _overlay_form(self) -> None:
        def overlay(sheet_num: int) -> None:
            data_page_path = path_join(self._tmp_data_dir(), f'Sheet_{sheet_num:02}.pdf')
            form_page_path = path_join(self._tmp_form_dir(), f'Sheet_{sheet_num:02}.pdf')
            overlay_page_path = path_join(self._tmp_overlay_dir(), f'Sheet_{sheet_num:02}.pdf')
            check_call(['pdftk', form_page_path, 'stamp', data_page_path, 'output', overlay_page_path])

        pool.map(overlay, range(1, self.sheet_count + 1), 1)

    def _combine_sheets(self) -> None:
        sheets = [path_join(self._tmp_overlay_dir(), f'Sheet_{sheet_num:02}.pdf') for sheet_num in range(1, self.sheet_count + 1)]
        check_call(['pdftk'] + sheets + ['cat', 'output', self.document_path])


class SpecificationBuilder(DocumentBuilder):
    def __init__(self, *args):
        super().__init__(*args)

    def _build_data_sheets(self) -> None:
        check_call([self._tool_path('2.106-form1'), '-i', self._default_data()[0]], cwd=self._tmp_data_dir())


class RegisterBuilder(DocumentBuilder):
    def __init__(self, *args):
        super().__init__(*args)

    def _build_data_sheets(self) -> None:
        check_call([self._tool_path('2.106-form5'), '-i', self._default_data()[0]], cwd=self._tmp_data_dir())
