//***********************************************
// File: IfStatementNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the IfStatementNode class,
// which represents if statements in the abstract syntax tree (AST).
//**************************************************

#include <memory>

#include "IfStatementNode.h"

IfStatementNode::IfStatementNode(shared_ptr<ExpressionNode> condition, shared_ptr<StatementNode> thenBranch, shared_ptr<StatementNode> elseBranch):
    condition(condition),
    thenBranch(thenBranch),
    elseBranch(elseBranch)
{
    if (condition)
    {
        setRangeStart(condition->getRange().getStart());
    }
    if (thenBranch)
    {
        setRangeEnd(thenBranch->getRange().getEnd());
    }
    if (elseBranch)
    {
        setRangeEnd(elseBranch->getRange().getEnd());
    }
}

shared_ptr<ExpressionNode> IfStatementNode::getCondition() const
{
    return condition;
}

shared_ptr<StatementNode> IfStatementNode::getThenBranch() const
{
    return thenBranch;
}

shared_ptr<StatementNode> IfStatementNode::getElseBranch() const
{
    return elseBranch;
}

void IfStatementNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}