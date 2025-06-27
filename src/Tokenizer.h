//**************************************************
// File: Tokenizer.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the Tokenizer class, which
// tokenizes input source code for the parser.
//**************************************************

#pragma once

#include <istream>
#include <string>
#include <memory>

#include "Token.h"
#include "Location.h"

using std::string;
using std::istream;
using std::shared_ptr;
using std::make_shared;

class Tokenizer
{
public:
    Tokenizer();
    Tokenizer(const string &input, const string &filename = "");
    ~Tokenizer(); // Add explicit destructor
    Token lex();
private:
    void skipWhitespace();
    char peek() const;
    void advance();
private:
    shared_ptr<string> input;
    shared_ptr<string> filename;
    Location location;
    bool endOfFile;
};