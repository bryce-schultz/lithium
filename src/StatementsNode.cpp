//**************************************************
// File: StatementsNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the StatementsNode class,
// representing a sequence of statements in the AST.
//**************************************************

#include "StatementsNode.h"

StatementsNode::StatementsNode(shared_ptr<StatementNode> statement):
    statements()
{
    if (statement)
    {
        statements.push_back(statement);
        setRange(statement->getRange());
    }
}

void StatementsNode::addStatement(shared_ptr<StatementNode> statement)
{
    if (statement)
    {
        statements.push_back(statement);
        if (statements.size() == 1)
        {
            setRange(statement->getRange());
        }
        else
        {
            setRangeEnd(statement->getRange().getEnd());
        }
    }
}

shared_ptr<StatementNode> StatementsNode::getStatement(int index) const
{
    if (index < 0 || index >= static_cast<int>(statements.size()))
    {
        return nullptr;
    }
    return statements[index];
}

int StatementsNode::getStatementCount() const
{
    return static_cast<int>(statements.size());
}

const vector<shared_ptr<StatementNode>> &StatementsNode::getStatements() const
{
    return statements;
}

void StatementsNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}