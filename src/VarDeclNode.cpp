#include "VarDeclNode.h"

VarDeclNode::VarDeclNode(const Token &token, ExpressionNode *expr):
    token(token)
{
    setRange(token.getRange());
    if (expr)
    {
        addChild(expr);
        setRangeEnd(expr->getRange().getEnd());
    }
}

const Token &VarDeclNode::getToken() const
{
    return token;
}

ExpressionNode *VarDeclNode::getExpr() const
{
    if (getChildCount() > 0)
    {
        return dynamic_cast<ExpressionNode*>(getChild(0));
    }
    return nullptr;
}

void VarDeclNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}