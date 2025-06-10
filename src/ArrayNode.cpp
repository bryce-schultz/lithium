#include "ArrayNode.h"

ArrayNode::ArrayNode(const vector<shared_ptr<ExpressionNode>> &elements):
    elements(elements)
{
    if (!elements.empty())
    {
        setRangeStart(elements.front()->getRange().getStart());
        setRangeEnd(elements.back()->getRange().getEnd());
    }
}

const vector<shared_ptr<ExpressionNode>> &ArrayNode::getElements() const
{
    return elements;
}

bool ArrayNode::isLval() const
{
    return true; // Arrays are lvalues
}

void ArrayNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}