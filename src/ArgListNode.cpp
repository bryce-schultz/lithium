//**************************************************
// File: ArgListNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the ArgListNode class, which
// manages argument lists for function calls in the AST.
//**************************************************

#include "ArgListNode.h"

ArgListNode::ArgListNode(shared_ptr<ExpressionNode> arg):
    args()
{
    if (arg)
    {
        addArg(arg);
    }
}

void ArgListNode::addArg(shared_ptr<ExpressionNode> arg)
{
    if (arg)
    {
        args.emplace_back(arg);
        if (args.size() == 1)
        {
            setRangeStart(arg->getRange().getStart());
        }
        setRangeEnd(arg->getRange().getEnd());
    }
}

void ArgListNode::addAllArgs(shared_ptr<ArgListNode> other)
{
    if (other)
    {
        for (const auto &arg : other->args)
        {
            addArg(arg);
        }
    }
}

shared_ptr<ExpressionNode> ArgListNode::getArg(int index) const
{
    if (index < 0 || index >= static_cast<int>(args.size()))
    {
        return nullptr;
    }
    return args[index];
}

int ArgListNode::getArgCount() const
{
    return static_cast<int>(args.size());
}

const vector<shared_ptr<ExpressionNode>> &ArgListNode::getArgs() const
{
    return args;
}

void ArgListNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}
