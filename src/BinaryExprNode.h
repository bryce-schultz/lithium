//**************************************************
// File: BinaryExpressionNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the BinaryExpressionNode class,
// representing binary expressions in the AST.
//**************************************************

#pragma once

#include "ExpressionNode.h"
#include "OpNode.h"
#include "Visitor.h"
#include "Token.h"

class BinaryExprNode : public ExpressionNode
{
public:
    BinaryExprNode(ExpressionNode *left, OpNode *op, ExpressionNode *right);

    OpNode *getOperator() const;

    ExpressionNode *getLeft() const;
    ExpressionNode *getRight() const;

    bool isUnary() const;
    bool isPrefix() const;

    virtual void visit(Visitor *visitor) override;
};