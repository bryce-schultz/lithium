# Lithium Programming Language

Lithium is a lightweight, interpreted programming language designed for clarity, expressiveness, and ease of embedding. It features a C-like syntax, dynamic typing, first-class functions, closures, arrays, and a small but powerful set of built-in functions. Lithium is ideal for scripting, teaching, and rapid prototyping.

---

## Features

- **C-like syntax** with blocks, expressions, and statements
- **Dynamic typing**: numbers, strings, booleans, arrays, and user-defined classes
- **First-class functions** and closures
- **Block scoping** with `let` and `const`
- **Array literals** and indexing
- **Simple class system** (WIP)
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
lithium v0.1. type 'exit' to quit.
```

Type code and see results immediately.

---

## Imports and Builtins

There are a few always-available built-in modules in Lithium, which do not require imports.
These include:
- `FILE` — The current file being executed
- `LINE` — The current line number in the file
- `VERSION` — The version of the Lithium interpreter
- `true`, `false`, `null` — Boolean and null values

Lithium uses an explicit import system for built-in functions and constants. To use standard IO builtins, add this at the top of your file:

```lithium
import <io>
```

This makes the following available:
- `print(...)` — Print values to stdout (space-separated, no newline)
- `println(...)` — Print values to stdout (space-separated, with newline)
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
3. `./lib/` directory
4. Built-in modules (e.g., `<io>`, `<math>`, etc.)

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
print(arr[0]); // 1
arr[1] = 42;
```

### Classes (WIP)

```lithium
class Point {
    let x = 0;
    let y = 0;

    pub fn move(dx, dy) {
        x += dx;
        y += dy;
    }

    pub fn getX() {
        return x;
    }

    pub fn getY() {
        return y;
    }
}
```

---

## Comments

- Only line comments are supported:
  ```lithium
  # This is a comment
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
