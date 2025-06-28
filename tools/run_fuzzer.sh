#!/bin/bash

# Build the fuzzer
echo "Building fuzzer..."
g++ -Wall -Wextra -O2 -o fuzzer fuzzer.cpp

if [ $? -ne 0 ]; then
    echo "Failed to build fuzzer"
    exit 1
fi

# Check if interpreter exists
if [ ! -x "../src/li" ]; then
    echo "Lithium interpreter not found. Building..."
    cd ../src && make && cd ../tools
    if [ ! -x "../src/li" ]; then
        echo "Failed to build interpreter"
        exit 1
    fi
fi

# Set number of tests (default 1000)
NUM_TESTS=${1:-1000}
LOG_FILE=${2:-"fuzzer_results.log"}

echo "Starting fuzzer with $NUM_TESTS test cases..."
echo "Results will be saved to: $LOG_FILE"

# Run the fuzzer
./fuzzer $NUM_TESTS $LOG_FILE

echo "Fuzzing completed. Check $LOG_FILE for detailed results."

# Show summary of interesting findings
if [ -f "$LOG_FILE" ]; then
    echo ""
    echo "=== SUMMARY ==="
    echo "Crashes found: $(grep -c "Crashed: YES" $LOG_FILE)"
    echo "Timeouts found: $(grep -c "Timeout: YES" $LOG_FILE)"
    echo "Non-zero exits: $(grep -c "Exit Code: [1-9]" $LOG_FILE)"

    # Show a few interesting cases
    echo ""
    echo "=== SAMPLE INTERESTING CASES ==="
    grep -A 20 "Crashed: YES\|Timeout: YES" $LOG_FILE | head -40
fi
