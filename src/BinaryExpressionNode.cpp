//**************************************************
// File: BinaryExpressionNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the BinaryExpressionNode class,
// representing binary expressions in the AST.
//**************************************************

#include "BinaryExpressionNode.h"

BinaryExpressionNode::BinaryExpressionNode(ExpressionNode *left, OpNode *op, ExpressionNode *right)
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

ExpressionNode *BinaryExpressionNode::getLeft() const
{
    if (getChildCount() > 0)
    {
        return dynamic_cast<ExpressionNode*>(getChild(0));
    }
    return nullptr;
}

OpNode *BinaryExpressionNode::getOperator() const
{
    if (getChildCount() > 0)
    {
        return dynamic_cast<OpNode*>(getChild(1));
    }
    return nullptr;
}

ExpressionNode *BinaryExpressionNode::getRight() const
{
    if (getChildCount() > 1)
    {
        return dynamic_cast<ExpressionNode*>(getChild(2));
    }
    return nullptr;
}

bool BinaryExpressionNode::isUnary() const
{
    return getChildCount() == 1;
}

bool BinaryExpressionNode::isPrefix() const
{
    return isUnary() && getLeft() == nullptr;
}

void BinaryExpressionNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}