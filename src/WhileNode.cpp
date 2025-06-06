//***********************************************
// File: WhileNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the WhileNode class, which represents
// while loops in the abstract syntax tree (AST).
//**************************************************

#include <memory>

#include "WhileNode.h"
#include "Visitor.h"

WhileNode::WhileNode(shared_ptr<ExpressionNode> condition, shared_ptr<StatementNode> body):
    condition(condition),
    body(body)
{
    if (condition)
    {
        setRangeStart(condition->getRange().getStart());
    }
    if (body)
    {
        setRangeEnd(body->getRange().getEnd());
    }
}

shared_ptr<ExpressionNode> WhileNode::getCondition() const
{
    return condition;
}

shared_ptr<StatementNode> WhileNode::getBody() const
{
    return body;
}

void WhileNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}