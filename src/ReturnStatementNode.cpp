//***********************************************
// File: ReturnStatementNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the ReturnStatementNode class,
// which represents return statements in the AST.
//**************************************************

#include "ReturnStatementNode.h"
#include "Visitor.h"

ReturnStatementNode::ReturnStatementNode(shared_ptr<ExpressionNode> expr):
    expression(expr)
{
    if (expr)
    {
        setRange(expr->getRange());
    }
}

void ReturnStatementNode::setExpression(shared_ptr<ExpressionNode> expr)
{
    expression = expr;
    if (expr)
    {
        setRange(expr->getRange());
    }
}

shared_ptr<ExpressionNode> ReturnStatementNode::getExpression() const
{
    return expression;
}

void ReturnStatementNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}