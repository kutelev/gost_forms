from builtins import print as original_print
from sys import stderr
from subprocess import check_call as original_check_call, list2cmdline
from typing import List, Optional, Dict
from os import environ


# noinspection PyShadowingBuiltins
def print(*args) -> None:
    original_print(*args, file=stderr, flush=True)


def check_call(cmd: List[str], *, cwd: str, env: Optional[Dict[str, str]] = None, **kwargs) -> None:
    print(f'Executing: {list2cmdline(cmd)} (in {cwd})')
    if env is None:
        env = {}
    original_check_call(cmd, cwd=cwd, env={**environ, **env}, **kwargs)
