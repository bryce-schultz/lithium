#pragma once

#include "StatementNode.h"
#include "Visitor.h"

class ExpressionNode : public StatementNode
{
public:
    virtual void visit(Visitor *visitor) override
    {
        visitor->visit(this);
    }
};