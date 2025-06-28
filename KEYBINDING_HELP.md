# VSCode Keybinding Troubleshooting Guide

If the keybindings aren't working, here are several approaches to try:

## Approach 1: Function Keys (Recommended)
The simplest and most reliable keybindings:
- `F8` - Build Interpreter
- `F6` - Run All Tests  
- `F9` - Run Pytest Tests
- `F7` - Run Single Test
- `Shift+F6` - Run Tests with Memory Check
- `Shift+F7` - Run Advanced Fuzzer

## Approach 2: Command Palette (Always Works)
Use `Ctrl+Shift+P` then type:
- "Tasks: Run Task" → Select any task
- "Python: Run All Tests" → For pytest integration
- "Build" → Quick build

## Approach 3: Terminal (Direct)
Open terminal (`Ctrl+Shift+``) and run:
```bash
# Build interpreter
make -C src

# Run all tests
./run_all_tests

# Run pytest tests
python3 -m pytest test_lithium.py

# Run single test
./run_test array/basic

# Run fuzzer
python3 tools/advanced_fuzzer.py 100
```

## Approach 4: VSCode Task Bar
1. Open Command Palette (`Ctrl+Shift+P`)
2. Type "Tasks: Run Task"
3. Select from the list of available tasks

## Troubleshooting Steps

### Step 1: Verify Workspace
Make sure you opened the `lithium` folder as a workspace:
1. File → Open Folder → Select the lithium directory
2. NOT just opening individual files

### Step 2: Reload Window
1. `Ctrl+Shift+P`
2. Type "Developer: Reload Window"
3. Try keybindings again

### Step 3: Check Extensions
Some extensions can interfere with keybindings:
1. Try disabling other extensions temporarily
2. Restart VSCode

### Step 4: Manual Task Verification
Test if tasks work via menu:
1. `Ctrl+Shift+P`
2. "Tasks: Run Task"
3. Select "Build Interpreter"
4. If this works, keybindings are the only issue

### Step 5: Alternative Keybinding Format
If nothing works, you can edit `.vscode/keybindings.json` to try:
```json
[
    {
        "key": "f8",
        "command": "workbench.action.terminal.sendSequence",
        "args": {
            "text": "make -C src\\r"
        }
    }
]
```

## Why Keybindings Might Not Work

1. **Extension Conflicts** - Other extensions using the same keys
2. **OS Conflicts** - System shortcuts overriding VSCode
3. **Workspace Context** - Not opened as a proper workspace
4. **VSCode Version** - Some older versions have different syntax
5. **User Settings** - Global VSCode settings overriding project settings

## Recommended Workflow

Even without keybindings, the development workflow is excellent:

1. **Build**: `Ctrl+Shift+P` → "Tasks: Run Task" → "Build Interpreter"
2. **Test**: `Ctrl+Shift+P` → "Tasks: Run Task" → "Run All Tests"
3. **Debug**: `Ctrl+Shift+D` → Select debug config → F5
4. **Python Tests**: Use VSCode's Test Explorer panel

The important part is that all the tasks and integration work perfectly - keybindings are just a convenience!
