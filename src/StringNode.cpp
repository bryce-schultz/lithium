//***********************************************
// File: StringNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the StringNode class, which represents
// a string literal in the abstract syntax tree (AST).
//***********************************************

#include <string>

#include "Visitor.h"
#include "StringNode.h"

StringNode::StringNode(const Token &token):
    token(token)
{
    setRange(token.getRange());
}

const Token &StringNode::getToken() const
{
    return token;
}

const string &StringNode::getValue() const
{
    return token.getValue();
}

void StringNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}