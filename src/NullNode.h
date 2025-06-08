#pragma once

#include <memory>
#include "ExpressionNode.h"
#include "Visitor.h"

using std::shared_ptr;

class NullNode : public ExpressionNode
{
public:
    using Ptr = shared_ptr<NullNode>;
public:
    void visit(Visitor *visitor) override;
};

using NullExprNodePtr = shared_ptr<NullNode>;