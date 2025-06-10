#pragma once

#include <memory>
#include <string>

#include "ExpressionNode.h"
#include "OpNode.h"
#include "Visitor.h"

using std::shared_ptr;
using std::string;

class UnaryExprNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<UnaryExprNode>;
public:
    UnaryExprNode(shared_ptr<ExpressionNode> expr, shared_ptr<OpNode> op, bool prefix = true);

    shared_ptr<OpNode> getOperator() const;
    shared_ptr<ExpressionNode> getExpression() const;
    bool isPrefix() const;

    virtual void visit(Visitor *visitor) override;
private:
    shared_ptr<ExpressionNode> expr;
    shared_ptr<OpNode> op;
    bool prefix; // true for prefix, false for postfix
};

using UnaryExprNodePtr = shared_ptr<UnaryExprNode>;