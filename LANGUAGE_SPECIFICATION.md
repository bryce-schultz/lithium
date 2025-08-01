# 📋 Lithium Language Specification

<style>
body {
    background-color: #1e2a3a;
    color: #b0c4de;
    font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
    line-height: 1.6;
    margin: 0;
    padding: 20px;
}

h1, h2, h3, h4, h5, h6 {
    color: #87ceeb;
    border-bottom: 2px solid #2c3e50;
    padding-bottom: 8px;
}

h1 {
    background: linear-gradient(135deg, #2c3e50 0%, #34495e 100%);
    padding: 25px 20px;
    border-radius: 8px;
    text-align: center;
    font-size: 2.5em;
    margin-bottom: 30px;
    box-shadow: 0 4px 12px rgba(0,0,0,0.3);
    border: none;
}

code {
    background-color: #2c3e50;
    color: #e8f4fd;
    padding: 2px 6px;
    border-radius: 4px;
    font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
}

pre {
    background-color: #263847;
    border: 1px solid #34495e;
    border-radius: 8px;
    padding: 16px;
    overflow-x: auto;
    box-shadow: inset 0 2px 4px rgba(0,0,0,0.3);
    position: relative;
}

pre::before {
    content: "lithium";
    position: absolute;
    top: 8px;
    right: 12px;
    background-color: #34495e;
    color: #87ceeb;
    padding: 2px 8px;
    border-radius: 4px;
    font-size: 10px;
    font-weight: bold;
    text-transform: uppercase;
    letter-spacing: 0.5px;
}

/* Enhanced code highlighting */
pre code {
    color: #d4d4d4;
    font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
    font-size: 14px;
    line-height: 1.5;
}

/* Simple CSS-based syntax highlighting using text content matching */
pre code {
    /* Base text color */
    color: #d4d4d4;
}

/* Style specific patterns that commonly appear in code */
pre {
    /* Keywords - common programming keywords */
    --keyword-color: #569cd6;
    --string-color: #ce9178;
    --comment-color: #6a9955;
    --number-color: #b5cea8;
    --function-color: #dcdcaa;
    --class-color: #4ec9b0;
    --builtin-color: #569cd6;
}

/* Use CSS content selectors for basic highlighting */
pre code {
    background: linear-gradient(to right, transparent 0%);
}

/* Manual styling for common code patterns */
pre {
    font-feature-settings: "liga" 0;
    text-rendering: optimizeLegibility;
}

/* Improved readability */
pre code {
    white-space: pre;
    word-spacing: normal;
    word-break: normal;
    tab-size: 4;
}

blockquote {
    border-left: 4px solid #3498db;
    background-color: #2c3e50;
    margin: 16px 0;
    padding: 16px;
    border-radius: 0 8px 8px 0;
}

table {
    width: 100%;
    border-collapse: collapse;
    margin: 20px 0;
    background-color: #2c3e50;
    border-radius: 8px;
    overflow: hidden;
    border: 1px solid #34495e;
    box-shadow: 0 2px 8px rgba(0,0,0,0.2);
}

th, td {
    border: 1px solid #34495e;
    padding: 12px;
    text-align: left;
}

th {
    background-color: #34495e;
    color: #87ceeb;
    font-weight: bold;
    border: none !important;
    border-bottom: 1px solid #34495e !important;
}

tr:nth-child(even) {
    background-color: #2a3745;
}

.section-header {
    background: linear-gradient(135deg, #34495e 0%, #2c3e50 100%);
    padding: 15px;
    border-radius: 8px;
    margin: 20px 0;
    box-shadow: 0 2px 8px rgba(0,0,0,0.3);
}

.warning {
    background-color: #8b4513;
    border: 1px solid #cd853f;
    border-radius: 6px;
    padding: 12px;
    margin: 16px 0;
    box-shadow: 0 2px 6px rgba(0,0,0,0.2);
}

.info {
    background-color: #1e3a5f;
    border: 1px solid #3498db;
    border-radius: 6px;
    padding: 12px;
    margin: 16px 0;
    box-shadow: 0 2px 6px rgba(0,0,0,0.2);
}

/* Enhanced code highlighting - CSS-only approach */
pre code {
    color: #d4d4d4;
    font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
    font-size: 14px;
    line-height: 1.5;
    display: block;
    padding: 0;
    white-space: pre;
    text-rendering: optimizeLegibility;
    font-variant-ligatures: none;
    font-feature-settings: "liga" 0;
    letter-spacing: 0.01em;
}

/* Use text-shadow for subtle syntax highlighting effect */
pre code {
    text-shadow: 
        /* Subtle blue glow for the overall code */
        0 0 8px rgba(86, 156, 214, 0.15),
        /* Orange tint for quoted sections */
        0 0 12px rgba(206, 145, 120, 0.1),
        /* Green tint for comment-like sections */
        0 0 6px rgba(106, 153, 85, 0.1);
    
    /* Enhanced contrast and readability */
    filter: contrast(1.05) brightness(1.02);
}

/* Improved code block styling */
pre {
    background: linear-gradient(
        135deg, 
        #263847 0%, 
        #2a3a4a 50%, 
        #263847 100%
    );
    border: 1px solid #34495e;
    box-shadow: 
        inset 0 2px 4px rgba(0,0,0,0.3),
        0 1px 3px rgba(86, 156, 214, 0.1);
}

/* Add subtle color variations through creative CSS */
pre code::first-line {
    /* First line often contains important declarations */
    filter: hue-rotate(10deg) brightness(1.05);
}

/* Remove the manual syntax highlighting classes since we're not using them */
</style>

---

## 🎯 Overview

**Lithium** is a lightweight, interpreted programming language designed for clarity, expressiveness, and ease of embedding. It features a C-like syntax with modern programming constructs, dynamic typing, first-class functions, closures, classes, and a comprehensive built-in library.

### 🌟 Key Features

- **🏗️ C-like Syntax**: Familiar syntax with blocks, expressions, and statements
- **🔄 Dynamic Typing**: Numbers, strings, booleans, arrays, functions, and user-defined classes
- **⚡ First-class Functions**: Functions as values with full closure support
- **🎯 Block Scoping**: Lexical scoping with `let` and `const` declarations
- **📦 Array Literals**: Native array support with indexing and manipulation
- **🏛️ Class System**: Simple but powerful class-based object orientation
- **🔀 Control Flow**: Complete set of control structures (`if`, `while`, `for`, `foreach`, etc.)
- **🌐 Module System**: Explicit import system for built-ins and user modules
- **🔍 Interactive REPL**: Real-time code evaluation and testing
- **📝 Clear Error Messages**: Detailed error reporting with source locations

---

## 📚 Table of Contents

### Core Language
- [📖 Language Reference](#-language-reference)
  - [🔤 Lexical Structure](#-lexical-structure)
    - [Comments](#comments)
    - [Identifiers](#identifiers)
    - [Literals](#literals)
    - [String Escape Sequences](#string-escape-sequences)
  - [🔧 Operators and Expressions](#-operators-and-expressions)
    - [Operator Precedence](#operator-precedence-highest-to-lowest)
    - [Arithmetic Operators](#arithmetic-operators)
    - [Assignment Operators](#assignment-operators)
    - [Comparison Operators](#comparison-operators)
    - [Logical Operators](#logical-operators)
    - [Increment/Decrement Operators](#incrementdecrement-operators)
  - [📊 Data Types](#-data-types)
    - [Primitive Types](#primitive-types)
    - [Composite Types](#composite-types)
    - [Type Conversion](#type-conversion)
  - [🗂️ Variables and Scoping](#️-variables-and-scoping)
    - [Variable Declarations](#variable-declarations)
    - [Scoping Rules](#scoping-rules)
    - [Variable Shadowing](#variable-shadowing)

### Functions and Classes
- [🔧 Functions](#-functions)
  - [Function Declaration](#function-declaration)
  - [Function Calls](#function-calls)
  - [Closures](#closures)
  - [Function Hoisting](#function-hoisting)
- [🏛️ Classes and Objects](#️-classes-and-objects)
  - [Class Declaration](#class-declaration)
  - [Class Example](#class-example)
  - [Object Instantiation](#object-instantiation)
  - [Class Scope Rules](#class-scope-rules)
  - [Member Access](#member-access)

### Data Structures
- [📋 Arrays](#-arrays)
  - [Array Creation](#array-creation)
  - [Array Access](#array-access)
  - [Array Concatenation](#array-concatenation)
  - [Adding Arrays as Elements](#adding-arrays-as-elements-nested-arrays)
  - [Array Properties](#array-properties)
  - [String Indexing](#string-indexing)
  - [String Properties](#string-properties)

### Control Flow
- [🔄 Control Flow](#-control-flow)
  - [Conditional Statements](#conditional-statements)
  - [While Loops](#while-loops)
  - [For Loops](#for-loops)
  - [Foreach Loops](#foreach-loops)
  - [Break and Continue](#break-and-continue)

### Modules and Built-ins
- [🌐 Module System](#-module-system)
  - [Built-in Modules](#built-in-modules)
  - [Import Syntax](#import-syntax)
  - [Module Search Order](#module-search-order)
  - [Always Available](#always-available)
  - [User Modules](#user-modules)
- [🧪 Built-in Functions](#-built-in-functions)
  - [Core Functions](#core-functions)
  - [Type Function](#type-function)
  - [I/O Functions](#io-functions-require-import-io)
  - [Mathematical Constants](#mathematical-constants-require-import-math)
  - [Command Line Arguments](#command-line-arguments-require-import-args)
  - [Random Numbers](#random-numbers-require-import-random)
  - [Operating System Functions](#operating-system-functions-require-import-os)
  - [Socket Operations](#socket-operations-require-import-socket)
  - [Time Functions](#time-functions-require-import-time)
  - [Color Module](#color-module-require-import-color)

### Advanced Topics
- [✅ Assertions](#-assertions)
  - [Basic Assertions](#basic-assertions)
  - [Assertion Examples](#assertion-examples)
- [🎯 Type System Details](#-type-system-details)
  - [Object Equality](#object-equality)
  - [Truthiness](#truthiness)

### Reference Materials
- [📝 Grammar Reference](#-grammar-reference)
- [🚨 Error Handling](#-error-handling)
  - [Common Error Types](#common-error-types)
  - [Error Message Format](#error-message-format)
- [🎮 Interactive REPL](#-interactive-repl)
  - [Starting the REPL](#starting-the-repl)
  - [REPL Features](#repl-features)
  - [REPL Examples](#repl-examples)
- [🔧 Best Practices](#-best-practices)
  - [Code Style](#code-style)
  - [Array Operations](#array-operations)
  - [Random Number Generation](#random-number-generation)
  - [Error Handling](#error-handling-1)
  - [Class Design](#class-design)
- [🏁 Conclusion](#-conclusion)

---

## 📖 Language Reference

<div class="section-header">

### 🔤 Lexical Structure

</div>

#### **Comments**
Only line comments are supported using the `#` character:

```lithium
# this is a comment
let x = 10;  # this is also a comment
```

#### **Identifiers**
Identifiers follow the pattern: `[A-Za-z_][A-Za-z0-9_]*`

```lithium
let variable_name = 10;
let _private = "hidden";
let MyClass = "class name";
```

#### **Literals**

| **Type** | **Syntax** | **Examples** |
|----------|------------|--------------|
| **Number** | `[0-9]+\.?[0-9]*` | `42`, `3.14`, `0`, `123.456` |
| **String** | `"..."` | `"hello"`, `"Hello, World!"`, `""` |
| **Boolean** | `true` \| `false` | `true`, `false` |
| **Null** | `null` | `null` |
| **Array** | `[expr, ...]` | `[1, 2, 3]`, `["a", "b"]`, `[]` |

#### **String Escape Sequences**

| **Sequence** | **Meaning** |
|-------------|-------------|
| `\"` | Double quote |
| `\\` | Backslash |
| `\n` | Newline |
| `\t` | Tab |

```lithium
println("He said, \"Hello!\"");
println("C:\\Program Files\\");
println("Line1\nLine2");
println("Tab\tSeparated");
```

---

<div class="section-header">

### 🔧 Operators and Expressions

</div>

#### **Operator Precedence** (Highest to Lowest)

| **Level** | **Operators** | **Associativity** | **Description** |
|-----------|---------------|------------------|-----------------|
| 1 | `()` `[]` `.` | Left-to-right | Function calls, array access, member access |
| 2 | `++` `--` (postfix) | Left-to-right | Postfix increment/decrement |
| 3 | `++` `--` (prefix) `+` `-` `!` | Right-to-left | Prefix increment/decrement, unary plus/minus, logical NOT |
| 4 | `*` `/` `%` | Left-to-right | Multiplication, division, modulo |
| 5 | `+` `-` | Left-to-right | Addition, subtraction |
| 6 | `<` `<=` `>` `>=` | Left-to-right | Relational comparison |
| 7 | `==` `!=` | Left-to-right | Equality comparison |
| 8 | `&&` | Left-to-right | Logical AND |
| 9 | `\|\|` | Left-to-right | Logical OR |
| 10 | `=` `+=` `-=` `*=` `/=` `%=` | Right-to-left | Assignment operators |
| 11 | `,` | Left-to-right | Comma operator |

#### **Arithmetic Operators**

```lithium
let a = 10 + 5;     # addition: 15
let b = 10 - 5;     # subtraction: 5
let c = 10 * 5;     # multiplication: 50
let d = 10 / 5;     # division: 2
let e = 10 % 3;     # modulo: 1
```

#### **Assignment Operators**

```lithium
let x = 10;
x += 5;    # x = x + 5  -> 15
x -= 3;    # x = x - 3  -> 12
x *= 2;    # x = x * 2  -> 24
x /= 4;    # x = x / 4  -> 6
x %= 5;    # x = x % 5  -> 1

# special behavior for arrays
let arr = [1, 2];
arr += 3;       # appends element: [1, 2, 3]
arr += [4, 5];  # appends array: [1, 2, 3, 4, 5]
```

<div class="info">
<strong>📘 Array += Operator:</strong> For arrays, the <code>+=</code> operator performs concatenation rather than addition. It appends elements to the end of the array, automatically handling array growth.
</div>

#### **Comparison Operators**

```lithium
println(5 == 5);    # true
println(5 != 3);    # true
println(5 > 3);     # true
println(5 >= 5);    # true
println(3 < 5);     # true
println(3 <= 3);    # true
```

#### **Logical Operators**

```lithium
println(true && false);   # false (logical AND)
println(true || false);   # true (logical OR)
println(!true);           # false (logical NOT)
```

<div class="info">
<strong>📘 Short-Circuit Evaluation:</strong> Logical operators use short-circuit evaluation. In <code>a && b</code>, if <code>a</code> is false, <code>b</code> is not evaluated. In <code>a || b</code>, if <code>a</code> is true, <code>b</code> is not evaluated.
</div>

#### **Increment/Decrement Operators**

```lithium
let x = 5;
println(x++);  # 5 (postfix: returns old value, then increments)
println(++x);  # 7 (prefix: increments first, then returns new value)
println(x--);  # 7 (postfix: returns old value, then decrements)
println(--x);  # 5 (prefix: decrements first, then returns new value)
```

---

<div class="section-header">

### 📊 Data Types

</div>

#### **Primitive Types**

| **Type** | **Description** | **Examples** |
|----------|-----------------|--------------|
| **Number** | 64-bit floating point | `42`, `3.14`, `-10`, `0` |
| **String** | UTF-8 encoded text | `"hello"`, `"world"`, `""` |
| **Boolean** | Logical values | `true`, `false` |
| **Null** | Absence of value | `null` |

#### **Composite Types**

| **Type** | **Description** | **Examples** |
|----------|-----------------|--------------|
| **Array** | Ordered collection of values | `[1, 2, 3]`, `["a", "b"]` |
| **Function** | Callable code blocks | `fn add(a, b) { return a + b; }` |
| **Class** | User-defined types | `class Point { let x = 0; }` |
| **Object** | Instances of classes | `let p = Point();` |

#### **Type Conversion**

Lithium performs automatic type conversion in certain contexts:

```lithium
# string concatenation
println("Number: " + 42);        # "Number: 42"
println("Value: " + true);       # "Value: true"

# boolean context (truthiness)
if (0) { }          # false
if (1) { }          # true
if ("") { }         # false
if ("hello") { }    # true
if (null) { }       # false
if ([]) { }         # false (empty array is falsy)
```

---

<div class="section-header">

### 🗂️ Variables and Scoping

</div>

#### **Variable Declarations**

```lithium
# mutable variables (must be initialized)
let name = "John";
let age = 30;

# constants (must be initialized)
const PI = 3.14159;
```

<div class="warning">
<strong>⚠️ Initialization Required:</strong> All variable declarations must include an initial value. Declarations like <code>let x;</code> will result in a syntax error expecting an <code>=</code> sign. This ensures no uninitialized variables exist in Lithium code.
</div>

```lithium
# ✅ valid declarations
let x = 10;
let name = "hello";
let flag = true;
const MAX = 100;

# ❌ invalid - syntax error
let y;              # error: expected '='
const LIMIT;        # error: expected '='
```

<div class="warning">
<strong>⚠️ Constants:</strong> Once declared, constants cannot be reassigned. Attempting to modify a constant will result in a runtime error.
</div>

<div class="warning">
<strong>⚠️ Reserved Keywords:</strong> The keywords <code>true</code>, <code>false</code>, and <code>null</code> are reserved and cannot be used as variable names. Attempting to declare variables with these names will result in a syntax error.
</div>

```lithium
# ❌ invalid - reserved keyword usage
let true = false;   # error: expected 'identifier' but found 'keyword (true)'
let false = true;   # error: expected 'identifier' but found 'keyword (false)'
let null = 42;      # error: expected 'identifier' but found 'keyword (null)'

# ✅ valid - using reserved keywords as literals
let x = true;       # correct: true as literal value
let y = false;      # correct: false as literal value
let z = null;       # correct: null as literal value
```

#### **Scoping Rules**

Lithium uses **lexical (block) scoping**:

```lithium
let global = "I'm global";

if (true) {
    let block = "I'm in a block";
    println(global);  # accessible: "I'm global"
    println(block);   # accessible: "I'm in a block"
}

# println(block);  # error: 'block' is not defined
```

#### **Variable Shadowing**

```lithium
let x = "outer";
{
    let x = "inner";
    println(x);       # "inner"
}
println(x);           # "outer"
```

---

<div class="section-header">

### 🔧 Functions

</div>

#### **Function Declaration**

```lithium
fn functionName(param1, param2) {
    # function body
    return result;
}

# functions without parameters
fn greet() {
    println("Hello, World!");
}

# functions without explicit return
fn doSomething() {
    println("This function has no return value");
    # cannot assign result: let x = doSomething(); would be a syntax error
}
```

#### **Function Calls**

```lithium
let result = functionName(arg1, arg2);
greet();
doSomething();
```

#### **Closures**

Functions can capture variables from their enclosing scope:

```lithium
fn makeCounter(start) {
    let count = start;
    
    fn increment() {
        return count++;
    }
    
    return increment;
}

let counter1 = makeCounter(0);
let counter2 = makeCounter(10);

println(counter1());  # 0
println(counter2());  # 10
println(counter1());  # 1
println(counter2());  # 11
```

#### **Function Hoisting**

Functions are hoisted within their block scope:

```lithium
println(hoisted());  # works! Prints "I'm hoisted"

fn hoisted() {
    return "I'm hoisted";
}
```

<div class="info">
<strong>📘 First-Class Functions:</strong> Functions are values in Lithium. They can be assigned to variables, passed as arguments, and returned from other functions.
</div>

<div class="warning">
<strong>⚠️ Return Statement Usage:</strong> The <code>return</code> statement can only be used inside functions. Using <code>return</code> outside of a function will result in a compile-time error.
</div>

<div class="warning">
<strong>⚠️ Function Return Values:</strong> Functions without an explicit <code>return</code> statement do not return any value. Attempting to assign the result of such functions to variables will result in a syntax error, as Lithium does not allow undefined values.
</div>

```lithium
fn withReturn() {
    return 42;
}

fn withoutReturn() {
    println("No return value");
}

# ✅ valid - function has explicit return
let result = withReturn();  # result = 42

# ❌ invalid - function has no return value
let invalid = withoutReturn();  # syntax error: cannot assign undefined value

# ✅ valid - call function without assignment
withoutReturn();  # just calls the function
```

---

<div class="section-header">

### 🏛️ Classes and Objects

</div>

#### **Class Declaration**

```lithium
class ClassName {
    # class members (variables and methods)
    let memberVariable = defaultValue;
    
    # constructor (must match class name)
    fn ClassName(param1, param2) {
        memberVariable = param1;
        # initialize other members
    }
    
    # methods
    fn methodName() {
        return memberVariable;
    }
}
```

#### **Class Example**

```lithium
class Point {
    let x = 0;
    let y = 0;
    
    fn Point(xIn, yIn) {
        x = xIn;
        y = yIn;
    }
    
    fn move(dx, dy) {
        x += dx;
        y += dy;
    }
    
    fn toString() {
        return "Point(" + x + ", " + y + ")";
    }
    
    fn getX() {
        return x;
    }
    
    fn getY() {
        return y;
    }
}
```

#### **Object Instantiation**

```lithium
# create instances
let p1 = Point(10, 20);
let p2 = Point(0, 0);

# access members
println(p1.x);         # 10
println(p1.getX());    # 10

# call methods
p1.move(5, -3);
println(p1.toString()); # "Point(15, 17)"
```

#### **Class Scope Rules**

<div class="warning">
<strong>⚠️ Global Scope Only:</strong> Classes can only be declared in global scope, not inside functions or blocks.
</div>

```lithium
# ✅ valid - global scope
class MyClass { }

fn someFunction() {
    # ❌ invalid - classes cannot be declared in functions
    class LocalClass { }
}

if (condition) {
    # ❌ invalid - classes cannot be declared in blocks
    class BlockClass { }
}
```

#### **Member Access**

<div class="info">
<strong>📘 No "this" Keyword:</strong> Lithium does not use a <code>this</code> keyword. Class members are accessed directly by name within methods.
</div>

---

<div class="section-header">

### 📋 Arrays

</div>

#### **Array Creation**

```lithium
let empty = [];
let numbers = [1, 2, 3, 4, 5];
let mixed = ["hello", 42, true, null];
let nested = [[1, 2], [3, 4]];
```

#### **Array Access**

```lithium
let arr = [10, 20, 30];
println(arr[0]);  # 10
println(arr[1]);  # 20
println(arr[2]);  # 30

# Modify elements
arr[0] = 100;
println(arr[0]);  # 100
```

<div class="warning">
<strong>⚠️ Array Bounds Checking:</strong> Lithium performs strict bounds checking on array access. Accessing an index that doesn't exist (including <code>array[len(array)]</code>) will cause a runtime error. You cannot extend arrays by assigning to out-of-bounds indices - use the <code>+=</code> operator instead.
</div>

```lithium
let arr = [1, 2, 3];

# ❌ this will cause a runtime error
arr[3] = 4;           # error: array index out of bounds
arr[len(arr)] = 4;    # error: array index out of bounds

# ✅ use += to extend arrays safely
arr += 4;             # correct: [1, 2, 3, 4]
```

#### **Array Concatenation**

Arrays support concatenation using the `+=` operator for appending elements:

```lithium
let numbers = [1, 2, 3];

# append single element
numbers += 4;
println(numbers);  # [1, 2, 3, 4]

# append multiple elements from another array
numbers += [5, 6];
println(numbers);  # [1, 2, 3, 4, 5, 6]

# append mixed types
let mixed = ["hello"];
mixed += 42;
mixed += [true, null];
println(mixed);  # ["hello", 42, true, null]
```

<div class="info">
<strong>📘 Array Growth:</strong> The <code>+=</code> operator automatically handles array growth without needing to know the current length. This is the preferred way to append elements to arrays in Lithium.
</div>

#### **Adding Arrays as Elements (Nested Arrays)**

When working with nested arrays or records, you need to distinguish between concatenating array contents vs. adding entire arrays as single elements:

```lithium
let records = [];
let contact = ["John", "john@email.com", "555-1234"];

# ❌ wrong: this concatenates individual elements
records += contact;
println(records);  # ["John", "john@email.com", "555-1234"] (flattened)

# ✅ correct: this adds the array as a single element
records = [];  # reset
records += [contact];
println(records);  # [["John", "john@email.com", "555-1234"]] (nested)

# ✅ building a collection of records
let moreRecords = [];
moreRecords += [["Alice", "alice@email.com", "555-5678"]];
moreRecords += [["Bob", "bob@email.com", "555-9012"]];
println(moreRecords);  # [["Alice", "alice@email.com", "555-5678"], ["Bob", "bob@email.com", "555-9012"]]
```

<div class="warning">
<strong>⚠️ Common Mistake:</strong> When you want to add an array as a single element to another array, you must wrap it in square brackets: <code>array += [element]</code>. Using <code>array += element</code> when <code>element</code> is an array will concatenate its contents instead.
</div>

#### **Array Properties**

```lithium
let numbers = [1, 2, 3, 4, 5];
println(numbers.length());    # 5

let empty = [];
println(empty.length());      # 0

let nested = [[1, 2], [3, 4]];
println(nested.length());     # 2 (number of top-level elements)

# alternative: built-in len() function
println(len(numbers));        # 5 (same result)
```

<div class="info">
<strong>📘 Array Length:</strong> Arrays support both <code>array.length()</code> method and the built-in <code>len(array)</code> function. Both return the same result - use whichever style you prefer.
</div>

#### **Array Methods**

```lithium
let fruits = ["apple", "banana", "cherry"];

# join() - Join array elements into a string
let result1 = fruits.join();        # "applebananacherry" (no separator)
let result2 = fruits.join(", ");    # "apple, banana, cherry" (with separator)
let result3 = fruits.join(" | ");   # "apple | banana | cherry" (custom separator)

# Works with mixed types
let mixed = ["Hello", 42, true, null];
let message = mixed.join(" ");      # "Hello 42 true null"

# Empty arrays
let empty = [];
println(empty.join(","));           # "" (empty string)
```

**Available Methods:**
- `array.join()` - Join elements with no separator (empty string)
- `array.join(separator)` - Join elements with the specified separator string
- `array.length()` - Get the number of elements in the array

#### **String Indexing**

Strings support array-like indexing:

```lithium
let str = "hello";
println(str[0]);  # "h"
println(str[1]);  # "e"
println(str[4]);  # "o"
```

#### **String Properties**

```lithium
println("hello".length());     # 5
println("".length());          # 0

let text = "Hello, World!";
println(text.length());        # 13

# string splitting
let csv = "apple,banana,cherry";
let fruits = csv.split(",");
println(fruits);               # ["apple", "banana", "cherry"]

let sentence = "Hello World Example";
let words = sentence.split(" ");
println(words);                # ["Hello", "World", "Example"]

# split with single character
let path = "home/user/documents";
let parts = path.split("/");
println(parts);                # ["home", "user", "documents"]

# string case conversion
let text = "Hello World";
println(text.lower());         # "hello world"
println(text.upper());         # "HELLO WORLD"

let mixed = "CamelCase";
println(mixed.lower());        # "camelcase"
println(mixed.upper());        # "CAMELCASE"

# character codes
let single = "A";
println(single.code());        # 65 (single number for single character)

let word = "Hi";
println(word.code());          # [72, 105] (array for multiple characters)

let greeting = "Hello!";
println(greeting.code());      # [72, 101, 108, 108, 111, 33]

let empty = "";
println(empty.code());         # [] (empty array for empty string)
```

---

<div class="section-header">

### 🔄 Control Flow

</div>

#### **Conditional Statements**

```lithium
# if statement
if (condition) {
    # code block
}

# if-else statement
if (condition) {
    # code if true
} else {
    # code if false
}

# nested conditions
if (x > 0) {
    println("positive");
} else if (x < 0) {
    println("negative");
} else {
    println("zero");
}
```

#### **While Loops**

```lithium
let i = 0;
while (i < 5) {
    println(i);
    i++;
}
```

#### **For Loops**

```lithium
for (let i = 0; i < 10; i++) {
    println(i);
}

# for loop parts: initialization; condition; increment
for (let i = 10; i > 0; i--) {
    println("Countdown: " + i);
}
```

#### **Foreach Loops**

```lithium
# array iteration - single value variant
let numbers = [1, 2, 3, 4, 5];
foreach (num : numbers) {
    println(num);
}

# object iteration - key-value variant  
class Person {
    let name = "John";
    let age = 30;
}
let p = Person();
foreach (key, value : p) {
    println(key + ": " + value);
}

# works with mixed arrays
let mixed = ["hello", 42, true, null];
foreach (item : mixed) {
    println("Item: " + item);
}
```

<div class="warning">
<strong>⚠️ Foreach Variants:</strong>
<ul>
<li><strong>Array iteration:</strong> <code>foreach (value : array)</code> - single value before the colon</li>
<li><strong>Object iteration:</strong> <code>foreach (key, value : object)</code> - key and value before the colon</li>
<li>Using the wrong variant for the data type will result in a runtime error</li>
</ul>
</div>

```lithium
# ✅ valid usage
let arr = [1, 2, 3];
foreach (item : arr) {           # correct: single value for array
    println(item);
}

class MyClass { let x = 10; }
let obj = MyClass();
foreach (key, value : obj) {     # correct: key-value for object
    println(key + " = " + value);
}

# ❌ invalid usage  
foreach (key, value : arr) {     # error: key-value syntax on array
    println(key + ": " + value);
}

foreach (item : obj) {           # error: single value syntax on object
    println(item);
}
```

#### **Break and Continue**

```lithium
for (let i = 0; i < 10; i++) {
    if (i == 3) {
        continue;  # skip iteration when i is 3
    }
    if (i == 7) {
        break;     # exit loop when i is 7
    }
    println(i);
}
# output: 0, 1, 2, 4, 5, 6
```

<div class="warning">
<strong>⚠️ Loop Control Statements:</strong>
<ul>
<li><code>break</code> and <code>continue</code> can only be used inside loops (<code>for</code>, <code>while</code>, <code>foreach</code>)</li>
<li>Using these statements outside of loops will result in a compile-time error</li>
</ul>
</div>

```lithium
# ✅ valid usage
for (let i = 0; i < 10; i++) {
    if (i == 5) {
        break;     # valid: inside a loop
    }
    if (i % 2 == 0) {
        continue;  # valid: inside a loop
    }
}

# ❌ invalid usage
break;                 # error: break outside of loop
continue;              # error: continue outside of loop

if (true) {
    break;             # error: break not in a loop
}
```

#### **Delete Statement**

The `delete` statement removes a variable from the current scope. Once deleted, the variable becomes undefined and cannot be accessed.

```lithium
let x = 42;
let y = 100;
println(x);           # output: 42
println(y);           # output: 100

delete x;             # removes variable x from scope
println(y);           # output: 100
println(x);           # error: 'x' is not defined
```

<div class="warning">
<strong>⚠️ Delete Statement Restrictions:</strong>
<ul>
<li>Cannot delete <code>const</code> variables</li>
<li>Cannot delete undefined variables</li>
<li>Only works on variables in the current scope</li>
</ul>
</div>

```lithium
# ✅ valid usage
let variable = 42;
delete variable;      # valid: deletes a mutable variable

# ❌ invalid usage
const constant = 42;
delete constant;      # error: cannot delete constant variable

delete undefined_var; # error: variable 'undefined_var' is not defined
```

---

<div class="section-header">

### 🌐 Module System

</div>

#### **Built-in Modules**

Lithium provides several built-in modules that must be explicitly imported:

| **Module** | **Description** | **Provides** |
|------------|-----------------|--------------|
| `<io>` | Input/Output operations | `printf()`, `input()` |
| `<math>` | Mathematical constants | `PI`, `E` |
| `<args>` | Command line arguments | `args` (array of arguments) |
| `<random>` | Random number generation | `random()` - returns 64-bit integer |
| `<os>` | Operating system functions | `open()`, `close()`, `read()`, `write()`, `shell()` |
| `<socket>` | Network socket operations | `socket()`, `listen()`, `accept()`, `connect()`, `send()`, `receive()` |
| `<time>` | Time and sleep functions | `time()`, `sleep()` |
| `<color>` | Terminal color codes | `Color` object with ANSI escape sequences |

#### **Import Syntax**

```lithium
# import modules
import <io>
import <math>

# import modules using path notation (. becomes /)
import <io.getfile>  # imports from modules/io/getfile.li
import <utils.string.helpers>  # imports from modules/utils/string/helpers.li
```

#### **Module Search Order**

When you use `import <moduleName>`, Lithium searches for modules in this specific order:

**Phase 1: File System Search**
1. **Current file directory** - Same directory as the importing file
2. **`./modules/` directory** - Relative to current working directory
3. **`$HOME/modules/` directory** - User's home modules folder (if HOME environment variable exists)

**Phase 2: Built-in Modules (if no file found)**
4. **Built-in modules** - Hardcoded modules: `args`, `io`, `math`, `random`, `os`, `socket`, `time`, `color`

**Path Resolution:**
- Dot notation (`.`) becomes forward slash (`/`) in file paths
- All module files must have `.li` extension
- If neither a file nor built-in module is found, an import error occurs

**Examples:**
```lithium
import <io>                    # built-in module (phase 2)
import <mymodule>              # searches for mymodule.li in phase 1 locations
import <utils.math>            # searches for utils/math.li in phase 1 locations
import <data.parsers.json>     # searches for data/parsers/json.li in phase 1 locations
```

**Search Locations for `import <utils.math>`:**
1. `./utils/math.li` (current directory)
2. `./modules/utils/math.li` (modules directory)
3. `$HOME/modules/utils/math.li` (user modules, if HOME set)
4. Built-in check (not found, so import error)

#### **Always Available**

Some constants and functions are always available without imports:

```lithium
# built-in constants
println(VERSION);    # lithium version ("0.3")
println(FILE);       # current file name (string)
println(LINE);       # current line number (number)
println(true);       # boolean true
println(false);      # boolean false
println(null);       # null value

# built-in functions (no import required)
print("Hello");      # print values without newline
println("World");    # print values with newline
type(42);            # get type of value → "number"
exit(0);             # exit program with code
len([1, 2, 3]);      # get length of array → 3
number("42");        # convert to number → 42
dumpenv();           # dump environment variables (debugging)
```

#### **User Modules**

Create your own modules by placing `.li` files in the `modules/` directory:

```lithium
# modules/mymodule.li
fn utility() {
    return "This is a utility function";
}

# modules/math/advanced.li
fn complexCalculation() {
    return "Advanced math result";
}

# main.li
import <mymodule>
println(utility());  # "This is a utility function"

import <math.advanced>  # loads modules/math/advanced.li
println(complexCalculation());  # "Advanced math result"
```

---

<div class="section-header">

### 🧪 Built-in Functions

</div>

#### **Core Functions**

| **Function** | **Description** | **Example** |
|-------------|-----------------|-------------|
| `print(...)` | Print values without newline | `print("Hello", " ", "World")` |
| `println(...)` | Print values with newline | `println("Hello World")` |
| `type(value)` | Get type of value | `type(42)` → `"number"` |
| `exit(code)` | Exit program with code | `exit(0)` |
| `len(array)` | Get length of array | `len([1, 2, 3])` → `3` |
| `number(value)` | Convert value to number | `number("42")` → `42` |

#### **Type Function**

The `type()` function returns the type of any value:

```lithium
println(type("hello"));     # "string"
println(type(42));          # "number"
println(type(true));        # "boolean"
println(type(null));        # "null"
println(type([1, 2, 3]));   # "array"
println(type(type));        # "builtin"

# for object instances, returns class name
class Person { }
let p = Person();
println(type(p));           # "Person"
```

#### **I/O Functions** (require `import <io>`)

```lithium
import <io>

printf("Hello %, you are % years old\n", "John", 25);
let name = input("Enter your name: ");
println("Hello, " + name);
```

#### **Mathematical Constants** (require `import <math>`)

```lithium
import <math>

println(PI);    # 3.14159...
println(E);     # 2.71828...
```

#### **Command Line Arguments** (require `import <args>`)

```lithium
import <args>

println("Program arguments:");
foreach (arg : args) {
    println("  " + arg);
}
```

#### **Random Numbers** (require `import <random>`)

```lithium
import <random>

# random() returns a 64-bit random integer
let randomInt = random();
println("Random 64-bit integer: " + randomInt);

# to get a random number in a specific range (e.g., 1-100):
let min = 1;
let max = 100;
let range = max - min + 1;
let randomInRange = min + (random() % range);
println("Random number 1-100: " + randomInRange);

# for floating point random values between 0 and 1:
let randomFloat = (random() % 1000000) / 1000000.0;
println("Random float 0-1: " + randomFloat);
```

#### **Operating System Functions** (require `import <os>`)

```lithium
import <os>

# File operations
let fd = open("file.txt", "r");
let content = read(fd, 100);
close(fd);

# Directory operations
let currentDir = getcwd();
println("Current directory: " + currentDir);

chdir("/tmp");
println("Changed to: " + getcwd());

# List directory contents
let files = listdir(".");
println("Files in current directory:");
foreach (file : files) {
    println("  " + file);
}

# Process information
let processId = getpid();
println("Process ID: " + processId);

# Shell commands
let result = shell("ls -la");
println("Command output: " + result);
```

**Available Functions:**
- `open(filename, mode)` - Open a file for reading/writing
- `read(fd, size)` - Read data from a file descriptor
- `write(fd, data)` - Write data to a file descriptor
- `close(fd)` - Close a file descriptor
- `getcwd()` - Get current working directory
- `chdir(path)` - Change current working directory
- `getpid()` - Get process ID
- `shell(command)` - Execute a shell command and return output
- `listdir(path)` - List directory contents

#### **Socket Operations** (require `import <socket>`)

```lithium
import <socket>

let sock = socket("tcp");
connect(sock, "localhost", 8080);
send(sock, "Hello Server");
let response = receive(sock, 1024);
close(sock);
```

#### **Time Functions** (require `import <time>`)

```lithium
import <time>

let currentTime = time();
println("Current timestamp: " + currentTime);

sleep(1000);  # Sleep for 1000 milliseconds
```

#### **Color Module** (require `import <color>`)

The color module provides ANSI escape sequences for terminal text coloring:

```lithium
import <color>

# Available colors
println(Color.red + "This text is red" + Color.reset);
println(Color.green + "This text is green" + Color.reset);
println(Color.yellow + "This text is yellow" + Color.reset);
println(Color.blue + "This text is blue" + Color.reset);
println(Color.magenta + "This text is magenta" + Color.reset);
println(Color.cyan + "This text is cyan" + Color.reset);
println(Color.white + "This text is white" + Color.reset);

# Example: Colored output
println(Color.green + "SUCCESS: " + Color.reset + "Operation completed");
println(Color.red + "ERROR: " + Color.reset + "Something went wrong");
println(Color.yellow + "WARNING: " + Color.reset + "Please check this");

# Multiple colors in one line
println(Color.blue + "Hello " + Color.red + "World" + Color.reset);
```

**Available Color Properties:**
- `Color.red` - Red text
- `Color.green` - Green text  
- `Color.yellow` - Yellow text
- `Color.blue` - Blue text
- `Color.magenta` - Magenta text
- `Color.cyan` - Cyan text
- `Color.white` - White text
- `Color.reset` - Reset to default color

---

<div class="section-header">

### ✅ Assertions

</div>

#### **Basic Assertions**

```lithium
assert expression;
assert expression, "Custom error message";
```

#### **Assertion Examples**

```lithium
# basic assertions
assert true;
assert 1 + 1 == 2;
assert "hello" == "hello";

# assertions with custom messages
assert x > 0, "x must be positive";
assert len(array) > 0, "Array cannot be empty";

# assertions that will fail
assert false;                    # assertionError
assert 1 == 2, "Math is broken"; # assertionError: Math is broken
```

<div class="info">
<strong>📘 Assert Behavior:</strong> If an assertion fails, the program terminates with an error message showing the assertion that failed and the optional custom message.
</div>

---

<div class="section-header">

### 🎯 Type System Details

</div>

#### **Object Equality**

```lithium
# null equality works with any type
let obj = MyClass();
if (obj == null) {
    println("Object is null");
}
if (null == obj) {
    println("Object is null (reverse)");
}

# string/Object concatenation
class Person {
    let name = "John";
    fn toString() {
        return "Person(" + name + ")";
    }
}

let p = Person();
println("Hello " + p);  # calls toString() method
```

#### **Truthiness**

Values are considered truthy or falsy in boolean contexts:

| **Value** | **Truthiness** |
|-----------|----------------|
| `null` | `false` |
| `0` | `false` |
| `""` (empty string) | `false` |
| `[]` (empty array) | `false` |
| `{}` (empty object) | `false` |
| `false` | `false` |
| Everything else | `true` |

```lithium
if (null) { }      # false
if (0) { }         # false
if ("") { }        # false
if ([]) { }        # false (empty array is falsy)
if (false) { }     # false

# empty objects are also falsy
class Empty { }
let emptyObj = Empty();
if (emptyObj) { }  # false (empty object is falsy)

if (1) { }         # true
if ("hello") { }   # true
if ([1, 2]) { }    # true (non-empty array is truthy)
if (true) { }      # true
```

---

<div class="section-header">

### 📝 Grammar Reference

</div>

<details>
<summary><strong>🔍 Complete BNF Grammar (Click to expand)</strong></summary>

```bnf
module         → stmts

stmts          → stmt stmts | ε

stmt           → block | forStmt | whileStmt | ifStmt | funcDecl 
               | classDecl | returnStmt | breakStmt | continueStmt
               | assertStmt | exprStmt | importStmt

forStmt        → FOR ( exprStmt exprStmt expr ) stmt
foreachStmt    → FOREACH ( IDENT : expr ) stmt
               | FOREACH ( IDENT , IDENT : expr ) stmt
whileStmt      → WHILE ( expr ) stmt
ifStmt         → IF ( expr ) stmt | IF ( expr ) stmt ELSE stmt
block          → { stmts } | { }

funcDecl       → FN IDENT ( paramList ) stmt | FN IDENT ( ) stmt
paramList      → IDENT paramList' | ε
paramList'     → , IDENT paramList' | ε

classDecl      → CLASS IDENT { classBody }
classBody      → varDecl | funcDecl

returnStmt     → RETURN expr ; | RETURN ;
breakStmt      → BREAK ;
continueStmt   → CONTINUE ;
assertStmt     → ASSERT expr ; | ASSERT expr , expr ;

exprStmt       → expr ; | varDecl | ;
importStmt     → IMPORT < moduleName > ;
moduleName     → IDENT . IDENT | IDENT

varDecl        → letStmt | constStmt
letStmt        → LET IDENT = expr ;
constStmt      → CONST IDENT = expr ;

expr           → assign expr'
expr'          → , assign expr' | ε

assign         → or assign'
assign'        → = or assign' | PLUS_EQUAL or assign' | MINUS_EQUAL or assign'
               | MUL_EQUAL or assign' | DIV_EQUAL or assign' | MOD_EQUAL or assign' | ε

or             → and or'
or'            → OR and or' | ε

and            → equality and'
and'           → AND equality and' | ε

equality       → relation equality'
equality'      → EQ relation equality' | NE relation equality' | ε

relation       → addit relation'
relation'      → > addit relation' | < addit relation'
               | GE addit relation' | LE addit relation' | ε

addit          → mult addit'
addit'         → + mult addit' | - mult addit' | ε

mult           → unary mult'
mult'          → * unary mult' | / unary mult' | % unary mult' | ε

unary          → INC unary | DEC unary | + unary | - unary | ! unary | post

post           → primary post''
post'          → [ expr ] | ( argList ) | . IDENT | INC | DEC
post''         → post' post'' | ε

primary        → ( expr ) | [ argList ] | IDENT | NUMBER | STRING

argList        → assign argList'
argList'       → , assign argList' | ε
```

</details>

---

<div class="section-header">

### 🚨 Error Handling

</div>

#### **Common Error Types**

| **Error Type** | **Description** | **Example** |
|----------------|-----------------|-------------|
| **Syntax Error** | Invalid syntax | Missing semicolon, unmatched brackets |
| **Reference Error** | Undefined variable | Using undeclared variable |
| **Type Error** | Invalid operation for type | Calling non-function, array access on non-array |
| **Assignment Error** | Invalid assignment | Assigning to constant, non-lvalue |
| **Assertion Error** | Failed assertion | `assert false` |

#### **Error Message Format**

```
error: filename:line:column: error message
│ source code line
│ ~~~~~~
│ ^
```

Example:
```
error: main.li:5:7: 'x' is not defined
│ print(x);
│       ~
│       ^
```

---

<div class="section-header">

### 🎮 Interactive REPL

</div>

#### **Starting the REPL**

```bash
$ li
lithium 0.3
type 'exit' to quit.
>
```

#### **REPL Features**

- **Immediate Evaluation**: Enter expressions and see results instantly
- **Multi-line Support**: Enter complex statements across multiple lines
- **Variable Persistence**: Variables remain in scope across REPL sessions
- **Error Recovery**: Syntax errors don't crash the REPL

#### **REPL Examples**

```lithium
> let x = 10;
> x + 5
15
> fn double(n) { return n * 2; }
> double(x)
20
> class Point { let x = 0; let y = 0; }
> let p = Point();
> p.x = 42;
> p.x
42
> exit
```

---

<div class="section-header">

### 🔧 Best Practices

</div>

#### **Code Style**

```lithium
# ✅ good: clear function names
fn calculateArea(width, height) {
    return width * height;
}

# ✅ good: meaningful variable names
let userAge = 25;
let isLoggedIn = true;
```
