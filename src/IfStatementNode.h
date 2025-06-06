//***********************************************
// File: IfStatementNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the IfStatementNode class, which
// represents if statements in the abstract syntax tree (AST).
//**************************************************

#pragma once

#include <memory>

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

using std::shared_ptr;

class IfStatementNode : public StatementNode
{
public:
    using Ptr = shared_ptr<IfStatementNode>;
public:
    IfStatementNode(shared_ptr<ExpressionNode> condition, shared_ptr<StatementNode> thenBranch, shared_ptr<StatementNode> elseBranch = nullptr);

    shared_ptr<ExpressionNode> getCondition() const;

    shared_ptr<StatementNode> getThenBranch() const;

    shared_ptr<StatementNode> getElseBranch() const;

    virtual void visit(Visitor *visitor) override;
private:
    shared_ptr<ExpressionNode> condition;
    shared_ptr<StatementNode> thenBranch;
    shared_ptr<StatementNode> elseBranch;
};

using IfStatementNodePtr = shared_ptr<IfStatementNode>;