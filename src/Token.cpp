//**************************************************
// File: Token.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the Token class, which
// represents lexical tokens in the source code.
//**************************************************

#include "Token.h"
#include "Node.h"

string Token::tokenTypeToString(int type)
{
    switch (type)
    {
        case NONE:      return "none";
        case END:       return "eof";
        case IDENT:     return "identifier";
        case NUMBER:    return "number";
        case STRING:    return "string";
        case LET:       return "keyword (let)";
        case CONST:     return "keyword (const)";
        case IF:        return "keyword (if)";
        case ELSE:      return "keyword (else)";
        case FOR:       return "keyword (for)";
        case WHILE:     return "keyword (while)";
        case FN:        return "keyword (fn)";
        case RETURN:    return "keyword (return)";
        case BREAK:     return "keyword (break)";
        case CLASS:     return "keyword (class)";
        case IMPORT:    return "keyword (import)";
        case PRINT:     return "keyword (print)";
        case EQ:        return "operator (==)";
        case NE:        return "operator (!=)";
        case LE:        return "operator (<=)";
        case GE:        return "operator (>=)";
        case OR:        return "operator (||)";
        case AND:       return "operator (&&)";
        case INC:       return "operator (++)";
        case DEC:       return "operator (--)";
        case PLUS_EQUAL: return "operator (+=)";
        case MINUS_EQUAL: return "operator (-=)";
        case MUL_EQUAL: return "operator (*=)";
        case DIV_EQUAL: return "operator (/=)";
        case MOD_EQUAL: return "operator (%=)";
        case JUNK:      return "junk";
        default:
        {
            if (0 < type && type < 256)
            {
                return string(1, (char)type);
            }
            else
            {
                return "junk";
            }
        }
    }
}

Token::Token():
    type(NONE),
    range(),
    value("")
{ }

Token::Token(int type, const Range &range, const std::string &value):
    type(type),
    range(range),
    value(value)
{ }

Token::Token(int type, const Range &range, char value):
    type(type),
    range(range),
    value(1, value) // converts char to string
{ }

int Token::getType() const
{
    return type;
}

const Range& Token::getRange() const
{
    return range;
}

const std::string& Token::getValue() const
{
    return value;
}

bool Token::operator==(const Token &other) const
{
    return type == other.type && range == other.range && value == other.value;
}

bool Token::operator!=(const Token &other) const
{
    return !(*this == other);
}

bool Token::operator==(int type) const
{
    return this->type == type;
}

bool Token::operator!=(int type) const
{
    return this->type != type;
}

std::string Token::toString() const
{
    string result = tokenTypeToString(type);

    if (type == IDENT ||
        type == NUMBER ||
        type == STRING)
    {
       result += " (" + value + ")";
    }

    return result;
}
