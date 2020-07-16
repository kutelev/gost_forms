from os import makedirs
from os.path import dirname, abspath, realpath, join as path_join, exists
from platform import system
from shutil import rmtree
from subprocess import check_call, CalledProcessError
from sys import exit


script_path = dirname(abspath(realpath(__file__)))


def check_prerequisites():
    if system() != 'Linux':
        print('Error: nothing but Linux is supported.')
        exit(1)


def build_tools():
    tmp_path = path_join(script_path, 'tmp')
    if exists(tmp_path):
        rmtree(tmp_path)
    makedirs(tmp_path)
    check_call(['cmake', '..'], cwd=tmp_path)
    check_call(['cmake', '-DCMAKE_BUILD_TYPE=Release', '-DCMAKE_VERBOSE_MAKEFILE=1', '-G', 'Unix Makefiles', '..'], cwd=tmp_path)
    check_call(['cmake', '--build', '.', '--config', 'Release'], cwd=tmp_path)
    rmtree(tmp_path)


def build_documents():
    check_call(['make', 'clean'], cwd=path_join(script_path, 'АБВГ.123456.001 (Квантовый телепортатор)'))
    check_call(['make'], cwd=path_join(script_path, 'АБВГ.123456.001 (Квантовый телепортатор)'))


def main() -> None:
    check_prerequisites()
    build_tools()
    build_documents()


if __name__ == '__main__':
    try:
        main()
    except CalledProcessError as e:
        print('Something went wrong: {}'.format(str(e)))
        exit(1)
