# VSCode Integration for Lithium Interpreter

This document explains how to use the VSCode integration features for developing and testing the Lithium interpreter.

## Quick Start

1. **Open the project in VSCode**
2. **Install recommended extensions**:
   - C/C++ (for debugging)
   - Test Explorer UI (for test integration)
   - Python (for fuzzing tools)

3. **Use keyboard shortcuts**:
   - `Ctrl+Shift+T` - Run all tests
   - `Ctrl+Shift+R` - Run single test
   - `Ctrl+Shift+M` - Run tests with memory check
   - `Ctrl+Shift+F` - Run advanced fuzzer
   - `Ctrl+Shift+B` - Build interpreter

## Available Tasks

Access these via `Ctrl+Shift+P` → "Tasks: Run Task":

### Testing Tasks
- **Run All Tests** - Execute the complete test suite
- **Run All Tests with Memory Check** - Run tests with Valgrind memory analysis
- **Run Single Test** - Run a specific test (prompts for test name)
- **Run Advanced Fuzzer** - Run the Python-based vulnerability fuzzer
- **Run Basic Fuzzer** - Run the C++ basic fuzzer
- **Test Crash Cases** - Run regression tests for known crash patterns

### Build Tasks
- **Build Interpreter** - Compile the Lithium interpreter

## Test Explorer Integration

The project includes a custom test adapter (`tools/test_adapter.py`) that provides:

- **Test Discovery** - Automatically finds all `.li` test files with corresponding `.correct` files
- **Hierarchical View** - Tests organized by category (array, class, errors, etc.)
- **Individual Test Execution** - Run specific tests from the explorer
- **Test Results** - Pass/fail status with error details

### Using the Test Adapter

```bash
# Discover all tests
python3 tools/test_adapter.py discover

# Run a specific test
python3 tools/test_adapter.py run array/basic

# Run all tests
python3 tools/test_adapter.py run
```

## Debugging

### Debug Configurations
Two debug configurations are available:

1. **Debug Lithium Interpreter** - Debug the interpreter with a custom file
2. **Debug Single Test** - Debug the interpreter running a specific test

### Starting a Debug Session
1. Open the Debug panel (`Ctrl+Shift+D`)
2. Select a debug configuration
3. Press F5 or click the green play button
4. Enter the file/test name when prompted

### Debugging Features
- **Breakpoints** - Set breakpoints in C++ source code
- **Variable Inspection** - Examine interpreter state
- **Call Stack** - View function call hierarchy
- **Step Debugging** - Step through code execution

## File Organization

```
.vscode/
├── tasks.json       # Task definitions
├── launch.json      # Debug configurations  
├── keybindings.json # Custom keyboard shortcuts
└── settings.json    # VSCode settings

tools/
├── test_adapter.py  # Test discovery and execution
├── advanced_fuzzer.py # Advanced fuzzing tool
├── fuzzer.cpp       # Basic C++ fuzzer
└── README.md        # Tools documentation
```

## Test Categories

Tests are organized into logical categories:

- **array/** - Array operations and indexing
- **assert/** - Assertion functionality
- **class/** - Class definition and instantiation
- **errors/** - Error handling and edge cases
- **expressions/** - Expression evaluation
- **functions/** - Function calls and closures
- **if/** - Conditional statements
- **import/** - Module importing
- **loops/** - Loop constructs
- **null/** - Null value handling
- **print/** - Output functions
- **samples/** - Example programs
- **scopes/** - Variable scoping
- **string/** - String operations
- **variables/** - Variable assignment

## Error Reporting

The integration provides several levels of error reporting:

1. **Build Errors** - C++ compilation errors with file/line information
2. **Test Failures** - Test execution failures with diff output
3. **Runtime Errors** - Interpreter crashes and error messages
4. **Memory Errors** - Valgrind memory leak and error reports

## Performance Testing

### Memory Testing
Run tests with memory analysis:
```bash
# Via task
Ctrl+Shift+P → "Tasks: Run Task" → "Run All Tests with Memory Check"

# Via command line
./run_all_tests_with_memory
```

### Fuzzing
Run vulnerability and robustness testing:
```bash
# Advanced fuzzer (Python)
Ctrl+Shift+F → Enter number of tests

# Basic fuzzer (C++)
cd tools && ./run_fuzzer.sh [num_tests]
```

## Customization

### Adding New Tasks
Edit `.vscode/tasks.json` to add custom tasks.

### Modifying Keybindings
Edit `.vscode/keybindings.json` to change keyboard shortcuts.

### Test Adapter Extension
The test adapter can be extended to support:
- Custom test formats
- Additional test metadata
- Performance benchmarking
- Coverage reporting

## Troubleshooting

### Common Issues

1. **"Interpreter not found"**
   - Run the Build Interpreter task first
   - Check that `src/li` exists and is executable

2. **"Tests not discovered"**
   - Ensure test files have corresponding `.correct` files
   - Check that test files are in the `test/` directory

3. **"Debug symbols not found"**
   - Rebuild with debug symbols: `make -C src clean && make -C src DEBUG=1`

4. **"Task not found"**
   - Reload VSCode window: `Ctrl+Shift+P` → "Developer: Reload Window"

### Getting Help

- Check the Problems panel for build/syntax errors
- View the Terminal panel for task output
- Check the Debug Console for debugging information
- Review the Output panel for detailed logs

## Extensions

### Recommended Extensions
- **C/C++** (ms-vscode.cpptools) - C++ language support
- **Test Explorer UI** (hbenl.vscode-test-explorer) - Test integration
- **Python** (ms-python.python) - Python support for fuzzing tools
- **GitLens** (eamodio.gitlens) - Git integration and history

### Optional Extensions
- **Clang-Format** (xaver.clang-format) - Code formatting
- **CMake Tools** (ms-vscode.cmake-tools) - Build system integration
- **Valgrind** (webfreak.debug) - Memory debugging
