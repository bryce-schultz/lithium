//***********************************************
// File: PrintStatementNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the PrintStatementNode class, which
// represents print statements in the abstract syntax tree (AST).
//***********************************************

#pragma once

#include <memory>

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "Visitor.h"

using std::shared_ptr;

class PrintStatementNode : public StatementNode
{
public:
    using Ptr = shared_ptr<PrintStatementNode>;
public:
    PrintStatementNode(shared_ptr<ExpressionNode> expr = nullptr);

    shared_ptr<ExpressionNode> getExpr() const;

    void visit(Visitor *visitor) override;
private:
    shared_ptr<ExpressionNode> expr;
};

using PrintStatementNodePtr = shared_ptr<PrintStatementNode>;