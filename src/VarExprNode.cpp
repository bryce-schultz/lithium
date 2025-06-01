#include "VarExprNode.h"

VarExprNode::VarExprNode(const Token &token):
    token(token)
{
}

const Token &VarExprNode::getToken() const
{
    return token;
}

string VarExprNode::getName() const
{
    return token.getValue();
}

bool VarExprNode::isVariable() const
{
    return true; // This node represents a variable expression
}

void VarExprNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}