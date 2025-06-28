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

## Output Files

- **advanced_fuzzer_results.log** - Detailed results from advanced fuzzer
- **fuzzer_results.log** - Results from basic C++ fuzzer

## Notes

All tools expect to be run from the `tools/` directory and will look for the Lithium interpreter at `../src/li`. Make sure to compile the interpreter before running any fuzzing tools.
