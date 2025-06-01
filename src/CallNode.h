#pragma once

#include "ArgListNode.h"
#include "ExpressionNode.h"
#include "Visitor.h"

class CallNode : public ExpressionNode
{
public:
    CallNode(ExpressionNode *callee, ArgListNode *args = nullptr);

    ExpressionNode *getCallee() const;
    ArgListNode *getArgs() const;

    virtual void visit(Visitor *visitor) override;
};