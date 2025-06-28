# VSCode Test Integration

## Pytest Integration ✅ COMPLETE

The Lithium interpreter now fully integrates with VSCode's native Test Explorer through pytest.

### Features

- **✅ Individual Test Discovery**: All 85 Lithium test cases are discovered as individual pytest tests
- **✅ Test Explorer Integration**: Tests appear in VSCode's Test Explorer panel  
- **✅ Run Individual Tests**: You can run single tests via pytest or the Test Explorer
- **✅ Debugging Support**: Set breakpoints and debug individual tests
- **✅ Command Palette**: Run tests via `Python: Run All Tests` command
- **✅ 100% Pass Rate**: All 85 tests pass in both pytest and native test runner

### Usage

1. **Test Explorer**: Open the Test Explorer panel (Testing icon in the Activity Bar)
2. **Run All Tests**: Use `Ctrl+Shift+P` → "Python: Run All Tests"
3. **Run Single Test**: Click the play button next to any test in the Test Explorer
4. **Command Line**: `python3 -m pytest test_lithium.py -v`

### VSCode Tasks

Additional pytest tasks are available in the Command Palette:
- "Run Pytest Tests" - Run all tests with verbose output
- "Run Pytest with Coverage" - Run with coverage and short traceback

### Test Compatibility ✅ FULLY FIXED

Both test runners are now 100% compatible:
- **Native Test Runner**: `./run_all_tests` - **85/85 tests pass**
- **Pytest Runner**: `python3 -m pytest` - **87/87 tests pass** (includes 2 additional compatibility tests)

The pytest runner matches the exact behavior of the native test runner by:
- Running tests from the correct working directory
- Using relative file paths (matching `FILE` builtin behavior)
- Preserving exact output formatting and whitespace
- Supporting all test types: core tests, error tests, builtin tests, etc.

### Configuration Files

- `test_lithium.py` - Pytest adapter for Lithium tests
- `pyproject.toml` - Pytest configuration
- `.vscode/settings.json` - VSCode Python testing settings
- `.vscode/tasks.json` - Custom test tasks
