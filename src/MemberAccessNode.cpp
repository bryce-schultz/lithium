//**************************************************
// File: MemberAccessNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the MemberAccessNode class, which
// represents member access expressions in the AST.
//**************************************************

#include "MemberAccessNode.h"

MemberAccessNode::MemberAccessNode(ExpressionNode *expression, const Token &identifier):
    identifier(identifier)
{
    addChild(expression);
    setRangeStart(expression->getRange().getStart());
    setRangeEnd(identifier.getRange().getEnd());
}

const Token &MemberAccessNode::getIdentifier() const
{
    return identifier;
}

ExpressionNode *MemberAccessNode::getExpression() const
{
    if (getChildCount() > 0)
    {
        return static_cast<ExpressionNode *>(getChild(0));
    }
    return nullptr;
}

void MemberAccessNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}