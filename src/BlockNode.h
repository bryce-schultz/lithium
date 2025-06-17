//**************************************************
// File: BlockNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the BlockNode class, which
// represents a block of statements in the AST.
//**************************************************

#pragma once

#include <memory>

#include "StatementNode.h"
#include "StatementsNode.h"

using std::shared_ptr;

class BlockNode : public StatementNode
{
public:
    using Ptr = shared_ptr<BlockNode>;
public:
    BlockNode(shared_ptr<StatementsNode> statements = nullptr);

    shared_ptr<StatementsNode> getStatements() const;

    void addStatement(shared_ptr<StatementNode> statement);

    virtual void visit(Visitor *visitor) override;
private:
    shared_ptr<StatementsNode> statements;
};

using BlockNodePtr = shared_ptr<BlockNode>;