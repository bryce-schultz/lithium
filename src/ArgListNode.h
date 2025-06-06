//**************************************************
// File: ArgListNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the ArgListNode class, which
// represents a list of arguments in function calls.
//**************************************************

#pragma once

#include <memory>
#include <vector>

#include "ExpressionNode.h"

using std::shared_ptr;
using std::vector;

class ArgListNode : public Node
{
public:
    using Ptr = shared_ptr<ArgListNode>;
public:
    ArgListNode(shared_ptr<ExpressionNode> arg = nullptr);

    void addArg(shared_ptr<ExpressionNode> arg);

    void addAllArgs(shared_ptr<ArgListNode> other);

    shared_ptr<ExpressionNode> getArg(int index) const;

    const vector<shared_ptr<ExpressionNode>> &getArgs() const;

    int getArgCount() const;

    void visit(Visitor *visitor) override;
private:
    vector<shared_ptr<ExpressionNode>> args;
};