from argparse import ArgumentParser
from os import makedirs, listdir
from sys import stderr
from os.path import dirname, abspath, realpath, join as path_join, exists, isdir
from platform import system
from shutil import rmtree
from subprocess import check_call, CalledProcessError
from sys import exit
from typing import List
from re import compile
from builtins import print as original_print

from document_builder import SpecificationBuilder, DocumentBuilderException

project_pattern = compile(r'^[А-Я]{4}\.\d{6}\.\d{3} \(.*\)$')
document_pattern = compile(r'^[А-Я]{4}\.\d{6}\.\d{3} ((ВП|ПЭ3|РЭ) )?\(.*\)$')


# noinspection PyShadowingBuiltins
def print(*args) -> None:
    original_print(*args, file=stderr, flush=True)


def check_prerequisites():
    if system() != 'Linux':
        print('Error: nothing but Linux is supported.')
        exit(1)


class Builder:
    def __init__(self, root: str):
        self.root = root

    @staticmethod
    def build_tools(script_path: str, force: bool):
        if not force:
            tools = ['2.106-form1', '2.106-form5', 'formgen', 'listofelgen']
            if all(exists(path_join(script_path, 'tools', 'apps', tool, tool)) for tool in tools):
                # All tools are already built.
                return

        print('Building tools ...')
        tmp_path = path_join(script_path, 'tmp')
        if exists(tmp_path):
            rmtree(tmp_path)
        makedirs(tmp_path)
        check_call(['cmake', '..'], cwd=tmp_path)
        check_call(['cmake', '-DCMAKE_BUILD_TYPE=Release', '-DCMAKE_VERBOSE_MAKEFILE=1', '-G', 'Unix Makefiles', '..'], cwd=tmp_path)
        check_call(['cmake', '--build', '.', '--config', 'Release'], cwd=tmp_path)
        rmtree(tmp_path)
        print('Tools have been built.')

    def enumerate_projects(self) -> List[str]:
        def is_project(candidate: str):
            return isdir(path_join(self.root, candidate)) and project_pattern.match(candidate) is not None

        print('Enumerating projects ...')
        projects = sorted(candidate for candidate in listdir(self.root) if is_project(candidate))
        if not projects:
            print('No projects have been found')
            return []
        print('The following projects have been found:')
        for project in projects:
            print(f'* {project}')
        return projects

    def enumerate_project_documents(self, project: str) -> List[str]:
        def is_document(candidate: str):
            return isdir(path_join(self.root, project, candidate)) and document_pattern.match(candidate) is not None

        print(f'Enumerating project "{project}" documents ...')
        documents = sorted(candidate for candidate in listdir(path_join(self.root, project)) if is_document(candidate))
        if not documents:
            print('No documents have been found')
            return []
        print('The following documents have been found:')
        for document in documents:
            print(f'* {document}')
        return documents

    def build_documents(self, project: str):
        for document in self.enumerate_project_documents(project):
            print(f'Building document "{document}" for project "{project}" ...')
            document_type = document_pattern.match(document).group(2)
            if document_type is None:  # Спецификация
                builder = SpecificationBuilder(project, document)
                builder.build()
            elif document_type == 'ВП':  # Ведомость покупных изделий
                check_call(['make'], cwd=path_join(self.root, project, document))
            elif document_type == 'ПЭ3':  # Перечень элементов
                check_call(['make'], cwd=path_join(self.root, project, document))
            elif document_type == 'РЭ':  # Руководство по эксплуатации
                check_call(['make'], cwd=path_join(self.root, project, document))
            else:
                assert False, f'Unknown document type: {document_type}'
            print(f'Document "{document}" of project "{project}" has been successfully built.')

    def clean(self, project: str):
        print(f'Cleaning working copy of project "{project}" ...')
        check_call(['git', 'clean', '-ffdxq'], cwd=path_join(self.root, project))


def main() -> None:
    script_path = dirname(abspath(realpath(__file__)))

    arg_parser = ArgumentParser(description='Documentation builder')
    arg_parser.add_argument('--root', type=str, default=script_path, help='A path to search for projects at')
    arg_parser.add_argument('--clean', action='store_true', default=False, help='Clean working copy (git clean -ffdxq)')
    arg_parser.add_argument('--force_tools_rebuild', action='store_true', default=False, help='Forcibly rebuild generating tools')
    args = arg_parser.parse_args()

    check_prerequisites()

    builder = Builder(args.root)
    builder.build_tools(script_path, args.force_tools_rebuild)

    for project in builder.enumerate_projects():
        if args.clean:
            builder.clean(project)
        else:
            builder.build_documents(project)


if __name__ == '__main__':
    try:
        main()
    except (CalledProcessError, DocumentBuilderException) as e:
        print(f'Something went wrong: {str(e)}')
        exit(1)
