#include "BreakNode.h"

BreakNode::BreakNode(const Token &token)
{
    setRange(token.getRange());
}

void BreakNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}