#pragma once

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "Visitor.h"

class ReturnStatementNode : public StatementNode
{
public:
    ReturnStatementNode(ExpressionNode *expr = nullptr)
    {
        if (expr)
        {
            addChild(expr);
        }
    }

    void setExpression(ExpressionNode *expr)
    {
        if (expr)
        {
            clearChildren(); // remove any existing children
            addChild(expr);
        }
    }

    ExpressionNode *getExpression() const
    {
        if (getChildCount() > 0)
        {
            return static_cast<ExpressionNode *>(getChild(0));
        }
        return nullptr;
    }

    void visit(Visitor *visitor) override
    {
        visitor->visit(this);
    }
};