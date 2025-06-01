#include "IfStatementNode.h"

IfStatementNode::IfStatementNode(ExpressionNode *condition, StatementNode *thenBranch, StatementNode *elseBranch)
{
    setRangeStart(condition->getRange().getStart());
    addChild(condition);

    setRangeEnd(thenBranch->getRange().getEnd());
    addChild(thenBranch);

    if (elseBranch)
    {
        addChild(elseBranch);
        setRangeEnd(elseBranch->getRange().getEnd());
    }
}

ExpressionNode *IfStatementNode::getCondition() const
{
    if (getChildCount() > 0)
    {
        return dynamic_cast<ExpressionNode*>(getChild(0));
    }
    return nullptr;
}

StatementNode *IfStatementNode::getThenBranch() const
{
    if (getChildCount() > 1)
    {
        return dynamic_cast<StatementNode*>(getChild(1));
    }
    return nullptr;
}

StatementNode *IfStatementNode::getElseBranch() const
{
    if (getChildCount() > 2)
    {
        return dynamic_cast<StatementNode*>(getChild(2));
    }
    return nullptr;
}

void IfStatementNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}