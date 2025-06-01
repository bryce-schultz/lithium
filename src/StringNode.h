#pragma once

#include <string>

#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

using std::string;

class StringNode : public ExpressionNode
{
public:
    StringNode(const Token &value);

    const string &getValue() const;

    virtual void visit(Visitor *visitor) override;
private:
    Token token;
};