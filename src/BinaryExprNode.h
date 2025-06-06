//**************************************************
// File: BinaryExpressionNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the BinaryExpressionNode class,
// representing binary expressions in the AST.
//**************************************************

#pragma once

#include <memory>

#include "ExpressionNode.h"
#include "OpNode.h"
#include "Visitor.h"
#include "Token.h"

using std::shared_ptr;
using std::string;

class BinaryExprNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<BinaryExprNode>;
public:
    BinaryExprNode(shared_ptr<ExpressionNode> left, shared_ptr<OpNode> op, shared_ptr<ExpressionNode> right);
    shared_ptr<ExpressionNode> getLeft() const;
    shared_ptr<OpNode> getOperator() const;
    shared_ptr<ExpressionNode> getRight() const;
    bool isUnary() const;
    bool isPrefix() const;
    void visit(Visitor *visitor) override;
private:
    shared_ptr<ExpressionNode> left;
    shared_ptr<OpNode> op;
    shared_ptr<ExpressionNode> right;
    bool unary;
    bool prefix;
};

using BinaryExprNodePtr = shared_ptr<BinaryExprNode>;