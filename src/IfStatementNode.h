#pragma once

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

class IfStatementNode : public StatementNode
{
public:
    IfStatementNode(ExpressionNode *condition, StatementNode *thenBranch, StatementNode *elseBranch = nullptr);

    ExpressionNode *getCondition() const;
    StatementNode *getThenBranch() const;
    StatementNode *getElseBranch() const;

    virtual void visit(Visitor *visitor) override;
};