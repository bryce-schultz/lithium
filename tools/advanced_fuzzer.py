#!/usr/bin/env python3
"""
Advanced Lithium Fuzzer - Focuses on specific vulnerability patterns
"""

import subprocess
import random
import string
import tempfile
import os
import signal
import time
from typing import List, Dict, Tuple

class LithiumAdvancedFuzzer:
    def __init__(self):
        self.interpreter_path = "../src/li"
        self.crash_count = 0
        self.timeout_count = 0
        self.interesting_inputs = []

    def generate_parser_stress_test(self) -> str:
        """Generate inputs that stress the parser"""
        patterns = [
            # Deeply nested expressions
            "(" * 1000 + "1" + ")" * 1000,

            # Very long identifiers
            "let " + "a" * 10000 + " = 1;",

            # Unicode edge cases
            "let café = 1; let 函数 = 2; let 🚀 = 3;",

            # Extreme string escapes
            "let s = \"" + "\\n" * 1000 + "\";",

            # Complex array nesting
            "[" * 500 + "1" + "]" * 500,

            # Function parameter explosion
            "fn test(" + ", ".join([f"p{i}" for i in range(1000)]) + ") { }",

            # Malformed UTF-8
            "let x = \"\x80\x81\x82\";",
        ]
        return random.choice(patterns)

    def generate_memory_stress_test(self) -> str:
        """Generate inputs that might cause memory issues"""
        patterns = [
            # Large object creation
            f"class Test {{ {'; '.join([f'let x{i} = {i}' for i in range(10000)])} }}",

            # Recursive function definitions
            "fn a() { return b(); } fn b() { return a(); } a();",

            # Large array with object references
            f"let arr = [{', '.join(['{}'] * 1000)}];",

            # Circular references attempt
            """
            class Node {
                let value = 0;
                let next = null;
            }
            let n1 = Node();
            let n2 = Node();
            n1.next = n2;
            n2.next = n1;
            """,

            # Memory allocation in loops
            """
            let big_arrays = [];
            for (let i = 0; i < 1000; i++) {
                big_arrays[i] = [];
                for (let j = 0; j < 1000; j++) {
                    big_arrays[i][j] = j;
                }
            }
            """,
        ]
        return random.choice(patterns)

    def generate_type_confusion_test(self) -> str:
        """Generate inputs that might cause type confusion"""
        patterns = [
            # Mixed type operations
            "let x = 1 + \"hello\" + [] + null + true;",

            # Invalid member access
            "let x = 42; x.nonexistent.field.access;",

            # Function call on non-function
            "let x = 42; x();",
            "let x = \"hello\"; x();",
            "let x = []; x();",
            "let x = true; x();",

            # Array access on non-array
            "let x = \"hello\"; x[999999];",
            "let x = 42; x[0];",
            "let x = true; x[\"key\"];",

            # Class instantiation edge cases
            "class Test {} let x = Test; x();",

            # Null/undefined operations
            "null.property; null();",

            # Deeply nested member access
            "let x = {}; x.a.b.c.d.e.f.g.h.i.j;",

            # Mixed array and member access
            "let x = []; x.length[0].property();",
        ]
        return random.choice(patterns)

    def generate_control_flow_test(self) -> str:
        """Generate complex control flow that might break the interpreter"""
        patterns = [
            # Deeply nested blocks
            "{" * 1000 + "let x = 1;" + "}" * 1000,

            # Complex loop nesting
            """
            for (let i = 0; i < 100; i++) {
                for (let j = 0; j < 100; j++) {
                    for (let k = 0; k < 100; k++) {
                        if (i == j) break;
                        if (j == k) continue;
                        return;
                    }
                }
            }
            """,

            # Exception handling edge cases
            """
            fn test() {
                try {
                    throw "error";
                } catch (e) {
                    throw e;
                }
            }
            test();
            """,

            # Infinite recursion potential
            "fn inf() { return inf(); } inf();",
        ]
        return random.choice(patterns)

    def generate_random_mutation(self, base_code: str) -> str:
        """Apply random mutations to valid code"""
        code = base_code

        # Apply 1-5 random mutations
        for _ in range(random.randint(1, 5)):
            mutation_type = random.choice([
                'insert_random', 'delete_char', 'replace_char',
                'duplicate_substring', 'shuffle_substring'
            ])

            if not code:
                continue

            if mutation_type == 'insert_random':
                pos = random.randint(0, len(code))
                char = chr(random.randint(0, 127))
                code = code[:pos] + char + code[pos:]

            elif mutation_type == 'delete_char':
                pos = random.randint(0, len(code) - 1)
                code = code[:pos] + code[pos + 1:]

            elif mutation_type == 'replace_char':
                pos = random.randint(0, len(code) - 1)
                char = chr(random.randint(0, 127))
                code = code[:pos] + char + code[pos + 1:]

            elif mutation_type == 'duplicate_substring':
                start = random.randint(0, len(code) - 1)
                end = random.randint(start, len(code))
                substr = code[start:end]
                pos = random.randint(0, len(code))
                code = code[:pos] + substr + code[pos:]

            elif mutation_type == 'shuffle_substring':
                if len(code) > 10:
                    start = random.randint(0, len(code) - 10)
                    end = start + 10
                    substr = list(code[start:end])
                    random.shuffle(substr)
                    code = code[:start] + ''.join(substr) + code[end:]

        return code

    def run_test(self, code: str, timeout: int = 5) -> Dict:
        """Run a single test case"""
        result = {
            'crashed': False,
            'timeout': False,
            'exit_code': 0,
            'output': '',
            'error': '',
            'signal': None
        }

        # Write code to temporary file
        with tempfile.NamedTemporaryFile(mode='w', suffix='.li', delete=False) as f:
            f.write(code)
            temp_file = f.name

        try:
            # Run the interpreter with timeout
            process = subprocess.Popen(
                [self.interpreter_path, temp_file],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )

            try:
                stdout, stderr = process.communicate(timeout=timeout)
                result['output'] = stdout
                result['error'] = stderr
                result['exit_code'] = process.returncode

                if process.returncode < 0:
                    result['crashed'] = True
                    result['signal'] = -process.returncode

            except subprocess.TimeoutExpired:
                process.kill()
                process.wait()
                result['timeout'] = True

        except Exception as e:
            result['error'] = str(e)

        finally:
            # Clean up
            try:
                os.unlink(temp_file)
            except:
                pass

        return result

    def is_interesting(self, result: Dict) -> bool:
        """Determine if a test result is interesting"""
        return (result['crashed'] or
                result['timeout'] or
                result['exit_code'] not in [0, 1] or  # 1 is normal error exit
                'segmentation fault' in result['error'].lower() or
                'abort' in result['error'].lower() or
                'assertion' in result['error'].lower())

    def run_fuzzing_campaign(self, num_tests: int = 1000):
        """Run the main fuzzing campaign"""
        print(f"Starting advanced fuzzing campaign with {num_tests} tests...")

        generators = [
            self.generate_parser_stress_test,
            self.generate_memory_stress_test,
            self.generate_type_confusion_test,
            self.generate_control_flow_test,
        ]

        # Some valid base codes for mutation
        valid_codes = [
            "let x = 1; println(x);",
            "fn test() { return 42; } println(test());",
            "class Test { let x = 1; } let t = Test(); println(t.x);",
            "let arr = [1, 2, 3]; for (let x : arr) { println(x); }",
            "if (true) { println(\"true\"); } else { println(\"false\"); }",
        ]

        interesting_count = 0

        for i in range(num_tests):
            if i % 100 == 0:
                print(f"Progress: {i}/{num_tests} "
                     f"(Crashes: {self.crash_count}, "
                     f"Timeouts: {self.timeout_count}, "
                     f"Interesting: {interesting_count})")

            # Choose test type
            if random.random() < 0.7:
                # Use generators
                generator = random.choice(generators)
                code = generator()
            else:
                # Mutate valid code
                base = random.choice(valid_codes)
                code = self.generate_random_mutation(base)

            # Run the test
            result = self.run_test(code)

            # Check if interesting
            if self.is_interesting(result):
                interesting_count += 1
                self.interesting_inputs.append((code, result))

                if result['crashed']:
                    self.crash_count += 1
                    print(f"CRASH FOUND (Test {i})!")

                if result['timeout']:
                    self.timeout_count += 1
                    print(f"TIMEOUT FOUND (Test {i})!")

        self.report_results()

    def report_results(self):
        """Generate a report of findings"""
        print("\n" + "="*50)
        print("FUZZING RESULTS")
        print("="*50)
        print(f"Total crashes: {self.crash_count}")
        print(f"Total timeouts: {self.timeout_count}")
        print(f"Total interesting cases: {len(self.interesting_inputs)}")

        # Save detailed results
        with open("advanced_fuzzer_results.log", "w") as f:
            f.write("Advanced Lithium Fuzzer Results\n")
            f.write("="*50 + "\n\n")

            for i, (code, result) in enumerate(self.interesting_inputs):
                f.write(f"=== Interesting Case {i+1} ===\n")
                f.write(f"Crashed: {result['crashed']}\n")
                f.write(f"Timeout: {result['timeout']}\n")
                f.write(f"Exit Code: {result['exit_code']}\n")
                if result['signal']:
                    f.write(f"Signal: {result['signal']}\n")
                f.write("Code:\n")
                f.write(code + "\n")
                f.write("Output:\n")
                f.write(result['output'] + "\n")
                f.write("Error:\n")
                f.write(result['error'] + "\n")
                f.write("-" * 30 + "\n\n")

        print("Detailed results saved to: advanced_fuzzer_results.log")

if __name__ == "__main__":
    import sys

    if not os.path.exists("../src/li"):
        print("Error: Lithium interpreter not found at ../src/li")
        print("Please compile the interpreter first.")
        sys.exit(1)

    num_tests = int(sys.argv[1]) if len(sys.argv) > 1 else 1000

    fuzzer = LithiumAdvancedFuzzer()
    fuzzer.run_fuzzing_campaign(num_tests)
