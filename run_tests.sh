#!/bin/bash

dir="riscv-tests-64/bin"

EXTENSION="ui"
MEMORY_MODE="p"

for file in "$dir"/rv64$EXTENSION-$MEMORY_MODE-*; do
    if [ -f "$file" ]; then
        echo "Running simulation for: $file"
        ./build/bin/simlinx "$file"
    fi
done
