#!/bin/zsh

o(){
  if [ "$#" -eq 0 ]; then
    \builtin cd ~
  elif [ "$#" -eq 1 ] && [ "$1" = '-' ]; then
    \builtin popd
  elif [ "$#" -eq 1 ] && [ -d "$1" ]; then
    coxide - "$1"
    \builtin cd "$1"
  else
    \builtin cd $(coxide $1)
  fi
}

