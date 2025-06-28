#!/usr/bin/env python3
"""
Pytest adapter for Lithium Interpreter tests
Integrates with VSCode's native testing features
"""

import pytest
import subprocess
import os
from pathlib import Path
import difflib

def get_workspace_root():
    """Find the workspace root directory"""
    current = Path(__file__).parent
    if (current / "src" / "li").exists():
        return current
    # Fallback to current directory
    return Path.cwd()

def discover_test_files():
    """Discover all Lithium test files"""
    workspace_root = get_workspace_root()
    test_dir = workspace_root / "test"
    
    test_files = []
    if test_dir.exists():
        for li_file in test_dir.rglob("*.li"):
            correct_file = li_file.with_suffix(".correct")
            if correct_file.exists():
                # Create relative path for test ID
                rel_path = li_file.relative_to(test_dir)
                test_id = str(rel_path.with_suffix("")).replace(os.sep, "/")
                test_files.append((test_id, li_file, correct_file))
    
    return test_files

def run_lithium_test(test_id, li_file, correct_file):
    """Run a single Lithium test"""
    workspace_root = get_workspace_root()
    interpreter = workspace_root / "src" / "li"
    
    if not interpreter.exists():
        pytest.fail(f"Lithium interpreter not found at {interpreter}")
    
    try:
        # Match the native test runner behavior:
        # - cd to test directory (or subdirectory for nested tests)
        # - run interpreter on just the filename
        
        test_dir = workspace_root / "test"
        if "/" in test_id:
            # For nested tests like "print/basic", cd to the print directory
            subdir_parts = test_id.split("/")[:-1]  # All but the last part
            test_dir = test_dir / "/".join(subdir_parts)
        
        filename = li_file.name  # Just the filename, like "basic.li"
        
        # Run the test from the correct directory with just the filename
        result = subprocess.run(
            [str(interpreter), filename],
            cwd=test_dir,
            capture_output=True,
            text=True,
            timeout=30
        )
        
        # Get actual output (stdout + stderr)
        actual_output = result.stdout + result.stderr
        
        # Read expected output
        with open(correct_file, 'r') as f:
            expected_output = f.read()
        
        # Compare outputs (normalize whitespace)
        actual_lines = [line.rstrip() for line in actual_output.splitlines()]
        expected_lines = [line.rstrip() for line in expected_output.splitlines()]
        
        if actual_lines != expected_lines:
            # Create a readable diff
            diff = '\n'.join(difflib.unified_diff(
                expected_lines,
                actual_lines,
                fromfile='expected',
                tofile='actual',
                lineterm=''
            ))
            pytest.fail(f"Test output mismatch:\n{diff}")
            
    except subprocess.TimeoutExpired:
        pytest.fail(f"Test {test_id} timed out after 30 seconds")
    except Exception as e:
        pytest.fail(f"Error running test {test_id}: {str(e)}")

# Use pytest.mark.parametrize to create individual test cases
test_cases = discover_test_files()
test_ids = [test_id for test_id, _, _ in test_cases]
test_files = [(li_file, correct_file) for _, li_file, correct_file in test_cases]

@pytest.mark.parametrize("li_file,correct_file", test_files, ids=test_ids)
def test_lithium_interpreter(li_file, correct_file):
    """Test runner for individual Lithium tests"""
    # Extract test_id from the file path
    workspace_root = get_workspace_root()
    test_dir = workspace_root / "test"
    rel_path = li_file.relative_to(test_dir)
    test_id = str(rel_path.with_suffix("")).replace(os.sep, "/")
    
    run_lithium_test(test_id, li_file, correct_file)
