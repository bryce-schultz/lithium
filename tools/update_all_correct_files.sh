#!/usr/bin/env bash
# Regenerate all .correct files from .out files for all tests in the test/ directory
# Usage: ./tools/update_all_correct_files.sh

set -e

cd "$(dirname "$0")/.."  # Go to project root

find test -type f -name '*.out' | while read -r out_file; do
    correct_file="${out_file%.out}.correct"
    echo "Updating $correct_file from $out_file"
    cp "$out_file" "$correct_file"
done

echo "All .correct files updated from .out files."
