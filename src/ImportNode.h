#pragma once

#include <string>
#include <memory>

#include "StatementNode.h"
#include "Visitor.h"
#include "Token.h"

using std::string;
using std::shared_ptr;

class ImportNode : public StatementNode
{
public:
    using Ptr = shared_ptr<ImportNode>;
public:
    ImportNode(const Token &token);

    const Token &getToken() const;

    const std::string &getModuleName() const;

    void visit(Visitor *visitor) override;
private:
    Token token;
};

using ImportNodePtr = shared_ptr<ImportNode>;