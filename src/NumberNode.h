//***********************************************
// NumberNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the NumberNode class, which represents
// a numeric literal in the abstract syntax tree (AST).
//***********************************************

#pragma once

#include <memory>
#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

using NumberNodePtr = std::shared_ptr<NumberNode>;

class NumberNode : public ExpressionNode
{
public:
    NumberNode(const Token &token);

    const Token &getToken() const;

    double getValue() const;

    virtual void visit(Visitor *visitor) override;
private:
    Token token;
    double value;
};