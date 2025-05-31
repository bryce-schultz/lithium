#pragma once

#include <istream>
#include <string>

#include "Token.h"
#include "Location.h"
#include "FileCache.h"

using std::string;
using std::istream;

class Tokenizer
{
public:
    Tokenizer();
    Tokenizer(FileId fileId);
    Token lex();
private:
    void skipWhitespace();
    char peek() const;
    void advance();
private:
    FileId fileId;
    Location location;
    bool endOfFile;
};