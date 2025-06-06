//**************************************************
// File: MemberAccessNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the MemberAccessNode class, which
// represents member access expressions in the AST.
//**************************************************

#include <memory>
#include <string>

#include "MemberAccessNode.h"

MemberAccessNode::MemberAccessNode(shared_ptr<ExpressionNode> expression, const Token &identifier): 
    expression(expression), identifier(identifier)
{
    if (this->expression)
    {
        setRangeStart(this->expression->getRange().getStart());
    }
    setRangeEnd(identifier.getRange().getEnd());
}

shared_ptr<ExpressionNode> MemberAccessNode::getExpression() const
{
    return expression;
}

const Token &MemberAccessNode::getIdentifier() const
{
    return identifier;
}

void MemberAccessNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}