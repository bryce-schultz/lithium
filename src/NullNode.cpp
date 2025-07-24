#include "NullNode.h"

NullNode::NullNode(Range range)
{
    setRange(range);
}

void NullNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}