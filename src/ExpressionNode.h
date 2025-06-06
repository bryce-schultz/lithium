//***********************************************
// File: ExpressionNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the ExpressionNode class, which
// represents an expression node in the abstract syntax tree (AST).
// ************************************************

#pragma once

#include <memory>
#include "StatementNode.h"
#include "Visitor.h"

using std::shared_ptr;

using ExpressionNodePtr = shared_ptr<ExpressionNode>;

class ExpressionNode : public StatementNode
{
public:
    using Ptr = shared_ptr<ExpressionNode>;
public:
    virtual bool isLval() const;
    bool isRval() const;

    virtual void visit(Visitor *visitor) override = 0;
};