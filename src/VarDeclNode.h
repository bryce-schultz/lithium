#pragma once

#include "DeclNode.h"
#include "Token.h"
#include "ExpressionNode.h"
#include "Visitor.h"

class VarDeclNode : public DeclNode
{
public:
    VarDeclNode(const Token &token, ExpressionNode *expr = nullptr, bool isConst = false);

    const Token &getToken() const;

    virtual string getName() const override;

    virtual bool isConst() const override;

    ExpressionNode *getExpr() const;

    virtual void visit(Visitor *visitor) override;
private:
    Token token;
    bool constFlag;
};