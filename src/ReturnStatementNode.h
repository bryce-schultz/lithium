//***********************************************
// File: ReturnStatementNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the ReturnStatementNode class, which
// represents return statements in the abstract syntax tree (AST).
//***********************************************

#pragma once

#include <memory>

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "Visitor.h"

using std::shared_ptr;

class ReturnStatementNode : public StatementNode
{
public:
    using Ptr = shared_ptr<ReturnStatementNode>;
public:
    ReturnStatementNode(shared_ptr<ExpressionNode> expr = nullptr);

    void setExpression(shared_ptr<ExpressionNode> expr);

    shared_ptr<ExpressionNode> getExpression() const;

    void visit(Visitor *visitor) override;
private:
    shared_ptr<ExpressionNode> expression;
};

using ReturnStatementNodePtr = shared_ptr<ReturnStatementNode>;