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
        case AND:           return "operator (&&)";
        case ASSERT:        return "keyword (assert)";
        case BREAK:         return "keyword (break)";
        case CLASS:         return "keyword (class)";
        case CONST:         return "keyword (const)";
        case DEC:           return "operator (--)";
        case DELETE:        return "keyword (delete)";
        case DIV_EQUAL:     return "operator (/=)";
        case ELSE:          return "keyword (else)";
        case END:           return "eof";
        case EQ:            return "operator (==)";
        case FALSE:         return "keyword (false)";
        case FN:            return "keyword (fn)";
        case FOR:           return "keyword (for)";
        case FOREACH:       return "keyword (foreach)";
        case GE:            return "operator (>=)";
        case IDENT:         return "identifier";
        case IF:            return "keyword (if)";
        case IMPORT:        return "keyword (import)";
        case INC:           return "operator (++)";
        case JUNK:          return "junk";
        case LE:            return "operator (<=)";
        case LET:           return "keyword (let)";
        case MINUS_EQUAL:   return "operator (-=)";
        case MOD_EQUAL:     return "operator (%=)";
        case MUL_EQUAL:     return "operator (*=)";
        case NE:            return "operator (!=)";
        case NONE:          return "none";
        case NULL_TOKEN:    return "keyword (null)";
        case NUMBER:        return "number";
        case OR:            return "operator (||)";
        case PLUS_EQUAL:    return "operator (+=)";
        case RETURN:        return "keyword (return)";
        case STRING:        return "string";
        case TRUE:          return "keyword (true)";
        case WHILE:         return "keyword (while)";
        default:
        {
            if (0 < type && type < 256)
            {
                return string(1, (char)type);
            }
            else
            {
                return "unknown";
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
