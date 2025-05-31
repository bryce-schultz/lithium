#include "PrintStatementNode.h"

PrintStatementNode::PrintStatementNode(ExpressionNode *expr)
{
    if (expr)
    {
        addChild(expr);
        setRange(expr->getRange());
    }
}

ExpressionNode *PrintStatementNode::getExpr() const
{
    if (getChildCount() > 0)
    {
        return dynamic_cast<ExpressionNode*>(getChild(0));
    }
    return nullptr;
}

void PrintStatementNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}
