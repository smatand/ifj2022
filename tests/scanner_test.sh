#!/bin/sh

INPUT_CASES=$(ls scan_test_cases/scan_input)
#EXPECTED_OUTPUT_CASES=$(ls scan_test_cases/scan_expected)

INPUT_DIR="scan_test_cases/scan_input"
EXPECTED_DIR="scan_test_cases/scan_expected"
OUTPUT_DIR="scan_results/output"
RESULT_DIR="scan_results/diff"

# creating or emptying ouput directories
if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir "$OUTPUT_DIR"
else
    rm -rf "$OUTPUT_DIR"
    mkdir "$OUTPUT_DIR"
fi

if [ ! -d "$RESULT_DIR" ]; then
    mkdir "$RESULT_DIR"
else
    rm -rf "$RESULT_DIR"
    mkdir "$RESULT_DIR"
fi

# remaking scanner_test binary
cd .. && make scanner_test && cd tests

for file in $INPUT_CASES; do
    ../scan_test < "$INPUT_DIR/$file" > "$OUTPUT_DIR/$file.out"
    diff -u "$OUTPUT_DIR/$file.out" "$EXPECTED_DIR/$file.expected" > "$RESULT_DIR/$file.diff"
    if [ -s "$RESULT_DIR/$file.diff" ]; then
        echo "TEST CASE: $file";
        echo "-------------------- FAILED --------------------";
        cat "$RESULT_DIR/$file.diff"
        echo "------------------------------------------------";
    else
        echo "TEST CASE: $file";
        echo "-------------------- PASSED --------------------";
        echo "------------------------------------------------";
    fi
done