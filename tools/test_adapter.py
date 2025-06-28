#!/usr/bin/env python3
"""
VSCode Test Adapter for Lithium Interpreter
Discovers and runs tests in a format compatible with VSCode Test Explorer
"""

import json
import os
import subprocess
import sys
from pathlib import Path

class LithiumTestAdapter:
    def __init__(self, workspace_root):
        self.workspace_root = Path(workspace_root)
        self.test_dir = self.workspace_root / "test"
        
    def discover_tests(self):
        """Discover all test files and return them in VSCode Test format"""
        tests = []
        
        if not self.test_dir.exists():
            return tests
            
        for category_dir in self.test_dir.iterdir():
            if category_dir.is_dir():
                category_tests = self._discover_category_tests(category_dir)
                if category_tests:
                    tests.append({
                        "type": "suite",
                        "id": category_dir.name,
                        "label": category_dir.name.title(),
                        "children": category_tests
                    })
        
        return tests
    
    def _discover_category_tests(self, category_dir):
        """Discover tests in a specific category directory"""
        tests = []
        
        for test_file in category_dir.glob("*.li"):
            # Skip if no corresponding .correct file
            correct_file = test_file.with_suffix(".correct")
            if not correct_file.exists():
                continue
                
            test_id = f"{category_dir.name}/{test_file.stem}"
            tests.append({
                "type": "test",
                "id": test_id,
                "label": test_file.stem,
                "file": str(test_file),
                "line": 1
            })
        
        return tests
    
    def run_test(self, test_id):
        """Run a specific test and return results"""
        try:
            # Use the existing run_test script
            result = subprocess.run(
                ["./run_test", test_id],
                cwd=self.workspace_root,
                capture_output=True,
                text=True,
                timeout=30
            )
            
            return {
                "type": "test",
                "test": test_id,
                "state": "passed" if result.returncode == 0 else "failed",
                "message": result.stdout + result.stderr if result.returncode != 0 else None
            }
            
        except subprocess.TimeoutExpired:
            return {
                "type": "test", 
                "test": test_id,
                "state": "failed",
                "message": "Test timed out after 30 seconds"
            }
        except Exception as e:
            return {
                "type": "test",
                "test": test_id, 
                "state": "failed",
                "message": f"Error running test: {str(e)}"
            }
    
    def run_all_tests(self):
        """Run all tests and return results"""
        try:
            result = subprocess.run(
                ["./run_all_tests"],
                cwd=self.workspace_root,
                capture_output=True,
                text=True,
                timeout=300  # 5 minutes
            )
            
            # Parse the output to get individual test results
            # This is a simplified parser - you might want to enhance it
            lines = result.stdout.split('\n')
            passed_count = 0
            failed_count = 0
            
            for line in lines:
                if "passed" in line.lower() and "failed" in line.lower():
                    # Extract counts from summary line
                    parts = line.split()
                    for i, part in enumerate(parts):
                        if part.endswith("passed.") or part.endswith("passed,"):
                            passed_count = int(parts[i-1].split("/")[0])
                        elif part.endswith("failed."):
                            failed_count = int(parts[i-1].split("/")[0])
            
            return {
                "type": "suite",
                "suite": "root",
                "state": "passed" if failed_count == 0 else "failed",
                "message": f"Tests: {passed_count} passed, {failed_count} failed"
            }
            
        except Exception as e:
            return {
                "type": "suite",
                "suite": "root", 
                "state": "failed",
                "message": f"Error running tests: {str(e)}"
            }

def main():
    if len(sys.argv) < 2:
        print("Usage: test_adapter.py <command> [args...]")
        sys.exit(1)
    
    command = sys.argv[1]
    workspace_root = os.getcwd()
    adapter = LithiumTestAdapter(workspace_root)
    
    if command == "discover":
        tests = adapter.discover_tests()
        print(json.dumps(tests, indent=2))
    
    elif command == "run":
        if len(sys.argv) < 3:
            # Run all tests
            result = adapter.run_all_tests()
        else:
            # Run specific test
            test_id = sys.argv[2]
            result = adapter.run_test(test_id)
        
        print(json.dumps(result, indent=2))
    
    else:
        print(f"Unknown command: {command}")
        sys.exit(1)

if __name__ == "__main__":
    main()
