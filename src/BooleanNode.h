#pragma once

#include <memory>

#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

using std::shared_ptr;

class BooleanNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<BooleanNode>;
public:
    BooleanNode(const Token &token);

    const Token &getToken() const;
    bool getValue() const;

    void visit(Visitor *visitor) override;
private:
    Token token;
};

using BooleanExprNodePtr = shared_ptr<BooleanNode>;
