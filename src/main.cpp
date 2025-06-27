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
    Interpreter interpreter(true, env, args); // Create interpreter once, reuse it

    string line;
    while ((line = Utils::getInputLine()) != "exit")
    {
        Result<Node> result = parser.parse(line, "cin");
        if (!result.status)
        {
            continue;
        }

        SemanticErrorVisitor semanticVisitor;
        semanticVisitor.visitAllChildren(result.value.get());
        if (semanticVisitor.hasErrors())
        {
            continue;
        }

        try
        {
            interpreter.interpret(result.value.get()); // Reuse the same interpreter
        }
        catch (const exception &e)
        {
            // Exception caught during interpretation, ensure cleanup
            // The shared_ptr in result.value will automatically clean up when result goes out of scope
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
        if (!interpreter.interpret(result.value.get()))
        {
            return 1;
        }
    } // interpreter destructor called here

    // Clear the environment after interpreter is destroyed
    if (env)
    {
        env->clear();
    }

    return 0;
}
