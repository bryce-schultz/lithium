//**************************************************
// File: OpNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the OpNode class, representing
// operator nodes in the AST for expressions.
//**************************************************

#include "OpNode.h"

OpNode::OpNode(Token op):
    op(op)
{
    setRange(op.getRange());
}

int OpNode::getType() const
{
    return op.getType();
}

const Token &OpNode::getToken() const
{
    return op;
}

void OpNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}