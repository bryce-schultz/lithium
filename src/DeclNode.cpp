#include "DeclNode.h"

DeclNode::DeclNode()
{
}

void DeclNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}