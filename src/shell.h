#include <stdio.h>

void posixsh(){
  puts(
      "o(){\n"
      "if [ \"$#\" -eq 0 ]; then\n"
      "\t\\builtin cd ~\n"
      "elif [ \"$#\" -eq 1 ] && [ \"$1\" = '-' ]; then\n"
      "\t\\builtin popd\n"
      "elif [ \"$#\" -eq 1 ] && [ -d \"$1\" ]; then\n"
      "\t\\coxide - \"$1\"\n"
      "\t\\builtin cd \"$1\"\n"
      "else\n"
      "\t\\builtin cd $(coxide $1)\n"
      "fi\n"
      "}\n"
      );
}

void xonsh(){
  puts(
      "import os.path\n"
      "import subprocess\n"
      "from typing import List\n"
      "import xonsh.dirstack\n"
      "import xonsh.environ\n"
      "\n"
      "\n"
      "def coxide_bin() -> str:\n"
      "\tbin = xonsh.environ.locate_binary(\"coxide\")\n"
      "\tif bin is None:\n"
      "\t\tbin = \"coxide\"\n"
      "\treturn bin\n"
      "\n"
      "\n"
      "def o(args: List[str]):\n"
      "\tif args == []:\n"
      "\t\txonsh.dirstack.cd([])\n"
      "\t# elif args == [\"-\"]:\n"
      "\telif len(args) == 1 and os.path.isdir(args[0]):\n"
      "\t\tsubprocess.run([coxide_bin(), \"-\", args[0]])\n"
      "\t\txonsh.dirstack.cd(args[0])\n"
      "\telse:\n"
      "\t\tcoxide = coxide_bin()\n"
      "\t\tcmd = subprocess.run(\n"
      "[coxide, args[0]],\n"
      "check=True,\n"
      "stdout=subprocess.PIPE,\n"
      ")\n"
      "result = cmd.stdout[:-1]\n"
      "xonsh.dirstack.cd(result)\n"
      );
}

void elvish(){
  puts(
      "fn o [@rest]{\n"
      "\tif (builtin:eq [] $rest) {\n"
      "\t\tcd ~\n"
      "\t} elif (builtin:eq [-] $rest) {\n"
      "\t\tpopd\n"
      "\t} elif (and (builtin:eq (builtin:count $rest) 1) (path:is-dir $rest[0])) {\n"
      "\t\tcoxide - $rest[0]\n"
      "\t\tcd $rest[0]\n"
      "\t} else {\n"
      "\t\tcd (coxide $rest[0])\n"
      "\t}\n"
      );
}

void fish(){
  puts(
      "function o\n"
      "\tset argc (count $argv)\n"
      "\tif test $argc -eq 0\n"
      "\t\tcd $HOME\n"
      "\telse if test \"$argv\" = -\n"
      "\t\tpopd\n"
      "\telse if begin\n"
      "\t\t\ttest $argc -eq 1; and test -d $argv[1]\n"
      "\t\tend\n"
      "\t\tcoxide - $argv[1]\n"
      "\t\tcd $argv[1]\n"
      "\telse\n"
      "\t\tset -l coxide_result (command coxide $argv[1])\n"
      "\t\tand cd $coxide_result\n"
      "\tend\n"
      "end\n"
      );
}

void powershell(){ // lol
  puts(
      "function o {\n"
      "\tif ($args.Length -eq 0) {\n"
      "\t\tcd ~\n"
      "\t}\n"
      "\telseif ($args.Length -eq 1 -and $args[0] -eq '-') {\n"
      "\t\tpopd\n"
      "\t}\n"
      "\telseif ($args.Length -eq 1 -and (Test-Path $args[0] -PathType Container)) {\n"
      "\t\tcoxide - $args[0]\n"
      "\t\t\tcd $args[0]\n"
      "\t}\n"
      "\telse {\n"
      "\t\t$result = coxide @args\n"
      "\t\t\tcd $result\n"
      "\t}\n"
      "}\n"
      );
}
