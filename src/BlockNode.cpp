//**************************************************
// File: BlockNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the BlockNode class, which
// represents a block of statements in the AST.
//**************************************************

#include "BlockNode.h"

BlockNode::BlockNode(shared_ptr<StatementsNode> statements):
    statements(statements)
{
    if (statements)
    {
        setRange(statements->getRange());
    }
}

shared_ptr<StatementsNode> BlockNode::getStatements() const
{
    return statements;
}

void BlockNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}