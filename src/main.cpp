//**************************************************
// File: main.cpp
//
// Author: Bryce Schultz
//
// Purpose: Entry point for the Lithium interpreter.
// Handles interactive mode and file execution.
//**************************************************

#include <iostream>
#include <random>

#include "Utils.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Environment.h"
#include "SemanticErrorVisitor.h"
#include "Values.h"
#include "Error.h"
#include "Color.h"
#include "Exceptions.h"  // Add this for ExitException

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::exception;
using std::vector;

#define error(msg) \
    generalError(msg, __FILE__, __LINE__); \
    return 1;

int runInteractiveMode(const vector<string> &args);
int runFileMode(const vector<string> &args);

int main(int argc, char **argv)
{
    srandom(static_cast<unsigned int>(time(nullptr) ^ getpid()));

    vector<string> args;
    if (argc > 1)
    {
        args = vector<string>(argv + 1, argv + argc);
        Utils::removePrefix(args[0], "./");
    }

    // if no file is specified, or args[0] is not a file, run interactive mode
    if (argc == 1 || !Utils::fileExists(args[0]))
    {
        cout << yellow << "lithium " << blue << INTERPRETER_VERSION << reset << "\ntype '" << cyan << "exit" << reset << "' to quit." << endl;
        return runInteractiveMode(args);
    }

    return runFileMode(args);
}

int runInteractiveMode(const vector<string> &args)
{
    Parser parser;
    shared_ptr<Environment> env = make_shared<Environment>();
    Interpreter interpreter(true, env, args);
    SemanticErrorVisitor semanticVisitor;

    string line;
    string accumulatedInput; // Track all input so far

    while ((line = Utils::getInputLine()) != "exit")
    {
        // Check for help command (with or without newline)
        if (line == "help\n" || line == "help")
        {
            cout << cyan << "📚 Lithium Interactive Help" << reset << endl;
            cout << endl;
            cout << yellow << "Commands:" << reset << endl;
            cout << "  " << green << "help" << reset << "   - Show this help message" << endl;
            cout << "  " << green << "exit" << reset << "   - Exit the interpreter" << endl;
            cout << endl;
            cout << "🚀 " << magenta << "Happy coding!" << reset << endl;
            cout << endl;
            continue;
        }

        clearErrorLocations();
        semanticVisitor.resetErrorCount();

        // Store current position for this line
        size_t currentLineStart = accumulatedInput.length();

        // Append current line to accumulated input (line already includes \n)
        accumulatedInput += line;

        // Parse with accumulated input starting from the current line position
        Result<Node> result = parser.parseFromPosition(accumulatedInput, currentLineStart, "cin");
        if (!result.status)
        {
            continue;
        }

        semanticVisitor.visitAllChildren(result.value.get());
        if (semanticVisitor.hasErrors())
        {
            continue;
        }

        try
        {
            interpreter.interpret(result.value.get()); // Reuse the same interpreter
        }
        catch (const ExitException &e)
        {
            // Clean up and exit
            result.value.reset();
            clearErrorLocations();
            return e.exitCode;
        }

        line.clear();
        clearErrorLocations();

        // Explicitly reset the result to ensure immediate cleanup
        result.value.reset();
    }

    return 0;
}

int runFileMode(const vector<string> &args)
{
    if (args.empty())
    {
        error("no source file specified.");
    }

    string input;
    string filename = args[0];

    try
    {
        input = Utils::readWholeFile(filename);
    }
    catch (const std::runtime_error &e)
    {
        error("failed to open source file: " + filename + "\n" + string(e.what()));
    }

    Parser parser;
    Result<Node> result = parser.parse(input, filename);
    if (!result.status)
    {
        return 1;
    }

    SemanticErrorVisitor semanticVisitor;
    semanticVisitor.visitAllChildren(result.value.get());
    if (semanticVisitor.hasErrors())
    {
        return 1;
    }

    shared_ptr<Environment> env = make_shared<Environment>();

    Interpreter interpreter(false, env, args);
    try
    {
        if (!interpreter.interpret(result.value.get()))
        {
            return 1;
        }
    }
    catch (const ExitException &e)
    {
        return e.exitCode;
    }

    return 0;
}
