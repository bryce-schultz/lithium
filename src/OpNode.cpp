#include "OpNode.h"

OpNode::OpNode(Token op):
    op(op)
{
}

int OpNode::getType() const
{
    return op.getType();
}

void OpNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}