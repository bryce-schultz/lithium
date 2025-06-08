//***********************************************
// File: StringNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the StringNode class, which represents
// a string literal in the abstract syntax tree (AST).
//***********************************************

#pragma once

#include <memory>
#include <string>

#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

using std::string;
using std::shared_ptr;

class StringNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<StringNode>;
public:
    StringNode(const Token &value);

    const Token &getToken() const;

    const string &getValue() const;

    void visit(Visitor *visitor) override;
private:
    Token token;
};

using StringNodePtr = shared_ptr<StringNode>;