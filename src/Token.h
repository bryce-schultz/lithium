//**************************************************
// File: Token.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the Token class, representing
// lexical tokens and their types for parsing.
//**************************************************

#pragma once

#include <string>

#include "Range.h"

using std::string;

class Token
{
public:
    enum TokenType {
        NONE = 256,
        END,
        IDENT,
        NUMBER,
        STRING,
        LET,
        CONST,
        IF,
        ELSE,
        FOR,
        WHILE,
        FN,
        RETURN,
        BREAK,
        CONTINUE,
        CLASS,
        IMPORT,
        EQ,
        NE,
        LE,
        GE,
        OR,
        AND,
        INC,
        DEC,
        PLUS_EQUAL,
        MINUS_EQUAL,
        MUL_EQUAL,
        DIV_EQUAL,
        MOD_EQUAL,
        JUNK
    };

    static string tokenTypeToString(int type);
public:
    Token();
    Token(int type, const Range& range, const string& value);
    Token(int type, const Range& range, char value);

    int getType() const;
    const Range& getRange() const;
    const string& getValue() const;

    bool operator==(const Token& other) const;
    bool operator!=(const Token& other) const;

    bool operator==(int type) const;
    bool operator!=(int type) const;

    string toString() const;
private:
    int type;
    Range range;
    string value;
};