//**************************************************
// File: ArrayAccessNode.h
//
// Author: Bryce Schultz
//
//**************************************************
#pragma once

#include <memory>

#include "ExpressionNode.h"
#include "Token.h"

using std::shared_ptr;
using std::string;

class ArrayAccessNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<ArrayAccessNode>;
public:
    ArrayAccessNode(shared_ptr<ExpressionNode> arr, shared_ptr<ExpressionNode> index);

    shared_ptr<ExpressionNode> getArray() const;

    shared_ptr<ExpressionNode> getIndex() const;

    bool isLval() const override;

    void visit(Visitor *visitor) override;
private:
    shared_ptr<ExpressionNode> arr;
    shared_ptr<ExpressionNode> index;
};

using ArrayAccessNodePtr = shared_ptr<ArrayAccessNode>;