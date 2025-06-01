#pragma once

#include <string>

#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

using std::string;

class AssignNode : public ExpressionNode
{
public:
    AssignNode(ExpressionNode *asignee, ExpressionNode *expr = nullptr);

    virtual bool isVariable() const override;

    ExpressionNode *getAsignee() const;
    ExpressionNode *getExpr() const;

    virtual void visit(Visitor *visitor) override;
};