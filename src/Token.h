#pragma once

#include <string>
#include "Range.h"

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
        CLASS,
        IMPORT,
        PRINT,
        EQ,
        NE,
        LE,
        GE,
        OR,
        AND,
        INC,
        DEC,
        JUNK
    };
    string tokenTypeToString(int type) const;
public:
    Token();
    Token(int type, const Range& range, const std::string& value);
    Token(int type, const Range& range, char value);

    int getType() const;
    const Range& getRange() const;
    const std::string& getValue() const;

    void setType(int type);
    void setRange(const Range& range);
    void setValue(const std::string& value);

    bool operator==(const Token& other) const;
    bool operator!=(const Token& other) const;

    bool operator==(int type) const;
    bool operator!=(int type) const;

    std::string toString() const;
private:
    int type;
    TokenType typeAsType;
    Range range;
    string value;
};