#include <iostream>
#include <sstream>

#include "Parser.h"
#include "XmlVisitor.h"

int main(int argc, char **argv)
{
    auto id = global::files.openFile("test.li");
    if (id == INVALID_FILE_ID)
    {
        std::cerr << "error opening file: test.li" << std::endl;
        return 1;
    }

    Tokenizer tester(id);
    
    Token token;
    while (token != Token::END)
    {
        token = tester.lex();
        std::cout << token.toString() << " @ " << token.getRange().getStart().toString() << std::endl;
    }

    istream *input = &std::cin;
    if (argc == 2)
    {
        input = new std::ifstream(argv[1]);
        if (!input->good())
        {
            std::cerr << "error opening file: " << argv[1] << std::endl;
            return 1;
        }
    }

    Parser parser;
    Result<Node> result = parser.parse(*input, argc == 2 ? argv[1] : "cin");

    if (result.success)
    {
        XmlVisitor xmlVisitor;
        xmlVisitor.visitAllChildren(result.node);

        std::cout << xmlVisitor.getOutput();
        delete result.node; // Clean up the parsed node
    }
    else
    {
        std::cout << "Parsing failed." << std::endl;
    }

    if (input != &std::cin)
    {
        delete input; // Clean up if we opened a file
    }

    return 0;
}