#include "Token.h"
#include "Node.h"

string Token::tokenTypeToString(int type) const
{
    switch (type) {
        case NONE: return "none";
        case END: return "eof";
        case IDENT: return "identifier";
        case NUMBER: return "number";
        case STRING: return "string";
        case LET: return "let";
        case CONST: return "const";
        case IF: return "if";
        case ELSE: return "else";
        case FOR: return "for";
        case WHILE: return "while";
        case FN: return "fn";
        case RETURN: return "return";
        case CLASS: return "class";
        case IMPORT: return "import";
        case PRINT: return "print";
        case EQ: return "equal";
        case NE: return "not_equal";
        case LE: return "less_or_equal";
        case GE: return "greater_or_equal";
        case OR: return "or";
        case AND: return "and";
        case INC: return "increment";
        case DEC: return "decrement";
        case JUNK: return "junk";
        default:
        {
            if (0 < type && type < 256)
            {
                return string(1, (char)type);
            } else {
                return "junk";
            }
        }
    }
}

Token::Token() : type(NONE),
                 typeAsType((TokenType)type),
                 range(),
                 value("")
{
}

Token::Token(int type, const Range &range, const std::string &value):
    type(type),
    typeAsType((TokenType)type),
    range(range), 
    value(value)
{
}

Token::Token(int type, const Range &range, char value):
    type(type),
    typeAsType((TokenType)type),
    range(range), 
    value(1, value) // converts char to string
{
}

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

void Token::setType(int type)
{
    this->type = type;
}

void Token::setRange(const Range &range)
{
    this->range = range;
}

void Token::setValue(const std::string &value)
{
    this->value = value;
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
