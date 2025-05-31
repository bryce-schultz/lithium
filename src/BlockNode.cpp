#include "BlockNode.h"

BlockNode::BlockNode(StatementsNode *statements)
{
    if (statements)
    {
        addChild(statements);
        setRange(statements->getRange());
    }
    else
    {
        setRange(Range(Location(1, 1), Location(1, 1))); // Default empty range
    }
}

void BlockNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}