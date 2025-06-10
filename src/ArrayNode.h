#pragma once

#include <memory>
#include <vector>
#include <string>

#include "ExpressionNode.h"
#include "ArgListNode.h"

using std::shared_ptr;
using std::vector;
using std::string;

class ArrayNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<ArrayNode>;
public:
    ArrayNode(const vector<shared_ptr<ExpressionNode>> &elements = {});

    const vector<shared_ptr<ExpressionNode>> &getElements() const;

    virtual bool isLval() const override;

    virtual void visit(Visitor *visitor) override;
private:
    vector<shared_ptr<ExpressionNode>> elements;
};