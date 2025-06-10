#pragma once

#include <memory>
#include <string>

#include "DeclNode.h"
#include "Visitor.h"
#include "Token.h"

using std::shared_ptr;
using std::string;

class ClassNode : public DeclNode
{
public:
    using Ptr = shared_ptr<ClassNode>;
public:
    ClassNode(const Token &token, shared_ptr<StatementNode> body = nullptr);

    const Token &getToken() const override;

    const string &getName() const override;

    shared_ptr<StatementNode> getBody() const;

    void setBody(shared_ptr<StatementNode> body);

    bool isConst() const override;

    void visit(Visitor *visitor) override;
private:
    Token token;
    shared_ptr<StatementNode> body;
};