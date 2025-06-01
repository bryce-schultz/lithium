#include "WhileNode.h"

WhileNode::WhileNode(ExpressionNode *condition, StatementNode *body)
{
    addChild(condition);
    addChild(body);
}

ExpressionNode *WhileNode::getCondition() const
{
    return static_cast<ExpressionNode *>(getChild(0));
}

StatementNode *WhileNode::getBody() const
{
    return static_cast<StatementNode *>(getChild(1));
}

void WhileNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}