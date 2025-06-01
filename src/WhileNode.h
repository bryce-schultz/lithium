#pragma once

#include "Node.h"
#include "ExpressionNode.h"
#include "StatementNode.h"

class WhileNode : public StatementNode
{
public:
    WhileNode(ExpressionNode *condition, StatementNode *body);

    ExpressionNode *getCondition() const;

    StatementNode *getBody() const;

    void visit(Visitor *visitor) override;
};