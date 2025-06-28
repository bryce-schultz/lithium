#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <algorithm>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ofstream;

class LithiumFuzzer {
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> charDist;
    std::uniform_int_distribution<int> lengthDist;
    std::uniform_real_distribution<double> probDist;

    // Common Lithium keywords and constructs
    vector<string> keywords = {
        "let", "const", "fn", "class", "if", "else", "while", "for",
        "return", "break", "continue", "import", "null", "true", "false",
        "println", "print", "type", "len", "assert", "foreach", "in"
    };

    vector<string> operators = {
        "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=",
        "&&", "||", "!", "++", "--", "+=", "-=", "*=", "/=", "%=",
        "(", ")", "{", "}", "[", "]", ";", ",", ".", ":"
    };

    vector<string> literals = {
        "0", "1", "-1", "3.14", "-2.71", "\"hello\"", "\"\"", "[]", "[1,2,3]"
    };

    vector<string> common_patterns = {
        "x = 1;",
        "fn test() { return 42; }",
        "class Test { let x = 0; }",
        "let arr = [1, 2, 3];",
        "if (true) { println(\"ok\"); }",
        "for (let i = 0; i < 10; i++) {}",
        "while (false) { break; }"
    };

public:
    LithiumFuzzer() : rng(std::chrono::steady_clock::now().time_since_epoch().count()),
                      charDist(32, 126), lengthDist(1, 1000), probDist(0.0, 1.0) {}

    // Generate random string with printable ASCII characters
    string generateRandomString(int maxLen = 100) {
        int len = lengthDist(rng) % maxLen + 1;
        string result;
        result.reserve(len);
        for (int i = 0; i < len; i++) {
            result += static_cast<char>(charDist(rng));
        }
        return result;
    }

    // Generate malformed syntax patterns
    string generateMalformedSyntax() {
        vector<string> patterns = {
            // Unmatched brackets/braces
            "{ { { } }",
            "( ( ) ) )",
            "[ [ ] ] ]",
            "{ ( } )",

            // Invalid operators
            "x ++ ++",
            "y -- --",
            "z === w",
            "a <== b",

            // Malformed strings
            "\"unclosed string",
            "\"escaped \\\" middle",
            "\"\\x invalid escape\"",

            // Invalid numbers
            "123.45.67",
            ".123.",
            "1e",
            "0x",

            // Malformed function declarations
            "fn () { }",
            "fn name( { }",
            "fn name() }",
            "fn 123() { }",

            // Invalid class syntax
            "class { }",
            "class 123 { }",
            "class Test { fn { } }",

            // Nested function/class errors
            "fn outer() { fn inner() { class Nested { } } }",

            // Invalid control flow
            "if { }",
            "while { }",
            "for { }",
            "return return;",

            // Invalid assignments
            "123 = x;",
            "\"string\" = y;",
            "x + y = z;",

            // Unicode and special characters
            "let café = 1;",
            "fn 函数() { }",
            "class 类 { }",
        };

        return patterns[rng() % patterns.size()];
    }

    // Generate edge case inputs
    string generateEdgeCase() {
        vector<string> patterns = {
            // Very long identifiers
            "let " + string(10000, 'x') + " = 1;",

            // Deeply nested structures
            string(1000, '{') + string(1000, '}'),
            string(500, '(') + "x" + string(500, ')'),

            // Very large numbers
            "let big = " + string(1000, '9') + ";",
            "let decimal = 1." + string(1000, '0') + "1;",

            // Very long strings
            "let str = \"" + string(10000, 'a') + "\";",

            // Many function parameters
            "fn test(" + [this]() {
                string params;
                for (int i = 0; i < 1000; i++) {
                    if (i > 0) params += ", ";
                    params += "p" + std::to_string(i);
                }
                return params;
            }() + ") { }",

            // Deep recursion potential
            "fn recursive() { return recursive(); } recursive();",

            // Memory intensive patterns
            "let arr = [" + [this]() {
                string elements;
                for (int i = 0; i < 10000; i++) {
                    if (i > 0) elements += ", ";
                    elements += std::to_string(i);
                }
                return elements;
            }() + "];",
        };

        return patterns[rng() % patterns.size()];
    }

    // Generate mutation of valid code
    string mutateValidCode() {
        string base = common_patterns[rng() % common_patterns.size()];

        // Apply random mutations
        for (int i = 0; i < 3; i++) {
            int mutationType = rng() % 6;
            switch (mutationType) {
                case 0: // Insert random character
                    if (!base.empty()) {
                        int pos = rng() % base.length();
                        base.insert(pos, 1, static_cast<char>(charDist(rng)));
                    }
                    break;
                case 1: // Delete character
                    if (!base.empty()) {
                        int pos = rng() % base.length();
                        base.erase(pos, 1);
                    }
                    break;
                case 2: // Replace character
                    if (!base.empty()) {
                        int pos = rng() % base.length();
                        base[pos] = static_cast<char>(charDist(rng));
                    }
                    break;
                case 3: // Duplicate substring
                    if (!base.empty()) {
                        int start = rng() % base.length();
                        int len = rng() % (base.length() - start) + 1;
                        string substr = base.substr(start, len);
                        base += substr;
                    }
                    break;
                case 4: // Insert keyword/operator
                    {
                        string toInsert = (rng() % 2) ?
                            keywords[rng() % keywords.size()] :
                            operators[rng() % operators.size()];
                        int pos = base.empty() ? 0 : rng() % base.length();
                        base.insert(pos, toInsert);
                    }
                    break;
                case 5: // Shuffle small section
                    if (base.length() > 4) {
                        int start = rng() % (base.length() - 4);
                        std::shuffle(base.begin() + start, base.begin() + start + 4, rng);
                    }
                    break;
            }
        }

        return base;
    }

    // Generate a test case
    string generateTestCase() {
        int type = rng() % 5;
        switch (type) {
            case 0: return generateRandomString();
            case 1: return generateMalformedSyntax();
            case 2: return generateEdgeCase();
            case 3: return mutateValidCode();
            default: {
                // Combine multiple patterns
                string result;
                int numParts = rng() % 5 + 1;
                for (int i = 0; i < numParts; i++) {
                    result += generateTestCase() + " ";
                }
                return result;
            }
        }
    }

    // Test a single input and return result
    struct TestResult {
        bool crashed;
        bool timeout;
        bool memoryLeak;
        int exitCode;
        string output;
        string error;
    };

    TestResult testInput(const string& input, int timeoutSeconds = 5) {
        TestResult result = {false, false, false, 0, "", ""};

        // Write input to temporary file
        string tempFile = "/tmp/lithium_fuzz_" + std::to_string(getpid()) + "_" + std::to_string(rng());
        ofstream file(tempFile);
        file << input;
        file.close();

        // Create pipes for stdout/stderr
        int stdoutPipe[2], stderrPipe[2];
        pipe(stdoutPipe);
        pipe(stderrPipe);

        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            close(stdoutPipe[0]);
            close(stderrPipe[0]);
            dup2(stdoutPipe[1], STDOUT_FILENO);
            dup2(stderrPipe[1], STDERR_FILENO);
            close(stdoutPipe[1]);
            close(stderrPipe[1]);

            // Run the interpreter
            execl("../src/li", "li", tempFile.c_str(), nullptr);
            exit(127); // exec failed
        } else if (pid > 0) {
            // Parent process
            close(stdoutPipe[1]);
            close(stderrPipe[1]);

            // Set up timeout
            alarm(timeoutSeconds);

            int status;
            pid_t waitResult = waitpid(pid, &status, 0);
            alarm(0); // Cancel alarm

            if (waitResult == -1) {
                // Timeout occurred
                kill(pid, SIGKILL);
                waitpid(pid, nullptr, 0);
                result.timeout = true;
            } else {
                if (WIFSIGNALED(status)) {
                    result.crashed = true;
                    result.exitCode = WTERMSIG(status);
                } else {
                    result.exitCode = WEXITSTATUS(status);
                }
            }

            // Read output
            char buffer[4096];
            ssize_t bytesRead;

            while ((bytesRead = read(stdoutPipe[0], buffer, sizeof(buffer) - 1)) > 0) {
                buffer[bytesRead] = '\0';
                result.output += buffer;
            }

            while ((bytesRead = read(stderrPipe[0], buffer, sizeof(buffer) - 1)) > 0) {
                buffer[bytesRead] = '\0';
                result.error += buffer;
            }

            close(stdoutPipe[0]);
            close(stderrPipe[0]);
        }

        // Clean up temp file
        unlink(tempFile.c_str());

        return result;
    }

    // Run fuzzing campaign
    void runFuzzingCampaign(int numTests = 1000, const string& logFile = "fuzzer_results.log") {
        ofstream log(logFile);

        cout << "Starting fuzzing campaign with " << numTests << " test cases..." << endl;

        int crashes = 0, timeouts = 0, errors = 0;

        for (int i = 0; i < numTests; i++) {
            if (i % 100 == 0) {
                cout << "Progress: " << i << "/" << numTests << " (Crashes: " << crashes
                     << ", Timeouts: " << timeouts << ", Errors: " << errors << ")" << endl;
            }

            string testCase = generateTestCase();
            TestResult result = testInput(testCase);

            bool interesting = result.crashed || result.timeout || result.exitCode != 0;

            if (interesting) {
                log << "=== Test Case " << i << " ===" << endl;
                log << "Input:" << endl << testCase << endl;
                log << "Crashed: " << (result.crashed ? "YES" : "NO") << endl;
                log << "Timeout: " << (result.timeout ? "YES" : "NO") << endl;
                log << "Exit Code: " << result.exitCode << endl;
                log << "Output:" << endl << result.output << endl;
                log << "Error:" << endl << result.error << endl;
                log << "===================" << endl << endl;
                log.flush();

                if (result.crashed) crashes++;
                if (result.timeout) timeouts++;
                if (result.exitCode != 0 && !result.crashed) errors++;
            }
        }

        cout << "Fuzzing completed!" << endl;
        cout << "Total crashes: " << crashes << endl;
        cout << "Total timeouts: " << timeouts << endl;
        cout << "Total errors: " << errors << endl;
        cout << "Results saved to: " << logFile << endl;

        log.close();
    }
};

int main(int argc, char* argv[]) {
    int numTests = 1000;
    string logFile = "fuzzer_results.log";

    if (argc > 1) {
        numTests = std::atoi(argv[1]);
    }
    if (argc > 2) {
        logFile = argv[2];
    }

    // Check if interpreter exists
    if (access("../src/li", X_OK) != 0) {
        cout << "Error: Lithium interpreter '../src/li' not found or not executable." << endl;
        cout << "Please compile the interpreter first." << endl;
        return 1;
    }

    LithiumFuzzer fuzzer;
    fuzzer.runFuzzingCampaign(numTests, logFile);

    return 0;
}
