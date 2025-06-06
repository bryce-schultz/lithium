//***********************************************
// DeclNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the DeclNode class, which represents
// a declaration node in the abstract syntax tree (AST).
//***********************************************

#pragma once

#include <memory>

#include "StatementNode.h"
#include "Visitor.h"
#include "Token.h"

using std::string;
using std::shared_ptr;

class DeclNode : public StatementNode
{
public:
    using Ptr = shared_ptr<DeclNode>;
public:
    virtual const Token &getToken() const = 0;

    virtual const string &getName() const = 0;

    virtual bool isConst() const = 0;

    virtual void visit(Visitor *visitor) override = 0;
};

using DeclNodePtr = shared_ptr<DeclNode>;