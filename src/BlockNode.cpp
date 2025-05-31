//**************************************************
// File: BlockNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the BlockNode class, which
// represents a block of statements in the AST.
//**************************************************

#include "BlockNode.h"

BlockNode::BlockNode(StatementsNode *statements)
{
    if (statements)
    {
        addChild(statements);
        setRange(statements->getRange());
    }
}

void BlockNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}