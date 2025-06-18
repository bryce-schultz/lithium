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
        cout << "lithium v0.1. type 'exit' to quit." << endl;
        return runInteractiveMode(args);
    }

    return runFileMode(args);
}

int runInteractiveMode(const vector<string> &args)
{
    Parser parser;
    shared_ptr<Environment> env = make_shared<Environment>();
    env->declare("FILE", make_shared<StringValue>("cin"), true);
    // Optionally, you can use args in the interactive environment if needed

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

        Interpreter interpreter(true, env, args); // Pass args to interpreter
        interpreter.visitAllChildren(result.value.get());
        line.clear();
        clearErrorLocations();
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
    env->declare("FILE", make_shared<StringValue>(filename), true);

    Interpreter interpreter(false, env, args);
    interpreter.visitAllChildren(result.value.get());

    return 0;
}
