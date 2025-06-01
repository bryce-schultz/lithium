#include "CallNode.h"

CallNode::CallNode(ExpressionNode *callee, ArgListNode *args)
{

    addChild(callee);
    if (callee)
        setRangeStart(callee->getRange().getStart());

    addChild(args);
    if (args)
        setRangeEnd(args->getRange().getEnd());
}

ExpressionNode *CallNode::getCallee() const
{
    if (getChildCount() > 0)
    {
        return dynamic_cast<ExpressionNode*>(getChild(0));
    }
    return nullptr;
}

ArgListNode *CallNode::getArgs() const
{
    if (getChildCount() > 1)
    {
        return dynamic_cast<ArgListNode*>(getChild(1));
    }
    return nullptr;
}

void CallNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}