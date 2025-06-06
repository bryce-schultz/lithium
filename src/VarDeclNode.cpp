#include "VarDeclNode.h"

VarDeclNode::VarDeclNode(const Token &token,
    std::shared_ptr<ExpressionNode> expr,
    bool isConst):
    token(token),
    constFlag(isConst),
    expr(std::move(expr))
{
    setRange(token.getRange());
    if (this->expr)
    {
        setRangeEnd(this->expr->getRange().getEnd());
    }
}

const Token &VarDeclNode::getToken() const
{
    return token;
}

const string &VarDeclNode::getName() const
{
    return token.getValue();
}

std::shared_ptr<ExpressionNode> VarDeclNode::getExpr() const
{
    return expr;
}

bool VarDeclNode::isConst() const
{
    return constFlag;
}

void VarDeclNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}