# Keybinding Test Instructions

To test if the keybindings are working correctly:

## Method 1: Test Build First
1. Press `F8` - This should run "Build Interpreter"
2. You should see the terminal open and show compilation output
3. If this works, the keybinding system is functional

## Method 2: Test Task Discovery
1. Press `Ctrl+Shift+P` 
2. Type "Tasks: Run Task"
3. You should see all the tasks listed:
   - Run All Tests
   - Run All Tests with Memory Check
   - Build Interpreter
   - Run Single Test
   - Run Advanced Fuzzer
   - Run Basic Fuzzer
   - Test Crash Cases
   - Run Pytest Tests
   - Run Pytest with Coverage

## Method 3: Test Individual Keybindings
After confirming the build works:
- `F6` - Run all tests (should show test output)
- `F9` - Run pytest tests (should show pytest output)
- `F7` - Run single test (should prompt for test name)
- `Shift+F6` - Run tests with memory check
- `Shift+F7` - Run advanced fuzzer (should prompt for number of tests)

## Troubleshooting

If keybindings don't work:
1. Make sure you have the Lithium project open as a workspace
2. Try reloading the window: `Ctrl+Shift+P` → "Developer: Reload Window"
3. Check if there are conflicting extensions
4. Try the function key shortcuts (F6, F7, F8, F9) as they're less likely to conflict

## Alternative Access
If keybindings still don't work, you can always access tasks via:
- `Ctrl+Shift+P` → "Tasks: Run Task" → Select task
- Terminal: `./run_all_tests`, `make -C src`, etc.
