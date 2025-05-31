//**************************************************
// File: StatementsNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the StatementsNode class,
// representing a sequence of statements in the AST.
//**************************************************

#pragma once

#include "Node.h"
#include "StatementNode.h"
#include "Visitor.h"

class StatementsNode : public Node
{
public:
    StatementsNode(StatementNode *statement = nullptr);

    void addStatement(StatementNode *statement);

    void visit(Visitor *visitor) override;
};