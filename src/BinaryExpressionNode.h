#pragma once

#include "ExpressionNode.h"
#include "OpNode.h"
#include "Visitor.h"
#include "Token.h"

class BinaryExpressionNode : public ExpressionNode
{
public:
    BinaryExpressionNode(ExpressionNode *left, OpNode *op, ExpressionNode *right);

    OpNode *getOperator() const;

    ExpressionNode *getLeft() const;
    ExpressionNode *getRight() const;

    bool isUnary() const;
    bool isPrefix() const;

    virtual void visit(Visitor *visitor) override;
};