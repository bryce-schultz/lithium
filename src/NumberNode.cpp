//***********************************************
// File: NumberNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the NumberNode class, which represents
// a numeric literal in the abstract syntax tree (AST).
//***********************************************

#include "NumberNode.h"

NumberNode::NumberNode(const Token &token):
    token(token),
    value(std::stod(token.getValue()))
{
    setRange(token.getRange());
}

const Token &NumberNode::getToken() const
{
    return token;
}

double NumberNode::getValue() const
{
    return value;
}

void NumberNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}