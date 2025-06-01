#include "AssignNode.h"

AssignNode::AssignNode(ExpressionNode *asignee, ExpressionNode *expr)
{
    if (asignee)
    {
        setRangeStart(asignee->getRange().getStart());
        addChild(asignee);
    }
    if (expr)
    {
        addChild(expr);
        setRangeEnd(expr->getRange().getEnd());
    }
}

bool AssignNode::isVariable() const
{
    return true; // This node represents a variable assignment
}

ExpressionNode *AssignNode::getAsignee() const
{
    if (getChildCount() > 0)
    {
        return dynamic_cast<ExpressionNode*>(getChild(0));
    }
    return nullptr;
}

ExpressionNode *AssignNode::getExpr() const
{
    if (getChildCount() > 1)
    {
        return dynamic_cast<ExpressionNode*>(getChild(1));
    }
    return nullptr;
}

void AssignNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}