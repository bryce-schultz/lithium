#include "NullNode.h"

void NullNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}