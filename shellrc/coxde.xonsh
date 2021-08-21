import os
import os.path
import subprocess
from typing import List

import xonsh.dirstack
import xonsh.environ


def coxide_bin() -> str:
    """Finds and returns the location of the coxide binary."""
    bin = xonsh.environ.locate_binary("coxide")
    if bin is None:
        bin = "coxide"
    return bin


def o(args: List[str]):
    """Jump to a directory using only keywords."""
    if args == []:
        xonsh.dirstack.cd([])
    # elif args == ["-"]:
    elif len(args) == 1 and os.path.isdir(args[0]):
        subprocess.run([coxide_bin(), "-", args[0]])
        xonsh.dirstack.cd(args[0])
    else:
        coxide = coxide_bin()
        cmd = subprocess.run(
            [coxide, args[0]],
            check=True,
            stdout=subprocess.PIPE,
        )
        result = cmd.stdout[:-1]
        xonsh.dirstack.cd(result)
