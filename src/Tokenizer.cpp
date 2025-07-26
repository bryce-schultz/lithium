//**************************************************
// File: Tokenizer.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the Tokenizer class, which
// tokenizes input source code into tokens for
// parsing.
//**************************************************

#include "Tokenizer.h"

Tokenizer::Tokenizer():
    input(),
    location(),
    endOfFile(true)
{ }

Tokenizer::Tokenizer(const string &input, const string &filename):
    input(make_shared<string>(input)),
    filename(make_shared<string>(filename)),
    location(),
    endOfFile(false)
{
    location = Location(0, this->input, this->filename);
}

Tokenizer::~Tokenizer()
{
    // Explicitly clear shared_ptr members to ensure cleanup
    input.reset();
    filename.reset();
}

Token Tokenizer::lex()
{
    Location start = location;

    skipWhitespace();

    char c = peek();
    // check if we have reached the end of the file
    if (c == '\0')
    {
        location = start; // reset the end location to the start of the token so white space is not counted
        return Token(Token::END, Range(start, location), "");
    }

    start = location; // update start to the current location after skipping whitespace

    //-----------------------------------------------------------
    // one character tokens
    if (c == ';' ||
        c == ':' ||
        c == ',' ||
        c == '(' ||
        c == ')' ||
        c == '{' ||
        c == '}' ||
        c == '[' ||
        c == ']' ||
        c == '.' ||
        c == '?')
    {
        advance();
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    //-----------------------------------------------------------
    // two character tokens
    if (c == '=')
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(Token::EQ, Range(start, location), "==");
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    if (c == '!')
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(Token::NE, Range(start, location), "!=");
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    if (c == '<')
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(Token::LE, Range(start, location), "<=");
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    if (c == '>')
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(Token::GE, Range(start, location), ">=");
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    if (c == '&')
    {
        advance();
        if (peek() == '&')
        {
            advance();
            return Token(Token::AND, Range(start, location), "&&");
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    if (c == '|')
    {
        advance();
        if (peek() == '|')
        {
            advance();
            return Token(Token::OR, Range(start, location), "||");
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    if (c == '+')
    {
        advance();
        if (peek() == '+')
        {
            advance();
            return Token(Token::INC, Range(start, location), "++");
        }
        else if (peek() == '=')
        {
            advance();
            return Token(Token::PLUS_EQUAL, Range(start, location), "+=");
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    if (c == '-')
    {
        advance();
        if (peek() == '-')
        {
            advance();
            return Token(Token::DEC, Range(start, location), "--");
        }
        else if (peek() == '=')
        {
            advance();
            return Token(Token::MINUS_EQUAL, Range(start, location), "-=");
        }
        else if (isdigit(peek()))
        {
            c = peek();
            bool isFloat = false;
            string number = "-"; // start with minus sign
            while (isdigit(c) || (c == '.' && !isFloat))
            {
                if (c == '.')
                {
                    isFloat = true; // mark as float
                }
                number += c;
                advance();
                c = peek();
            }
            return Token(Token::NUMBER, Range(start, location), number);
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    if (c == '*')
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(Token::MUL_EQUAL, Range(start, location), "*=");
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    if (c == '/')
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(Token::DIV_EQUAL, Range(start, location), "/=");
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    if (c == '%')
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(Token::MOD_EQUAL, Range(start, location), "%=");
        }
        return Token(static_cast<int>(c), Range(start, location), c);
    }

    //-----------------------------------------------------------
    // numbers
    if (isdigit(c))
    {
        bool isFloat = false;
        string number;
        while (isdigit(c) || (c == '.' && !isFloat))
        {
            if (c == '.')
            {
                isFloat = true; // mark as float
            }
            number += c;
            advance();
            c = peek();
        }
        return Token(Token::NUMBER, Range(start, location), number);
    }

    //-----------------------------------------------------------
    // strings
    if (c == '"')
    {
        advance();
        string str;
        while (peek() != '"' && peek() != '\0')
        {
            if (peek() == '\\') // handle escapes
            {
                advance();
                char next = peek();
                switch (next)
                {
                    case '"': str += '"'; break;
                    case '\\': str += '\\'; break;
                    case 'n': str += '\n'; break;
                    case 't': str += '\t'; break;
                    // add more escapes as needed
                    default: str += next; break; // unknown escape, just add the char
                }
                advance();
            }
            else
            {
                str += peek();
                advance();
            }
        }
        if (peek() == '"')
        {
            advance(); // consume closing quote
            return Token(Token::STRING, Range(start, location), str);
        }
        else
        {
            // unterminated string
            return Token(Token::JUNK, Range(start, location), str);
        }
    }

    //-----------------------------------------------------------
    // identifiers and keywords
    if (isalpha(c) || c == '_')
    {
        string identifier;
        while (isalnum(c) || c == '_')
        {
            identifier += c;
            advance();
            c = peek();
        }

        //-----------------------------------------------------------
        // keywords
        if (identifier == "assert") return Token(Token::ASSERT, Range(start, location), identifier);
        if (identifier == "break") return Token(Token::BREAK, Range(start, location), identifier);
        if (identifier == "class") return Token(Token::CLASS, Range(start, location), identifier);
        if (identifier == "const") return Token(Token::CONST, Range(start, location), identifier);
        if (identifier == "continue") return Token(Token::CONTINUE, Range(start, location), identifier);
        if (identifier == "delete") return Token(Token::DELETE, Range(start, location), identifier);
        if (identifier == "else") return Token(Token::ELSE, Range(start, location), identifier);
        if (identifier == "false") return Token(Token::FALSE, Range(start, location), identifier);
        if (identifier == "fn") return Token(Token::FN, Range(start, location), identifier);
        if (identifier == "for") return Token(Token::FOR, Range(start, location), identifier);
        if (identifier == "foreach") return Token(Token::FOREACH, Range(start, location), identifier);
        if (identifier == "if") return Token(Token::IF, Range(start, location), identifier);
        if (identifier == "import") return Token(Token::IMPORT, Range(start, location), identifier);
        if (identifier == "let") return Token(Token::LET, Range(start, location), identifier);
        if (identifier == "null") return Token(Token::NULL_TOKEN, Range(start, location), identifier);
        if (identifier == "return") return Token(Token::RETURN, Range(start, location), identifier);
        if (identifier == "true") return Token(Token::TRUE, Range(start, location), identifier);
        if (identifier == "while") return Token(Token::WHILE, Range(start, location), identifier);

        //-----------------------------------------------------------
        // identifiers
        return Token(Token::IDENT, Range(start, location), identifier);
    }

    //-----------------------------------------------------------
    // unknown character
    advance();
    return Token(Token::JUNK, Range(start, location), c);
}

void Tokenizer::skipWhitespace()
{
    char c = peek();
    while (c != '\0' && (isspace(c) || c == '#'))
    {
        if (c == '#') // skip comments
        {
            while (c != '\n' && c != '\0')
            {
                advance();
                c = peek();
            }
        }
        else
        {
            advance(); // skip whitespace
            c = peek();
        }
    }
}

char Tokenizer::peek() const
{
    if (!input)
    {
        return '\0'; // no input, return null character
    }

    if (location.getPos() >= input->size())
    {
        return '\0'; // out of bounds, return null character
    }

    return input->at(location.getPos());
}

void Tokenizer::advance()
{
    if (!input)
    {
        return; // no input, do nothing
    }

    if (location.getPos() < input->size())
    {
        location.move(1); // move to the next character
    }
    else
    {
        if (!endOfFile)
        {
            location.move(1); // move past the last character once
        }
        endOfFile = true; // reached the end of the file
    }
}
