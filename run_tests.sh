#!/bin/bash

dir="riscv-tests-64/bin"

EXTENSION="ui"
MEMORY_MODE="p"

if [ ! -z "$1" ]; then
    EXTENSION="$1"
fi

# Check if a new MEMORY_MODE was passed as an argument
if [ ! -z "$2" ]; then
    MEMORY_MODE="$2"
fi

for file in "$dir"/rv64$EXTENSION-$MEMORY_MODE-*; do
    if [ -f "$file" ]; then
        echo "Running simulation for: $file"
        ./build/bin/simlinx "$file"
    fi
done
