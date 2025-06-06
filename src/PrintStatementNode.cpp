//***********************************************
// File: PrintStatementNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the PrintStatementNode class,
// which represents print statements in the abstract syntax tree (AST).
//***********************************************

#include <memory>

#include "PrintStatementNode.h"

PrintStatementNode::PrintStatementNode(shared_ptr<ExpressionNode> expr):
    expr(expr)
{
    if (expr)
    {
        setRange(expr->getRange());
    }
}

shared_ptr<ExpressionNode> PrintStatementNode::getExpr() const
{
    return expr;
}

void PrintStatementNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}
