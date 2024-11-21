#!/usr/bin/env bash

# Choose current directory
SCRIPT_DIR=$(dirname "$(realpath $0)")
cd $SCRIPT_DIR

# Create 'lib' directory
if ! [[ -d lib ]]; then
  mkdir lib
fi

cd lib

# Get host info
KERNEL=$(uname)
ARCH=$(uname -m)

# Install library
if [[ $KERNEL == "Linux" && $ARCH == "x86_64" ]] then
  if ! [[ -f LIEF-0.15.1-Linux-x86_64.tar.gz ]] then
    wget https://github.com/lief-project/LIEF/releases/download/0.15.1/LIEF-0.15.1-Linux-x86_64.tar.gz
  fi
  tar -xf LIEF-0.15.1-Linux-x86_64.tar.gz
  mv LIEF-0.15.1-Linux-x86_64 LIEF
else
  echo "There is no processing of your system configuration right now."
  echo "Great reason to make you a pull-request :)"
fi
