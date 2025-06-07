//**************************************************
// file: BinaryExprNode.cpp
//
// author: Bryce Schultz
//
// purpose: implements the BinaryExprNode class,
// representing binary expressions in the AST.
//**************************************************

#include "BinaryExprNode.h"

BinaryExprNode::BinaryExprNode(shared_ptr<ExpressionNode> left, shared_ptr<OpNode> op, shared_ptr<ExpressionNode> right):
    left(left),
    op(op),
    right(right)
{
    if (left && right)
    {
        setRange(left->getRange().getStart(), right->getRange().getEnd());
    }
    else if (left)
    {
        setRangeStart(left->getRange().getStart());
        setRangeEnd(op->getRange().getEnd());
    }
    else if (right)
    {
        setRangeStart(op->getRange().getStart());
        setRangeEnd(right->getRange().getEnd());
    }
    else
    {
        setRangeStart(op->getRange().getStart());
        setRangeEnd(op->getRange().getEnd());
    }
}

shared_ptr<ExpressionNode> BinaryExprNode::getLeft() const
{
    return left;
}

shared_ptr<OpNode> BinaryExprNode::getOperator() const
{
    return op;
}

shared_ptr<ExpressionNode> BinaryExprNode::getRight() const
{
    return right;
}

bool BinaryExprNode::isUnary() const
{
    return !left || !right;
}

bool BinaryExprNode::isPrefix() const
{
    // left op right
    //      ++ num   -> prefix
    //  num ++       -> postfix
    return (!left && right);
}

void BinaryExprNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}