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
        AND,
        ASSERT,
        BREAK,
        CLASS,
        CONST,
        CONTINUE,
        DEC,
        DIV_EQUAL,
        ELSE,
        END,
        EQ,
        FN,
        FOR,
        FOREACH,
        GE,
        IDENT,
        IF,
        IMPORT,
        INC,
        JUNK,
        LE,
        LET,
        MINUS_EQUAL,
        MOD_EQUAL,
        MUL_EQUAL,
        NE,
        NUMBER,
        OR,
        PLUS_EQUAL,
        RETURN,
        STRING,
        WHILE
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