//**************************************************
// File: main.cpp
//
// Author: Bryce Schultz
//
// Purpose: Entry point for the Lithium interpreter.
// Handles interactive mode and file execution.
//**************************************************

#include "Utils.h"
#include "Parser.h"
#include "XmlVisitor.h"
#include "Interpreter.h"
#include "Environment.h"
#include "SemanticErrorVisitor.h"

int runInteractiveMode();
int runFileMode(const std::string &filename);

int main(int argc, char **argv)
{
    // if a file is specified, run in file mode
    if (argc == 2)
    {
        return runFileMode(argv[1]);
    }

    // if no file is specified, run in interactive mode
    std::cout << "lithium v0.1. type 'exit' to quit." << std::endl;
    return runInteractiveMode();
}

int runInteractiveMode()
{
    Parser parser;
    shared_ptr<Environment> env = make_shared<Environment>();

    string line;
    while ((line = Utils::getInputLine()) != "exit")
    {
        Result<Node> result = parser.parse(line, "cin");
        if (!result.success)
        {
            continue;
        }

        SemanticErrorVisitor semanticVisitor;
        semanticVisitor.visitAllChildren(result.node.get());
        if (semanticVisitor.hasErrors())
        {
            continue;
        }

        Interpreter interpreter(env); // Use the shared environment
        interpreter.visitAllChildren(result.node.get());
        line.clear();
    }

    return 0;
}

int runFileMode(const string &filename)
{
    string input;
    shared_ptr<Environment> env = make_shared<Environment>();
    try
    {
        input = Utils::readWholeFile(filename);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    Parser parser;
    Result<Node> result = parser.parse(input, filename);
    if (!result.success)
    {
        return 1;
    }

    SemanticErrorVisitor semanticVisitor;
    semanticVisitor.visitAllChildren(result.node.get());
    if (semanticVisitor.hasErrors())
    {
        return 1;
    }

    Interpreter interpreter(env); // Create a new environment for file execution
    interpreter.visitAllChildren(result.node.get());

    return 0;
}
