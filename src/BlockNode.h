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