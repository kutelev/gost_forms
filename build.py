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

script_path = dirname(abspath(realpath(__file__)))
project_pattern = compile(r'^[А-Я]{4}\.\d{6}\.\d{3} \(.*\)$')
document_pattern = compile(r'^[А-Я]{4}\.\d{6}\.\d{3} ((ВП|ПЭ3|РЭ) )?\(.*\)$')


# noinspection PyShadowingBuiltins
def print(*args) -> None:
    original_print(*args, file=stderr, flush=True)


def check_prerequisites():
    if system() != 'Linux':
        print('Error: nothing but Linux is supported.')
        exit(1)


def build_tools():
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


def enumerate_projects() -> List[str]:
    def is_project(candidate: str):
        return isdir(path_join(script_path, candidate)) and project_pattern.match(candidate) is not None

    print('Enumerating projects ...')
    projects = sorted(candidate for candidate in listdir(script_path) if is_project(candidate))
    if not projects:
        print('No projects have been found')
        return []
    print('The following projects have been found:')
    for project in projects:
        print(f'* {project}')
    return projects


def enumerate_project_documents(project: str) -> List[str]:
    def is_document(candidate: str):
        return isdir(path_join(script_path, project, candidate)) and document_pattern.match(candidate) is not None

    print(f'Enumerating project "{project}" documents ...')
    documents = sorted(candidate for candidate in listdir(path_join(script_path, project)) if is_document(candidate))
    if not documents:
        print('No documents have been found')
        return []
    print('The following documents have been found:')
    for document in documents:
        print(f'* {document}')
    return documents


def build_documents(project: str):
    for document in enumerate_project_documents(project):
        print(f'Building document "{document}" for project "{project}" ...')
        document_type = document_pattern.match(document).group(2)
        if document_type is None:  # Спецификация
            check_call(['make'], cwd=path_join(script_path, project, document))
        elif document_type == 'ВП':  # Ведомость покупных изделий
            check_call(['make'], cwd=path_join(script_path, project, document))
        elif document_type == 'ПЭ3':  # Перечень элементов
            check_call(['make'], cwd=path_join(script_path, project, document))
        elif document_type == 'РЭ':  # Руководство по эксплуатации
            check_call(['make'], cwd=path_join(script_path, project, document))
        else:
            assert False, f'Unknown document type: {document_type}'
        print(f'Document "{document}" of project "{project}" has been successfully built.')


def main() -> None:
    check_prerequisites()
    build_tools()
    for project in enumerate_projects():
        build_documents(project)


if __name__ == '__main__':
    try:
        main()
    except CalledProcessError as e:
        print(f'Something went wrong: {str(e)}')
        exit(1)
