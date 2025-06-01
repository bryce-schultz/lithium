#include "StringNode.h"

StringNode::StringNode(const Token &token):
    token(token)
{
    setRange(token.getRange());
}

const string &StringNode::getValue() const
{
    return token.getValue();
}

void StringNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}