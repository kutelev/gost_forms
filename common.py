from builtins import print as original_print
from sys import stderr
from subprocess import check_call as original_check_call, list2cmdline
from typing import List


# noinspection PyShadowingBuiltins
def print(*args) -> None:
    original_print(*args, file=stderr, flush=True)


def check_call(cmd: List[str], *, cwd: str) -> None:
    print(f'Working directory: {cwd}')
    print(f'Command line: {list2cmdline(cmd)}')
    original_check_call(cmd, cwd=cwd)
