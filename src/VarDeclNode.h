#pragma once

#include "StatementNode.h"
#include "Token.h"
#include "ExpressionNode.h"
#include "Visitor.h"

class VarDeclNode : public StatementNode
{
public:
    VarDeclNode(const Token &token, ExpressionNode *expr = nullptr);

    const Token &getToken() const;

    ExpressionNode *getExpr() const;

    virtual void visit(Visitor *visitor) override;
private:
    Token token;
};