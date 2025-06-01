#include "ExpressionNode.h"

bool ExpressionNode::isVariable() const
{
    return false;
}

void ExpressionNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}