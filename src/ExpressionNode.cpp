//***********************************************
// File: ExpressionNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the ExpressionNode class, which
// represents an expression node in the abstract syntax tree (AST).
//**************************************************

#include "ExpressionNode.h"

bool ExpressionNode::isLval() const
{
    return false;
}

bool ExpressionNode::isRval() const
{
    return !isLval();
}

void ExpressionNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}