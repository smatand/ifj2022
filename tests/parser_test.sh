#!/bin/sh

INPUT_DIR="parserIn"
STDIN="parserIn/stdin"
EXPECTED_DIR="parserEx"
INPUT_CASES=$(ls -p "$INPUT_DIR/" | grep -v / | cut -f1 -d.)


RESULT_FILE="temp"
OUTPUT="toCompile"
RETURN="retVal"
COMPLETE_OUTPUT="current_output"

TEST_EXECUTABLE="./../src/ifj22"
INTERPRET="ic22int"

if [ -f "$TEST_EXECUTABLE" ]; then
    echo "==================================================="
    echo "Running $TEST_EXECUTABLE"
    echo "==================================================="
else
    echo "Cannot execute ifj22 binary. Missing file."
    exit 1
fi

for TEST_CASE in $INPUT_CASES
do
    echo "Testing $TEST_CASE"
    echo "---------------------------------------------------"
    ./$TEST_EXECUTABLE < "$INPUT_DIR/$TEST_CASE.php" > $RESULT_FILE 2>/dev/null
    ./$INTERPRET $RESULT_FILE > $OUTPUT
    # first check for err
    diff $? "$EXPECTED_DIR/ret/$TEST_CASE.err"
    if [ $? -eq 0 ]
    then
        echo "PASSED"
        diff $OUTPUT "$EXPECTED_DIR/$TEST_CASE.expected"
    else
        echo "ERROR CODE FAILED"
        echo EXPECTED: ____________________________________________________________
        echo $EXPECTED_DIR/${TEST_CASE}.err
        echo ______________________________________________________________________
        echo GOT 
        echo $ERROR
        echo ______________________________________________________________________
        continue
    fi
    echo "---------------------------------------------------"
done

rm $RESULT_FILE
rm $OUTPUT
