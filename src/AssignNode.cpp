//***********************************************
// File: AssignNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the AssignNode class, which represents
// an assignment operation in the abstract syntax tree (AST).
//***********************************************

#include <memory>
#include <string>

#include "AssignNode.h"
#include "Visitor.h"

AssignNode::AssignNode(shared_ptr<ExpressionNode> asignee, shared_ptr<ExpressionNode> expr):
    asignee(asignee),
    expr(expr)
{
    if (asignee)
    {
        setRange(asignee->getRange());
    }
    if (expr)
    {
        setRangeEnd(expr->getRange().getEnd());
    }
}

bool AssignNode::isLval() const
{
    // an assignment node is an lvalue if its asignee is an lvalue
    return asignee && asignee->isLval();
}

shared_ptr<ExpressionNode> AssignNode::getAsignee() const
{
    return asignee;
}

shared_ptr<ExpressionNode> AssignNode::getExpr() const
{
    return expr;
}

void AssignNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}