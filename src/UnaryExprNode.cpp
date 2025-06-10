#include "UnaryExprNode.h"

UnaryExprNode::UnaryExprNode(shared_ptr<ExpressionNode> expr, shared_ptr<OpNode> op, bool prefix):
    expr(expr),
    op(op),
    prefix(prefix)
{
    if (prefix)
    {
        setRangeStart(op->getRange().getStart());
        setRangeEnd(expr->getRange().getEnd());
    }
    else
    {
        setRangeStart(expr->getRange().getStart());
        setRangeEnd(op->getRange().getEnd());
    }
}

shared_ptr<OpNode> UnaryExprNode::getOperator() const
{
    return op;
}

shared_ptr<ExpressionNode> UnaryExprNode::getExpression() const
{
    return expr;
}

bool UnaryExprNode::isPrefix() const
{
    return prefix;
}

void UnaryExprNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}