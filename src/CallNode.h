//***********************************************
// File: CallNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the CallNode class, which represents
// a function call in the abstract syntax tree (AST).
//************************************************

#pragma once

#include <memory>

#include "ArgListNode.h"
#include "ExpressionNode.h"
#include "Visitor.h"

using std::shared_ptr;

class CallNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<CallNode>;
public:
    CallNode(shared_ptr<ExpressionNode> callee, shared_ptr<ArgListNode> args = nullptr);

    shared_ptr<ExpressionNode> getCallee() const;

    shared_ptr<ArgListNode> getArgs() const;

    void visit(Visitor *visitor) override;
private:
    shared_ptr<ExpressionNode> callee;
    shared_ptr<ArgListNode> args;
};

using CallNodePtr = shared_ptr<CallNode>;