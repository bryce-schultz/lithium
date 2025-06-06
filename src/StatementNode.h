//***********************************************
// File: StatementNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the StatementNode class, which
// represents a statement node in the abstract syntax tree (AST).
//***********************************************

#pragma once

#include <memory>

#include "Node.h"
#include "Visitor.h"

using std::shared_ptr;

class StatementNode : public Node
{
public:
    using Ptr = shared_ptr<StatementNode>;

    virtual void visit(Visitor *visitor) override = 0;
};

using StatementNodePtr = shared_ptr<StatementNode>;