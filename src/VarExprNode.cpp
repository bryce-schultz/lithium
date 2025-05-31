#include "VarExprNode.h"

VarExprNode::VarExprNode(const Token &token):
    token(token)
{
}

const Token &VarExprNode::getToken() const
{
    return token;
}

void VarExprNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}