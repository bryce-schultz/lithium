//**************************************************
// File: ArgListNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the ArgListNode class, which
// manages argument lists for function calls in the AST.
//**************************************************

#include "ArgListNode.h"

ArgListNode::ArgListNode(ExpressionNode *arg)
{
    addArg(arg);
    if (arg)
    {
        setRangeStart(arg->getRange().getStart());
    }
}

void ArgListNode::addArg(ExpressionNode *arg)
{
    if (arg)
    {
        addChild(arg);
        setRangeEnd(arg->getRange().getEnd());
    }
}

void ArgListNode::addAllArgs(ArgListNode *args)
{
    if (args)
    {
        for (int i = 0; i < args->getArgCount(); ++i)
        {
            ExpressionNode *arg = args->getArg(i);
            if (arg)
            {
                addArg(arg);
                setRangeEnd(arg->getRange().getEnd());
            }
        }
    }
}

ExpressionNode *ArgListNode::getArg(int index) const
{
    if (index < 0 || index >= getChildCount())
    {
        return nullptr;
    }
    return dynamic_cast<ExpressionNode*>(getChild(index));
}

int ArgListNode::getArgCount() const
{
    return getChildCount();
}

void ArgListNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}
