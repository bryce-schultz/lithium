#pragma once

#include "DeclNode.h"
#include "Token.h"
#include "ParamListNode.h"
#include "BlockNode.h"

class FuncDeclNode : public DeclNode
{
public:
    FuncDeclNode(const Token &token, ParamListNode *params, StatementNode *body);

    const Token &getToken() const;

    virtual string getName() const override;

    ParamListNode *getParams() const;

    StatementNode *getBody() const;

    void dropBody(); // drop so they don't get deleted
    void dropParams(); // drop so they don't get deleted

    virtual bool isConst() const override;

    virtual void visit(Visitor *visitor) override;
private:
    Token token;
};