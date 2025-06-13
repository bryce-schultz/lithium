#pragma once

#include <memory>
#include <string>

#include "StatementNode.h"
#include "Visitor.h"
#include "Token.h"

using std::shared_ptr;
using std::string;
using std::make_shared;

class ContinueNode : public StatementNode
{
public:
    ContinueNode(const Token &token);

    virtual void visit(Visitor *visitor) override;
};