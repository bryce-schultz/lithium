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
    //InterpreterVisitor interpreter;
    string line;
    while ((line = Utils::getInputLine()) != "exit")
    {
        Result<Node> result = parser.parse(line, "cin");
        if (result.success)
        {
            XmlVisitor xmlVisitor;
            xmlVisitor.visitAllChildren(result.node);

            std::cout << xmlVisitor.getOutput();
            delete result.node; // Clean up the parsed node
        }
        line.clear();
    }

    return 0;
}

int runFileMode(const string &filename)
{
    string input;
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
    if (result.success)
    {
        XmlVisitor xmlVisitor;
        xmlVisitor.visitAllChildren(result.node);

        std::cout << xmlVisitor.getOutput();
        delete result.node; // Clean up the parsed node
    }
    else
    {
        std::cout << "parsing failed." << std::endl;
        return 1;
    }

    return 0;
}
