//**************************************************
// file: BinaryExprNode.cpp
//
// author: Bryce Schultz
//
// purpose: implements the BinaryExprNode class,
// representing binary expressions in the AST.
//**************************************************

#include "BinaryExprNode.h"

BinaryExprNode::BinaryExprNode(ExpressionNode *left, OpNode *op, ExpressionNode *right)
{
    setRange(op->getRange());

    addChild(left);
    if (left)
        setRangeStart(left->getRange().getStart());

    addChild(op);

    addChild(right);
    if (right)
        setRangeEnd(right->getRange().getEnd());
}

ExpressionNode *BinaryExprNode::getLeft() const
{
    if (getChildCount() > 0)
    {
        return dynamic_cast<ExpressionNode*>(getChild(0));
    }
    return nullptr;
}

OpNode *BinaryExprNode::getOperator() const
{
    if (getChildCount() > 1)
    {
        return dynamic_cast<OpNode*>(getChild(1));
    }
    return nullptr;
}

ExpressionNode *BinaryExprNode::getRight() const
{
    if (getChildCount() > 2)
    {
        return dynamic_cast<ExpressionNode*>(getChild(2));
    }
    return nullptr;
}

bool BinaryExprNode::isUnary() const
{
    return getLeft() == nullptr || getRight() == nullptr;
}

bool BinaryExprNode::isPrefix() const
{
    return getRight() != nullptr && getLeft() == nullptr;
}

void BinaryExprNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}