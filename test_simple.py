#!/usr/bin/env python3
"""
Simple test to verify pytest integration works
"""

import subprocess
from pathlib import Path

def test_interpreter_exists():
    """Test that the Lithium interpreter binary exists"""
    interpreter = Path("src/li")
    assert interpreter.exists(), f"Lithium interpreter not found at {interpreter}"

def test_basic_lithium_execution():
    """Test that we can run a simple Lithium program"""
    # Create a simple test
    test_file = Path("test/print/basic.li")
    expected_file = Path("test/print/basic.correct")

    if not test_file.exists() or not expected_file.exists():
        return  # Skip if files don't exist

    # Run the test from the correct directory like the main test runner
    test_dir = Path("test/print")
    filename = "basic.li"
    interpreter = Path("../../src/li")

    result = subprocess.run(
        [str(interpreter), filename],
        cwd=test_dir,
        capture_output=True,
        text=True,
        timeout=10
    )

    # Read expected output
    with open(expected_file, 'r') as f:
        expected_output = f.read()

    actual_output = result.stdout + result.stderr

    # Normalize whitespace like the main test runner
    actual_lines = [line.rstrip() for line in actual_output.splitlines()]
    expected_lines = [line.rstrip() for line in expected_output.splitlines()]

    assert actual_lines == expected_lines
