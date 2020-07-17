from abc import ABC, abstractmethod
from os import stat, makedirs, listdir
from os.path import dirname, abspath, realpath, join as path_join, exists, basename
from subprocess import check_call
from typing import List
from shutil import rmtree, copyfile
from multiprocessing.pool import ThreadPool

from common import print, check_call

pool = ThreadPool()


class DocumentBuilderException(Exception):
    pass


class DocumentBuilder:
    script_path = dirname(abspath(realpath(__file__)))

    def __init__(self, root: str, project: str, document: str):
        self.project = project
        self.document = document
        self.document_dir = path_join(root, project, document)
        self.document_path = path_join(self.document_dir, document) + '.pdf'
        self.sheet_count = 0

    def _prerequisites(self) -> List[str]:
        return []

    def is_document_up_to_date(self) -> bool:
        if not exists(self.document_path):
            return False
        target_mtime = stat(self.document_path).st_mtime_ns
        return all(stat(dependency).st_mtime_ns < target_mtime for dependency in self._prerequisites())

    @abstractmethod
    def build(self) -> None:
        pass

    @staticmethod
    def _tool_path(tool_name: str) -> str:
        return path_join(TableBuilder.script_path, 'tools', 'apps', tool_name, tool_name)

    def _tmp_data_dir(self) -> str:
        return abspath(path_join(self.document_dir, 'data'))

    def _tmp_form_dir(self) -> str:
        return abspath(path_join(self.document_dir, 'form'))

    def _tmp_overlay_dir(self) -> str:
        return abspath(path_join(self.document_dir, 'overlay'))


class TableBuilder(DocumentBuilder):
    script_path = dirname(abspath(realpath(__file__)))

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
    def _tool_name(self) -> str:
        pass

    def _build_data_sheets(self) -> None:
        check_call([self._tool_path(self._tool_name()), '-i', path_join('..', 'data.kxg')], cwd=self._tmp_data_dir())

    def _prerequisites(self) -> List[str]:
        prerequisites = [path_join(self.document_dir, file_name) for file_name in ('data.kxg', 'form.kxg')]
        for file_path in prerequisites:
            if not exists(file_path):
                raise DocumentBuilderException(f'File "{basename(file_path)}" is missing')
        return prerequisites

    def _generate_list_of_changes(self) -> None:
        # TODO: Make possible generation of documents without this sheet.
        self._calculate_sheet_count()
        copyfile(path_join(TableBuilder.script_path, 'tools', 'common', 'list_of_changes.svg'),
                 path_join(self._tmp_data_dir(), f'Sheet_{self.sheet_count + 1:02}.svg'))

    def _calculate_sheet_count(self) -> None:
        self.sheet_count = len([file_name for file_name in listdir(self._tmp_data_dir()) if file_name.endswith('.svg')])

    def _build_form(self) -> None:
        check_call([self._tool_path('formgen'), '-i', path_join('..', 'form.kxg'), '-p', str(self.sheet_count)], cwd=self._tmp_form_dir())

    def _convert_svg_to_pdf(self) -> None:
        def convert(svg_file_path) -> None:
            pdf_file_path = svg_file_path[:-4] + '.pdf'
            # dbus-run-session is required due to the following issue:
            # https://gitlab.com/inkscape/inkscape/-/issues/294
            check_call(['dbus-run-session', 'inkscape', '--export-pdf', pdf_file_path, svg_file_path], cwd=self.document_dir)

        svg_files = []
        for tmp_dir in (self._tmp_data_dir(), self._tmp_form_dir()):
            svg_files.extend([path_join(basename(tmp_dir), f'Sheet_{sheet_num:02}.svg') for sheet_num in range(1, self.sheet_count + 1)])
        pool.map(convert, svg_files, 1)

    def _overlay_form(self) -> None:
        def overlay(sheet_num: int) -> None:
            data_page_path = path_join(basename(self._tmp_data_dir()), f'Sheet_{sheet_num:02}.pdf')
            form_page_path = path_join(basename(self._tmp_form_dir()), f'Sheet_{sheet_num:02}.pdf')
            overlay_page_path = path_join(basename(self._tmp_overlay_dir()), f'Sheet_{sheet_num:02}.pdf')
            check_call(['pdftk', form_page_path, 'stamp', data_page_path, 'output', overlay_page_path], cwd=self.document_dir)

        pool.map(overlay, range(1, self.sheet_count + 1), 1)

    def _combine_sheets(self) -> None:
        sheets = [path_join(basename(self._tmp_overlay_dir()), f'Sheet_{sheet_num:02}.pdf') for sheet_num in range(1, self.sheet_count + 1)]
        check_call(['pdftk'] + sheets + ['cat', 'output', basename(self.document_path)], cwd=self.document_dir)


class SpecificationBuilder(TableBuilder):
    def _tool_name(self) -> str:
        return '2.106-form1'


class RegisterBuilder(TableBuilder):
    def _tool_name(self) -> str:
        return '2.106-form5'


class ListOfElementsBuilder(TableBuilder):
    def _tool_name(self) -> str:
        return 'listofelgen'
