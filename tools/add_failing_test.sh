#!/bin/bash

echo "Adding intentionally failing test for CI verification..."

# Create ci directory if it doesn't exist
mkdir -p "test/ci"

# Create the failing test
cat > "test/ci/failing_test.li" << 'EOF'
println("This test should fail!");
println("Expected output: SUCCESS");
println("Actual output: FAILURE");
EOF

# Create the expected output (which will cause the test to fail)
cat > "test/ci/failing_test.correct" << 'EOF'
This test should fail!
Expected output: SUCCESS
Actual output: SUCCESS
EOF

echo "✓ Created test/ci/failing_test.li"
echo "✓ Created test/ci/failing_test.correct"
echo "Done! The test suite will now have 1 failing test to verify CI failure detection."
