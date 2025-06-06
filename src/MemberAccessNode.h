//**************************************************
// File: MemberAccessNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the MemberAccessNode class, which
// represents member access expressions in the AST.
//**************************************************
#pragma once

#include <memory>

#include "ExpressionNode.h"
#include "Token.h"

using std::shared_ptr;
using std::string;

class MemberAccessNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<MemberAccessNode>;
public:
    MemberAccessNode(shared_ptr<ExpressionNode> expression, const Token &identifier);

    shared_ptr<ExpressionNode> getExpression() const;

    const Token &getIdentifier() const;

    void visit(Visitor *visitor) override;
private:
    shared_ptr<ExpressionNode> expression;
    Token identifier;
};

using MemberAccessNodePtr = shared_ptr<MemberAccessNode>;