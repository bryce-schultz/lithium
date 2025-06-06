//***********************************************
// File: CallNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the CallNode class, which represents
// a function call in the abstract syntax tree (AST).
//***********************************************

#include "CallNode.h"

CallNode::CallNode(shared_ptr<ExpressionNode> callee, shared_ptr<ArgListNode> args):
    callee(callee),
    args(args)
{
    if (callee)
    {
        setRangeStart(callee->getRange().getStart());
    }
    if (args)
    {
        setRangeEnd(args->getRange().getEnd());
    }
}

shared_ptr<ExpressionNode> CallNode::getCallee() const
{
    return callee;
}

shared_ptr<ArgListNode> CallNode::getArgs() const
{
    return args;
}

void CallNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}