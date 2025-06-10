#include "ImportNode.h"

ImportNode::ImportNode(const Token &token):
    token(token)
{
    setRange(token.getRange());
}

const Token &ImportNode::getToken() const
{
    return token;
}

const std::string &ImportNode::getModuleName() const
{
    return token.getValue();
}

void ImportNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}