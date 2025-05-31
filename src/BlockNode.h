//**************************************************
// File: BlockNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the BlockNode class, which
// represents a block of statements in the AST.
//**************************************************

#pragma once

#include "StatementNode.h"
#include "StatementsNode.h"

class BlockNode : public StatementNode
{
public:
    BlockNode(StatementsNode *statements = nullptr);

    // Visit method for the visitor pattern
    virtual void visit(Visitor *visitor) override;
};