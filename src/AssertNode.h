#pragma once

#include <memory>
#include <string>

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "Visitor.h"
#include "Token.h"

using std::shared_ptr;
using std::string;

class AssertNode : public StatementNode
{
public:
    AssertNode(shared_ptr<ExpressionNode> condition, shared_ptr<ExpressionNode> message = nullptr);

    shared_ptr<ExpressionNode> getCondition() const;

    shared_ptr<ExpressionNode> getMessage() const;

    virtual void visit(Visitor *visitor) override;
private:
    shared_ptr<ExpressionNode> condition;
    shared_ptr<ExpressionNode> message;
};