#pragma once

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "Visitor.h"

class PrintStatementNode : public StatementNode
{
public:
    PrintStatementNode(ExpressionNode *expr = nullptr);

    ExpressionNode *getExpr() const;

    virtual void visit(Visitor *visitor) override;
};