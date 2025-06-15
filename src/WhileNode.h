#pragma once

#include <memory>

#include "Node.h"
#include "ExpressionNode.h"
#include "StatementNode.h"

using std::shared_ptr;

class WhileNode : public StatementNode
{
public:
    using Ptr = shared_ptr<WhileNode>;
public:
    WhileNode(shared_ptr<ExpressionNode> condition, shared_ptr<StatementNode> body);

    shared_ptr<ExpressionNode> getCondition() const;

    shared_ptr<StatementNode> getBody() const;

    void visit(Visitor *visitor) override;
private:
    shared_ptr<ExpressionNode> condition;
    shared_ptr<StatementNode> body;
};

using WhileNodePtr = shared_ptr<WhileNode>;
