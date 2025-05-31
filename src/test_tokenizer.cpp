#define CTEST

#include "ctest.h"

#include "Tokenizer.h"

Test test_tokenize_single_character_tokens()
{
    Tokenizer tokenizer("()[]{};.,");
    Token token;

    // Test single character tokens
    token = tokenizer.lex();
    assertTrue(token == '(');

    token = tokenizer.lex();
    assertTrue(token == ')');

    token = tokenizer.lex();
    assertTrue(token == '[');

    token = tokenizer.lex();
    assertTrue(token == ']');

    token = tokenizer.lex();
    assertTrue(token == '{');

    token = tokenizer.lex();
    assertTrue(token == '}');

    token = tokenizer.lex();
    assertTrue(token == ';');

    token = tokenizer.lex();
    assertTrue(token == '.');

    token = tokenizer.lex();
    assertTrue(token == ',');

    // ensure we reach the end of the input
    token = tokenizer.lex();
    assertTrue(token == Token::END);
}

Test test_two_character_tokens()
{
    Tokenizer tokenizer("== != <= >= ++ --");
    Token token;

    // Test two character tokens
    token = tokenizer.lex();
    assertTrue(token == Token::EQ); // '=='

    token = tokenizer.lex();
    assertTrue(token == Token::NE); // '!='

    token = tokenizer.lex();
    assertTrue(token == Token::LE); // '<='

    token = tokenizer.lex();
    assertTrue(token == Token::GE); // '>='

    token = tokenizer.lex();
    assertTrue(token == Token::INC); // '++'

    token = tokenizer.lex();
    assertTrue(token == Token::DEC); // '--'

    // ensure we reach the end of the input
    token = tokenizer.lex();
    assertTrue(token == Token::END);
}

Test test_keyword_tokens()
{
    Tokenizer tokenizer("let const if else for while fn return class import print");
    Token token;

    // Test keyword tokens
    token = tokenizer.lex();
    assertTrue(token == Token::LET);

    token = tokenizer.lex();
    assertTrue(token == Token::CONST);

    token = tokenizer.lex();
    assertTrue(token == Token::IF);

    token = tokenizer.lex();
    assertTrue(token == Token::ELSE);

    token = tokenizer.lex();
    assertTrue(token == Token::FOR);

    token = tokenizer.lex();
    assertTrue(token == Token::WHILE);

    token = tokenizer.lex();
    assertTrue(token == Token::FN);

    token = tokenizer.lex();
    assertTrue(token == Token::RETURN);

    token = tokenizer.lex();
    assertTrue(token == Token::CLASS);

    token = tokenizer.lex();
    assertTrue(token == Token::IMPORT);

    token = tokenizer.lex();
    assertTrue(token == Token::PRINT);

    // ensure we reach the end of the input
    token = tokenizer.lex();
    assertTrue(token == Token::END);
}

Test test_tokenize_junk_tokens()
{
    Tokenizer tokenizer("@#$^~");

    Token token;
    // Test junk tokens
    token = tokenizer.lex();
    assertTrue(token == Token::JUNK); // '@'
    token = tokenizer.lex();
    assertTrue(token == Token::JUNK); // '#'
    token = tokenizer.lex();
    assertTrue(token == Token::JUNK); // '$'
    token = tokenizer.lex();
    assertTrue(token == Token::JUNK); // '^'
    token = tokenizer.lex();
    assertTrue(token == Token::JUNK); // '~'
    // ensure we reach the end of the input
    token = tokenizer.lex();
    assertTrue(token == Token::END);
}

int main()
{
    START_TESTS

    START_GROUP(Tokenization Tests);
    TEST(test_tokenize_single_character_tokens);
    TEST(test_two_character_tokens);
    TEST(test_keyword_tokens);
    TEST(test_tokenize_junk_tokens);
    END_GROUP;

    END_TESTS;
}