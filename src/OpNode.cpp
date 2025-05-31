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
}

int OpNode::getType() const
{
    return op.getType();
}

void OpNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}