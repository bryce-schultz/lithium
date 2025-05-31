#pragma once

#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

class VarExprNode : public ExpressionNode
{
public:
    VarExprNode(const Token &token);

    const Token &getToken() const;

    void visit(Visitor *visitor) override;
private:
    Token token;
};