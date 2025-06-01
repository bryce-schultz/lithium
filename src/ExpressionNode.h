#pragma once

#include "StatementNode.h"
#include "Visitor.h"

class ExpressionNode : public StatementNode
{
public:
    virtual bool isVariable() const;

    virtual void visit(Visitor *visitor) override;
};