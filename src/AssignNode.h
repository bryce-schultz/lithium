//***********************************************
// File: AssignNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the AssignNode class, which represents
// an assignment operation in the abstract syntax tree (AST).
//***********************************************

#pragma once

#include <memory>
#include <string>

#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

using std::string;
using std::shared_ptr;

class AssignNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<AssignNode>;
public:
    AssignNode(shared_ptr<ExpressionNode> asignee, const Token &opToken, shared_ptr<ExpressionNode> expr = nullptr);

    virtual bool isLval() const override;

    shared_ptr<ExpressionNode> getAsignee() const;
    shared_ptr<ExpressionNode> getExpr() const;

    int getOp() const;

    virtual void visit(Visitor *visitor) override;

private:
    shared_ptr<ExpressionNode> asignee;
    Token opToken;
    shared_ptr<ExpressionNode> expr;
};