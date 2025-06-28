#!/bin/bash

echo "Removing intentionally failing test for clean CI builds..."

if [ -f "test/ci/failing_test.li" ]; then
    rm "test/ci/failing_test.li"
    echo "✓ Removed test/ci/failing_test.li"
else
    echo "✗ test/ci/failing_test.li not found"
fi

if [ -f "test/ci/failing_test.correct" ]; then
    rm "test/ci/failing_test.correct"
    echo "✓ Removed test/ci/failing_test.correct"
else
    echo "✗ test/ci/failing_test.correct not found"
fi

# Remove directory if empty
if [ -d "test/ci" ] && [ -z "$(ls -A test/ci)" ]; then
    rmdir "test/ci"
    echo "✓ Removed empty test/ci directory"
fi

echo "Done! You can now run tests with a clean build."
