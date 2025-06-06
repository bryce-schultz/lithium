//***********************************************
// File: VarExprNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the VarExprNode class, which represents
// a variable expression in the abstract syntax tree (AST).
//***********************************************

#pragma once

#include <memory>
#include <string>

#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

using std::string;
using std::shared_ptr;

using VarExprNodePtr = shared_ptr<VarExprNode>;

class VarExprNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<VarExprNode>;
public:
    VarExprNode(const Token &token);

    const Token &getToken() const;

    const std::string &getName() const;

    virtual bool isLval() const override;

    void visit(Visitor *visitor) override;
private:
    Token token;
};