#include "ArrayAccessNode.h"

ArrayAccessNode::ArrayAccessNode(shared_ptr<ExpressionNode> arr, shared_ptr<ExpressionNode> index):
    arr(arr),
    index(index)
{
    if (arr)
    {
        setRangeStart(arr->getRange().getStart());
    }
    if (index)
    {
        setRangeEnd(index->getRange().getEnd());
    }
}

shared_ptr<ExpressionNode> ArrayAccessNode::getArray() const
{
    return arr;
}

shared_ptr<ExpressionNode> ArrayAccessNode::getIndex() const
{
    return index;
}

bool ArrayAccessNode::isLval() const
{
    return true;
}

void ArrayAccessNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}

