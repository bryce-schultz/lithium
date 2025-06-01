#include "VarDeclNode.h"

VarDeclNode::VarDeclNode(const Token &token, ExpressionNode *expr, bool isConst):
    token(token)
{
    constFlag = isConst;
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

string VarDeclNode::getName() const
{
    return token.getValue();
}

bool VarDeclNode::isConst() const
{
    return constFlag;
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