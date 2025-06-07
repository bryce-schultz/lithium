#include "ForStatementNode.h"

ForStatementNode::ForStatementNode(shared_ptr<StatementNode> init,
    shared_ptr<StatementNode> condition,
    shared_ptr<ExpressionNode> increment,
    shared_ptr<StatementNode> body):
    init(init),
    condition(condition),
    increment(increment),
    body(body)
{
    if (init)
    {
        setRangeStart(init->getRange().getStart());
    }
    if (condition)
    {
        setRangeEnd(condition->getRange().getEnd());
    }
    if (increment)
    {
        setRangeEnd(increment->getRange().getEnd());
    }
    if (body)
    {
        setRangeEnd(body->getRange().getEnd());
    }
}

shared_ptr<StatementNode> ForStatementNode::getInit() const
{
    return init;
}

shared_ptr<StatementNode> ForStatementNode::getCondition() const
{
    return condition;
}

shared_ptr<ExpressionNode> ForStatementNode::getIncrement() const
{
    return increment;
}

shared_ptr<StatementNode> ForStatementNode::getBody() const
{
    return body;
}

void ForStatementNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}