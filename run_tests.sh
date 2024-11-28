#!/bin/bash

# Directory containing the files
dir="riscv-tests-64/bin"

# Loop over all files matching the pattern
for file in "$dir"/rv64ui-p-*; do
    # Check if it's a regular file (not a directory)
    if [ -f "$file" ]; then
        echo "Running simulation for: $file"
        # Run the command for the current file
        ./build/bin/simlinx "$file"
    fi
done
