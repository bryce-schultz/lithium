#include "AssertNode.h"

AssertNode::AssertNode(shared_ptr<ExpressionNode> condition, shared_ptr<ExpressionNode> message):
    condition(std::move(condition)),
    message(std::move(message))
{
    if (this->condition)
    {
        setRange(this->condition->getRange());
    }

    if (this->message)
    {
        setRangeEnd(this->message->getRange().getEnd());
    }
}

shared_ptr<ExpressionNode> AssertNode::getCondition() const
{
    return condition;
}

shared_ptr<ExpressionNode> AssertNode::getMessage() const
{
    return message;
}

void AssertNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}
