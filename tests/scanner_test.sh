#!/bin/sh

#EXPECTED_OUTPUT_CASES=$(ls scan_test_cases/scan_expected)
INPUT_DIR="scan_input"
INPUT_CASES=$(ls "$INPUT_DIR/" | cut -f1 -d.)

EXPECTED_DIR="scan_expected"

RESULT_FILE="temp"
COMPLETE_OUTPUT="current_output"

TEST_EXECUTABLE="scan_test"

if [ -f "$TEST_EXECUTABLE" ]; then
    echo "==================================================="
    echo "Running $TEST_EXECUTABLE"
    echo "==================================================="
else
    echo "Cannot execute scan_test binary. Missing file."
    exit 1
fi

for TEST_CASE in $INPUT_CASES
do
    echo "Testing $TEST_CASE"
    echo "---------------------------------------------------"
    ./$TEST_EXECUTABLE < "$INPUT_DIR/$TEST_CASE.input" > $RESULT_FILE
    diff $EXPECTED_DIR/$TEST_CASE.expected $RESULT_FILE #| tee $COMPLETE_OUTPUT
    if [ $? -eq 0 ]
    then
        echo "PASSED"
    else
        echo "FAILED"
    fi
    echo "---------------------------------------------------"
done

rm $RESULT_FILE
