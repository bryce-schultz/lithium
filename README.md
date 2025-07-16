# Lithium Programming Language

Lithium is a lightweight, interpreted programming language designed for clarity, expressiveness, and ease of embedding. It features a C-like syntax, dynamic typing, first-class functions, closures, arrays, and a small but powerful set of built-in functions. Lithium is ideal for scripting, teaching, and rapid prototyping.

---

## Quick Start

### 🚀 Build & Setup
```bash
# Clone the repository
git clone <repository-url>
cd lithium

# Option 1: Quick build (recommended - uses parallel compilation)
./build

# Option 2: Development setup with dependencies
./setup_dev.sh

# Option 3: Manual build
make -C src
```

### 🏃 Run Your First Program
```bash
# Create a simple program
echo 'println("Hello, Lithium!");' > hello.li

# Run it
./src/li hello.li

# Or use interactive mode (REPL)
./src/li
# lithium 0.3
# type 'exit' to quit.
# > println("Interactive mode!");
# > let x = 5 + 3;
# > println("Result:", x);
# Interactive mode!
# Result: 8
# >
```

### 🧪 Testing
```bash
# Run all native tests
./run_all_tests

# Run Python integration tests  
python3 -m pytest -v

# Run memory leak tests (optional)
./run_all_tests_with_memory

# Run a specific test
./run_test string/basic_object_concat
```

### 🛠️ Development Tools
```bash
# Build uses parallel compilation by default for faster builds
./build

# Add/remove CI test verification
./tools/add_failing_test.sh      # Add failing test
./tools/remove_failing_test.sh   # Remove for clean builds

# Run fuzzing tests
./tools/run_fuzzer.sh
```

### 📁 Project Structure
```
lithium/
├── src/           # C++ interpreter source
├── test/          # Comprehensive test suite  
├── tools/         # Development and testing tools
├── modules/       # Standard library modules
├── .github/       # CI/CD workflows
└── build          # Main build script
```

### 🔧 Troubleshooting
- **Build fails**: Ensure you have `gcc`, `g++`, and `make` installed
- **Tests not found**: Run tests from project root directory
- **Interpreter not in PATH**: Add `export PATH="$(pwd)/src:$PATH"` or use `./src/li`
- **Permission denied**: Make scripts executable with `chmod +x <script>`

---

## Features

- **C-like syntax** with blocks, expressions, and statements
- **Dynamic typing**: numbers, strings, booleans, arrays, and user-defined classes
- **First-class functions** and closures
- **Block scoping** with `let` and `const`
- **Array literals** and indexing
- **Simple class system** with methods and properties
- **Control flow**: `if`, `else`, `while`, `for`, `foreach`, `break`, `continue`
- **Built-in constants**: `true`, `false`, `null`, `PI`, `E`, `VERSION`, `FILE`, `LINE`
- **Interactive REPL** and script execution
- **Clear error messages** with source locations
- **Modular imports** for builtins and user modules

---

## Getting Started

### Running a Script

```sh
li your_script.li
```

### Interactive Mode (REPL)

Just run `li` with no arguments:

```sh
li
```

You'll see:

```
lithium 0.3
type 'exit' to quit.
> 
```

Type code and see results immediately:

```
> let greeting = "Hello, Lithium!";
> println(greeting);
Hello, Lithium!
> let x = 5 + 3;
> println("Result:", x);
Result: 8
> exit
```

### Interactive Examples

The REPL is perfect for experimenting with language features:

**Functions and Closures:**
```
> fn createCounter() {
>     let count = 0;
>     fn increment() {
>         count++;
>         return count;
>     }
>     return increment;
> }
> let counter = createCounter();
> counter();
1
> counter();
2
```

**Arrays and Loops:**
```
> let numbers = [1, 2, 3, 4, 5];
> for (let i = 0; i < numbers.length; i++) {
>     println("Number:", numbers[i]);
> }
Number: 1
Number: 2
Number: 3
Number: 4
Number: 5
```

**Classes and Objects:**
```
> class Person {
>     let name = "";
>     fn Person(n) { name = n; }
>     fn greet() { println("Hello, I'm", name); }
> }
> let person = Person("Alice");
> person.greet();
Hello, I'm Alice
```

---

## Imports and Builtins

There are a few always-available built-in modules in Lithium, which do not require imports.
These include:
- `FILE` — The current file being executed
- `LINE` — The current line number in the file
- `VERSION` — The version of the Lithium interpreter
- `true`, `false`, `null` — Boolean and null values
- `print`, `println` - Functions for printing to stdout

Lithium uses an explicit import system for built-in functions and constants. To use standard IO builtins, add this at the top of your file:

```lithium
import <io>
```

This makes the following available:
- `printf(format, ...)` — Print formatted string (C-style)
- `input([prompt])` — Read a line from stdin

> **Note:** If you do not import `<io>`, these builtins will not be available in your script.

To use the built-in math constants, you can import the math module:

```lithium
import <math>
```

This makes the following available:
- `E`, `PI` — Mathematical constants

> **Note:** If you do not import `<math>`, these builtins will not be available in your script.

### Import Search Order
When you write `import <name>`, Lithium searches for modules in the following order:
1. The directory of the current file (if importing a user module)
2. `./modules/` directory
3. Built-in modules (e.g., `<io>`, `<math>`, etc.)

---

## Language Overview

### Variables

```lithium
let x = 10;
const y = 20;
```

### Expressions & Operators

- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Assignment: `=`, `+=`, `-=`, `*=`, `/=`, `%=`
- Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
- Logical: `&&`, `||`, `!`
- Increment/Decrement: `++`, `--` (prefix/postfix)

### Control Flow

```lithium
if (x > 0) {
    print("positive");
} else {
    print("not positive");
}

while (x > 0) {
    x--;
}

for (let i = 0; i < 10; i++) {
    print(i);
}
```

### Functions

```lithium
fn add(a, b) {
    return a + b;
}
print(add(2, 3));
```

- Functions are hoisted within their block.
- Closures are supported.

### Arrays

```lithium
let arr = [1, 2, 3];
print(arr[0]);
arr[1] = 42;
```

### Classes

```lithium
class Point {
    let x = 0;
    let y = 0;

    # constructor (name must match class name)
    fn Point(xIn, yIn) {
        x = xIn;
        y = yIn;
    }

    fn move(dx, dy) {
        x += dx;
        y += dy;
    }

    fn getX() {
        return x;
    }

    fn getY() {
        return y;
    }
}
```

Note: Don't use `this` keyword; class properties are accessed directly. Be careful with variable shadowing. (`this` will be added in a future version.)

---

## Comments

- Only line comments are supported:
  ```lithium
  # This is a valid comment
  ```

---

## Grammar

See [grammar.txt](grammar.txt) for the full grammar.

---

## Error Reporting

Lithium provides detailed error messages with file, line, and column information, e.g.:

```
error: cin:1:7: 'x' is not defined
│ print(x);
│       ~
│       ^
```

---

## License

MIT License

---

## Project Structure

- **`src/`** - Interpreter source code (C++)
- **`test/`** - Comprehensive test suite organized by category
  - `test/array/` - Array operation tests
  - `test/class/` - Class and object tests
  - `test/errors/` - Error handling tests
  - `test/functions/` - Function and closure tests
  - `test/expressions/` - Expression evaluation tests
  - And more...
- **`tools/`** - Development and testing tools
  - `advanced_fuzzer.py` - Advanced vulnerability testing
  - `fuzzer.cpp` - Basic C++ fuzzer
  - `test_crashes.sh` - Crash regression tests
- **`modules/`** - Standard library modules
- **`memory_test_logs/`** - Memory testing outputs

---

## Continuous Integration

This project uses GitHub Actions for automated testing and quality assurance. The CI pipeline:

- **Builds** the interpreter from source on Ubuntu
- **Runs** all 97+ native tests using the test runner
- **Executes** Python/pytest integration tests
- **Performs** memory leak detection with valgrind
- **Tests** sample programs for regression detection

See [`.github/CI.md`](.github/CI.md) for detailed CI documentation.

---
