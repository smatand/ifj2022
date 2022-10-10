#!/bin/sh

INPUT_CASES=$(ls scan_test_cases/scan_input)
EXPECTED_OUTPUT_CASES=$(ls scan_test_cases/scan_expected)

WORKING_DIR="scan_results/"
if [ ! -d "$WORKING_DIR" ]; then
    mkdir "$WORKING_DIR"
else
    rm -rf "$WORKING_DIR"
    mkdir "$WORKING_DIR"
fi

for file in $INPUT_CASES; do
    ../scanner_test <$file >$WORKING_DIR$file.out
    diff $WORKING_DIR$file.out scan_test_cases/scan_expected/$file >$WORKING_DIR$file.diff
    if [ -s $WORKING_DIR$file.diff ]; then
        echo "Test case $file failed"
        cat $WORKING_DIR$file.diff
        echo "----------------------------------------"
    else
        echo "Test case $file passed"
        echo "----------------------------------------"
    fi
done
