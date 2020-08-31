from abc import ABC, abstractmethod
from os import stat, makedirs, listdir, devnull
from os.path import dirname, abspath, realpath, join as path_join, exists, basename
from typing import List, Tuple
from shutil import rmtree, copyfile
from multiprocessing.pool import ThreadPool
from hashlib import md5
from cairosvg import svg2pdf

from common import print, check_call

pool = ThreadPool()


class DocumentBuilderException(Exception):
    pass


class DocumentBuilder(ABC):
    script_dir = dirname(abspath(realpath(__file__)))

    def __init__(self, root: str, project: str, document: str):
        self.project = project
        self.document = document
        self.document_dir = path_join(root, project, document)
        self.document_path = path_join(self.document_dir, document) + '.pdf'
        self.sheet_count = 0

    @abstractmethod
    def _prerequisites(self) -> List[str]:
        raise NotImplementedError

    @abstractmethod
    def _calculate_sheet_count(self) -> None:
        raise NotImplementedError

    @abstractmethod
    def _managed_tmp_dirs(self) -> List[str]:
        raise NotImplementedError

    @abstractmethod
    def _actual_build(self) -> None:
        raise NotImplementedError

    def __is_document_up_to_date(self) -> bool:
        prerequisites = self._prerequisites()
        for file_path in prerequisites:
            if not exists(file_path):
                raise DocumentBuilderException(f'File "{basename(file_path)}" is missing')
        if not exists(self.document_path):
            return False
        target_mtime = stat(self.document_path).st_mtime_ns
        return all(stat(dependency).st_mtime_ns < target_mtime for dependency in prerequisites)

    def build(self) -> None:
        if self.__is_document_up_to_date():
            print(f'Document "{self.document}" of project "{self.project}" is up to date. Nothing to do.')
            return
        for tmp_dir in (self._tmp_data_dir(), self._tmp_form_dir(), self._tmp_overlay_dir()):
            if exists(tmp_dir):
                rmtree(tmp_dir)
            makedirs(tmp_dir)
        print('Generating sheets ...')
        self._actual_build()
        self._calculate_sheet_count()
        if self.sheet_count == 0:
            raise DocumentBuilderException('No data has been generated')
        print(f'{self.sheet_count} sheet(s) has/have been generated.')
        self._build_form()
        self._convert_svg_to_pdf(*self._managed_tmp_dirs())
        self._overlay_form()
        self._combine_sheets()

    def _build_form(self) -> None:
        print('Generating form sheets ...')
        check_call([self._tool_path('formgen'), '-i', path_join('..', 'form.kxg'), '-p', str(self.sheet_count)], cwd=self._tmp_form_dir())

    def _convert_svg_to_pdf(self, *tmp_dirs: str) -> None:
        def convert(svg_file_path) -> None:
            pdf_file_path = svg_file_path[:-4] + '.pdf'
            if exists(path_join(self.document_dir, pdf_file_path)):
                source_mtime = stat(path_join(self.document_dir, svg_file_path)).st_mtime_ns
                target_mtime = stat(path_join(self.document_dir, pdf_file_path)).st_mtime_ns
                if source_mtime < target_mtime:
                    print(f'File "{pdf_file_path}" is up to date.')
                    return           
            svg2pdf(url=path_join(self.document_dir, svg_file_path), write_to=path_join(self.document_dir, pdf_file_path))

        print('Converting SVG files to corresponding PDF ones ...')
        svg_files = []
        for tmp_dir in tmp_dirs:
            svg_files.extend([path_join(basename(tmp_dir), file_name) for file_name in listdir(tmp_dir) if file_name.endswith('.svg')])
        pool.map(convert, svg_files, 1)

    def _overlay_form(self) -> None:
        def overlay(sheet_num: int) -> None:
            data_page_path = path_join(basename(self._tmp_data_dir()), f'Sheet_{sheet_num:02}.pdf')
            form_page_path = path_join(basename(self._tmp_form_dir()), f'Sheet_{sheet_num:02}.pdf')
            overlay_page_path = path_join(basename(self._tmp_overlay_dir()), f'Sheet_{sheet_num:02}.pdf')
            check_call(['pdftk', form_page_path, 'stamp', data_page_path, 'output', overlay_page_path], cwd=self.document_dir)

        print('Overlaying form sheets over data sheets ...')
        pool.map(overlay, range(1, self.sheet_count + 1), 1)

    def _combine_sheets(self) -> None:
        print('Combining individual sheets into a resulting document ...')
        sheets = [path_join(basename(self._tmp_overlay_dir()), f'Sheet_{sheet_num:02}.pdf') for sheet_num in range(1, self.sheet_count + 1)]
        check_call(['pdftk'] + sheets + ['cat', 'output', basename(self.document_path)], cwd=self.document_dir)

    @staticmethod
    def _tool_path(tool_name: str) -> str:
        return path_join(DocumentBuilder.script_dir, 'tools', 'apps', tool_name, tool_name)

    def _tmp_data_dir(self) -> str:
        return abspath(path_join(self.document_dir, 'data'))

    def _tmp_form_dir(self) -> str:
        return abspath(path_join(self.document_dir, 'form'))

    def _tmp_overlay_dir(self) -> str:
        return abspath(path_join(self.document_dir, 'overlay'))


class TableBuilder(DocumentBuilder):
    def _prerequisites(self) -> List[str]:
        prerequisites = [path_join(self.document_dir, file_name) for file_name in ('data.kxg', 'form.kxg')]
        return prerequisites

    def _calculate_sheet_count(self) -> None:
        self.sheet_count = len([file_name for file_name in listdir(self._tmp_data_dir()) if file_name.endswith('.svg')])

    def _managed_tmp_dirs(self) -> List[str]:
        return [self._tmp_data_dir(), self._tmp_form_dir()]

    def _actual_build(self) -> None:
        check_call([self._tool_path(self._tool_name()), '-i', path_join('..', 'data.kxg')], cwd=self._tmp_data_dir())
        # TODO: Make possible generation of documents without list_of_changes.svg sheet.
        self._calculate_sheet_count()
        copyfile(path_join(DocumentBuilder.script_dir, 'tools', 'common', 'list_of_changes.svg'),
                 path_join(self._tmp_data_dir(), f'Sheet_{self.sheet_count + 1:02}.svg'))

    @abstractmethod
    def _tool_name(self) -> str:
        raise NotImplementedError


class SpecificationBuilder(TableBuilder):
    def _tool_name(self) -> str:
        return '2.106-form1'


class RegisterBuilder(TableBuilder):
    def _tool_name(self) -> str:
        return '2.106-form5'


class ListOfElementsBuilder(TableBuilder):
    def _tool_name(self) -> str:
        return 'listofelgen'


class TextDocumentBuilder(DocumentBuilder):
    def _prerequisites(self) -> List[str]:
        pics_dir = path_join(self.document_dir, 'pics')
        prerequisites = [path_join(self.document_dir, file_name) for file_name in ('doc.tex', 'form.kxg')]
        if exists(pics_dir):
            prerequisites.extend([path_join(pics_dir, file_name) for file_name in listdir(pics_dir) if file_name.endswith(('.png', '.svg'))])
        return prerequisites

    def _calculate_sheet_count(self) -> None:
        self.sheet_count = len([file_name for file_name in listdir(self._tmp_data_dir()) if file_name.startswith('Sheet_') and file_name.endswith('.pdf')])

    def _managed_tmp_dirs(self) -> List[str]:
        return [self._tmp_form_dir()]

    def _actual_build(self) -> None:
        # Convert SVG pictures to PDF files if exist.
        pics_dir = path_join(self.document_dir, 'pics')
        if exists(pics_dir):
            self._convert_svg_to_pdf(pics_dir)
        # Generate document itself.
        common_input_path = path_join(DocumentBuilder.script_dir, 'tools', 'common')
        cmd = ['xelatex', '-interaction=nonstopmode', '--output-driver=xdvipdfmx -V 5', '-synctex=1', f'-jobname=data', '-output-directory=data',
               fr'\def\pdf{{}}\input{{doc.tex}}']
        prev_digests = None, None
        while True:
            with open(devnull, 'w') as null:
                check_call(cmd, cwd=self.document_dir, env={'TEXINPUTS': f'.:{common_input_path}:'}, stdout=null)
            digests = self.__aux_digests()
            if digests == prev_digests:
                break
            prev_digests = digests
        # Burst generate document into separate pages.
        check_call(['pdftk', 'data.pdf', 'burst', 'output', 'Sheet_%02d.pdf'], cwd=self._tmp_data_dir())

    def __aux_digests(self) -> Tuple[str, str]:
        with open(path_join(self._tmp_data_dir(), 'data.aux'), 'rb') as aux_f, open(path_join(self._tmp_data_dir(), 'data.toc'), 'rb') as toc_f:
            return md5(aux_f.read()).hexdigest(), md5(toc_f.read()).hexdigest()
