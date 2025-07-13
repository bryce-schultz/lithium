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
#include "Exceptions.h"  // Add this for ExitException

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::exception;
using std::vector;

int runInteractiveMode(const vector<string> &args);
int runFileMode(const vector<string> &args);

int main(int argc, char **argv)
{
    srandom(time(nullptr) ^ getpid());

    vector<string> args;
    if (argc > 1)
    {
        args = vector<string>(argv + 1, argv + argc);
        Utils::removePrefix(args[0], "./");
    }

    // if no file is specified, or args[0] is not a file, run interactive mode
    if (argc == 1 || !Utils::fileExists(args[0]))
    {
        cout << "lithium v" << INTERPRETER_VERSION << " type 'exit' to quit." << endl;
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
    while ((line = Utils::getInputLine()) != "exit")
    {
        clearErrorLocations();
        semanticVisitor.resetErrorCount();

        Result<Node> result = parser.parse(line, "cin");
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
    string input;
    string filename = args[0];

    try
    {
        input = Utils::readWholeFile(filename);
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
        return 1;
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

    {
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
            // Clean up AST and environment, then exit
            result.value.reset();
            if (env)
            {
                env->clear();
            }
            return e.exitCode;
        }
    } // interpreter destructor called here

    // Clear the environment after interpreter is destroyed
    if (env)
    {
        env->clear();
    }

    // Explicitly clean up the AST
    result.value.reset();

    return 0;
}
