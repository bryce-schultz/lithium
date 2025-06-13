#include "ContinueNode.h"

ContinueNode::ContinueNode(const Token &token)
{
    setRange(token.getRange());
}

void ContinueNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}