# Continuous Integration

This project uses GitHub Actions for continuous integration. The CI pipeline automatically builds the interpreter and runs comprehensive tests on every push and pull request.

## CI Workflow

The CI pipeline consists of two main jobs:

### 1. Test Job (`test`)
- **Platform**: Ubuntu Latest
- **Python**: 3.12 (for fuzzer tools only)
- **Dependencies**: 
  - Build tools: `gcc`, `g++`, `make`
  - Memory tools: `valgrind`

**Steps:**
1. **Build Verification**: Clean build of the interpreter
2. **PATH Setup**: Add interpreter to PATH for test runner compatibility
3. **Basic Functionality**: Run a simple test program to verify core functionality
4. **Native Test Suite**: Execute all 97+ native Lithium tests using `./run_all_tests`
5. **Sample Programs**: Test interpreter with sample programs (grid, pascal)
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

## File Structure

```
.github/
├── workflows/
│   └── ci.yml              # Main CI workflow
└── test_ci_basic.li        # Basic functionality test for CI

test/ci/
└── failing_test.li         # Intentionally failing test to verify CI detection
```

## Running Tests Locally

To run the same tests that CI runs:

```bash
# Build interpreter
make -C src

# Add interpreter to PATH (required for test runner)
export PATH="$(pwd)/src:$PATH"

# Run native tests
./run_all_tests

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
5. **Exit Codes**: Test runners properly exit with code 1 on failure, code 0 on success
6. **Build System**: The build script uses parallel compilation by default for faster local builds

**Note**: There is an intentionally failing test (`test/ci/failing_test.li`) to verify CI failure detection. 

To remove it for clean builds:
```bash
./tools/remove_failing_test.sh
```

To restore it for CI testing:
```bash
./tools/add_failing_test.sh
```

## Adding New Tests

When adding new tests:

1. Add `.li` test files to appropriate directories in `test/`
2. Include corresponding `.correct` files with expected output
3. Ensure tests run quickly (avoid infinite loops or long-running operations)
4. Test locally before pushing to verify CI compatibility
