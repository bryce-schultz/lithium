//**************************************************
// File: MemberAccessNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the MemberAccessNode class, which
// represents member access expressions in the AST.
//**************************************************
#pragma once

#include "ExpressionNode.h"
#include "Token.h"

class MemberAccessNode : public ExpressionNode
{
public:
    MemberAccessNode(ExpressionNode *expression, const Token &identifier);

    ExpressionNode *getExpression() const;

    const Token &getIdentifier() const;

    void visit(Visitor *visitor) override;
private:
    Token identifier;
};