#include "BooleanNode.h"

BooleanNode::BooleanNode(const Token &token) : token(token)
{
    setRange(token.getRange());
}

const Token &BooleanNode::getToken() const
{
    return token;
}

bool BooleanNode::getValue() const
{
    return token.getValue() == "true";
}

void BooleanNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}
