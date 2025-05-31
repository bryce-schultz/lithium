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

void CallNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}