//**************************************************
// File: StatementsNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the StatementsNode class,
// representing a sequence of statements in the AST.
//**************************************************

#pragma once

#include <memory>
#include <vector>

#include "Node.h"
#include "StatementNode.h"
#include "Visitor.h"

using std::shared_ptr;
using std::vector;

class StatementsNode : public Node
{
public:
    using Ptr = shared_ptr<StatementsNode>;
public:
    StatementsNode(shared_ptr<StatementNode> statement = nullptr);

    void addStatement(shared_ptr<StatementNode> statement);

    shared_ptr<StatementNode> getStatement(int index) const;

    int getStatementCount() const;

    const vector<shared_ptr<StatementNode>> &getStatements() const;

    void visit(Visitor *visitor) override;
private:
    vector<shared_ptr<StatementNode>> statements;
};