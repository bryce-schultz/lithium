#include "Tokenizer.h"

Tokenizer::Tokenizer():
    fileId(INVALID_FILE_ID),
    location(1, 1),
    endOfFile(true)
{ }

Tokenizer::Tokenizer(FileId fileId): 
    fileId(fileId),
    location(1, 1, fileId),
    endOfFile(false)
{ }

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
    if (c == '*' ||
        c == '/' ||
        c == '%' ||
        c == ';' ||
        c == ',' ||
        c == '(' ||
        c == ')' ||
        c == '{' ||
        c == '}' ||
        c == '[' ||
        c == ']' ||
        c == '.') 
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
            str += peek();
            advance();
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

        if (identifier == "let") return Token(Token::LET, Range(start, location), identifier);
        if (identifier == "const") return Token(Token::CONST, Range(start, location), identifier);
        if (identifier == "if") return Token(Token::IF, Range(start, location), identifier);
        if (identifier == "else") return Token(Token::ELSE, Range(start, location), identifier);
        if (identifier == "for") return Token(Token::FOR, Range(start, location), identifier);
        if (identifier == "while") return Token(Token::WHILE, Range(start, location), identifier);
        if (identifier == "fn") return Token(Token::FN, Range(start, location), identifier);
        if (identifier == "return") return Token(Token::RETURN, Range(start, location), identifier);
        if (identifier == "class") return Token(Token::CLASS, Range(start, location), identifier);
        if (identifier == "import") return Token(Token::IMPORT, Range(start, location), identifier);
        if (identifier == "print") return Token(Token::PRINT, Range(start, location), identifier);

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
    while (c != '\0' && isspace(c)) 
    {
        advance();
        c = peek();
    }
}

char Tokenizer::peek() const
{
    auto record = global::files.getFileRecord(fileId);
    if (!record) 
    {
        return '\0'; // no input or file record available
    }
    istream *input = record->stream;

    return input->good() ? input->peek() : '\0';
}

void Tokenizer::advance()
{   
    auto record = global::files.getFileRecord(fileId);
    if (!record) 
    {
        return;
    }
    istream *input = record->stream;
    
    char current = '\0';
    if (input->good()) 
    {
        input->get(current);
        if (current == '\n') 
        {
            location.incrementLine();
            location.resetColumn();
        } 
        else 
        {
            location.incrementColumn();
        }
    }
}
