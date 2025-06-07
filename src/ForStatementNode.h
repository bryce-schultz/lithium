#pragma once

#include <memory>

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "Visitor.h"

using std::shared_ptr;

// forStmt -> FOR ( exprStmt exprStmt expr ) stmt
class ForStatementNode : public StatementNode
{
public:
    using Ptr = shared_ptr<ForStatementNode>;
public:
    ForStatementNode(shared_ptr<StatementNode> init,
        shared_ptr<StatementNode> condition,
        shared_ptr<ExpressionNode> increment,
        shared_ptr<StatementNode> body);

    shared_ptr<StatementNode> getInit() const;
    shared_ptr<StatementNode> getCondition() const;
    shared_ptr<ExpressionNode> getIncrement() const;
    shared_ptr<StatementNode> getBody() const;

    virtual void visit(Visitor *visitor) override;
private:
    shared_ptr<StatementNode> init;
    shared_ptr<StatementNode> condition;
    shared_ptr<ExpressionNode> increment;
    shared_ptr<StatementNode> body;
};