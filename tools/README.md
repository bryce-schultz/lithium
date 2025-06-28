# Lithium Interpreter Tools

This directory contains development and testing tools for the Lithium interpreter.

## Fuzzing Tools

### advanced_fuzzer.py
Advanced Python-based fuzzer that targets specific vulnerability patterns.

**Usage:**
```bash
cd tools
python3 advanced_fuzzer.py [num_tests]
```

**Features:**
- Parser stress testing
- Memory stress testing 
- Type confusion testing
- Control flow testing
- Mutation-based testing
- Detailed crash reporting

### fuzzer.cpp
Basic C++ fuzzer for high-volume random testing.

**Usage:**
```bash
cd tools
./run_fuzzer.sh
```

**Features:**
- Random input generation
- Fast execution
- Basic crash detection

### run_fuzzer.sh
Shell script to build and run the C++ fuzzer.

### test_crashes.sh
Regression test script for known crash patterns.

**Usage:**
```bash
cd tools
./test_crashes.sh
```

## CI Testing Tools

### add_failing_test.sh
Creates an intentionally failing test to verify CI failure detection.

**Usage:**
```bash
./tools/add_failing_test.sh
```

**Features:**
- Creates `test/ci/failing_test.li` and corresponding `.correct` file
- Ensures CI properly detects and reports test failures
- Useful for verifying CI configuration

### remove_failing_test.sh
Removes the intentionally failing test for clean builds.

**Usage:**
```bash
./tools/remove_failing_test.sh
```

**Features:**
- Removes `test/ci/failing_test.*` files
- Cleans up empty directories
- Enables clean CI builds for production

### verify_ci.sh
Comprehensive verification script for CI components.

**Usage:**
```bash
./tools/verify_ci.sh
```

**Features:**
- Tests interpreter build process
- Verifies basic functionality
- Checks exit code behavior for both success and failure
- Tests both native and pytest test runners
- Comprehensive CI readiness verification

## Output Files

- **advanced_fuzzer_results.log** - Detailed results from advanced fuzzer
- **fuzzer_results.log** - Results from basic C++ fuzzer

## Notes

All tools expect to be run from the `tools/` directory and will look for the Lithium interpreter at `../src/li`. Make sure to compile the interpreter before running any fuzzing tools.
