#!/bin/bash

echo "=== Testing known crash cases ==="

# Create test files for the crashes found
echo 'fn a() { return b(); } fn b() { return a(); } a();' > crash1.li
echo 'class Test {} let x = Test; x();' > crash2.li
echo 'fn inf() { return inf(); } inf();' > crash3.li

echo "Testing crash 1 (mutual recursion)..."
../src/li crash1.li
echo "Exit code: $?"

echo ""
echo "Testing crash 2 (invalid class call)..."
../src/li crash2.li
echo "Exit code: $?"

echo ""
echo "Testing crash 3 (infinite recursion)..."
../src/li crash3.li
echo "Exit code: $?"

# Clean up
rm -f crash1.li crash2.li crash3.li

echo ""
echo "=== Fuzzer Summary ==="
echo "The fuzzer has found several critical issues:"
echo "1. Stack overflow from infinite recursion"
echo "2. Segmentation fault when calling non-function as function"
echo "3. No proper recursion depth limiting"
echo ""
echo "These should be fixed by adding:"
echo "- Recursion depth limits"
echo "- Better type checking before function calls"
echo "- Stack overflow protection"
