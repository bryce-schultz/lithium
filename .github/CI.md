# Continuous Integration

This project uses GitHub Actions for continuous integration. The CI pipeline automatically builds the interpreter and runs comprehensive tests on every push and pull request.

## CI Workflow

The CI pipeline consists of two main jobs:

### 1. Test Job (`test`)
- **Platform**: Ubuntu Latest
- **Python**: 3.12
- **Dependencies**: 
  - Build tools: `gcc`, `g++`, `make`
  - Memory tools: `valgrind`
  - Python testing: `pytest` and other dev dependencies

**Steps:**
1. **Build Verification**: Clean build of the interpreter
2. **Basic Functionality**: Run a simple test program to verify core functionality
3. **Native Test Suite**: Execute all 97+ native Lithium tests using `./run_all_tests`
4. **Python Test Suite**: Run pytest-based tests for compatibility and integration
5. **Sample Programs**: Test interpreter with sample programs (grid, pascal)

### 2. Memory Test Job (`memory-test`)
- **Platform**: Ubuntu Latest
- **Dependencies**: Same as test job plus memory debugging tools
- **Purpose**: Run memory leak detection tests using valgrind

**Steps:**
1. **Build**: Clean build of the interpreter
2. **Memory Leak Tests**: Run `./run_all_tests_with_memory` with timeout protection

## Test Coverage

The CI pipeline covers:

- **Language Features**: All core language constructs (classes, functions, arrays, etc.)
- **Built-in Functions**: Type checking, string operations, I/O, etc.
- **Error Handling**: Syntax errors, runtime errors, edge cases
- **Memory Safety**: Memory leak detection and cleanup verification
- **Integration**: Python test compatibility and VSCode integration

## File Structure

```
.github/
├── workflows/
│   └── ci.yml              # Main CI workflow
└── test_ci_basic.li        # Basic functionality test for CI
```

## Running Tests Locally

To run the same tests that CI runs:

```bash
# Install dependencies
./setup_dev.sh

# Build interpreter
make -C src

# Run native tests
./run_all_tests

# Run Python tests
python -m pytest -v

# Run memory tests (optional)
./run_all_tests_with_memory
```

## CI Badge

The CI status is displayed in the main README with a badge that shows:
- ✅ Green: All tests passing
- ❌ Red: Tests failing
- 🟡 Yellow: Tests running

## Troubleshooting CI Issues

If CI fails:

1. **Build Errors**: Check for missing dependencies or compilation issues
2. **Test Failures**: Review test output to identify which specific tests failed
3. **Memory Issues**: Check valgrind output for memory leaks or access violations
4. **Timeout Issues**: Some tests have timeouts to prevent hanging in CI

## Adding New Tests

When adding new tests:

1. Add `.li` test files to appropriate directories in `test/`
2. Include corresponding `.correct` files with expected output
3. Ensure tests run quickly (avoid infinite loops or long-running operations)
4. Test locally before pushing to verify CI compatibility
