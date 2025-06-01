#pragma once

#include <string>

#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

class VarExprNode : public ExpressionNode
{
public:
    VarExprNode(const Token &token);

    const Token &getToken() const;

    string getName() const;

    virtual bool isVariable() const override;

    void visit(Visitor *visitor) override;
private:
    Token token;
};