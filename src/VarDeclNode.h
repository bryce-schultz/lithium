#pragma once

#include "DeclNode.h"
#include "Token.h"
#include "ExpressionNode.h"
#include "Visitor.h"
#include <memory>

class VarDeclNode : public DeclNode
{
public:
    using Ptr = std::shared_ptr<VarDeclNode>;
public:
    VarDeclNode(const Token &token, std::shared_ptr<ExpressionNode> expr = nullptr, bool isConst = false);

    const Token &getToken() const;

    virtual const string &getName() const override;

    std::shared_ptr<ExpressionNode> getExpr() const;

    virtual bool isConst() const override;

    virtual void visit(Visitor *visitor) override;
private:
    Token token;
    bool constFlag;
    std::shared_ptr<ExpressionNode> expr;
};

using VarDeclNodePtr = std::shared_ptr<VarDeclNode>;