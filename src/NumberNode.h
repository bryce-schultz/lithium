#pragma once

#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

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