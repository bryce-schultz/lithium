#pragma once

#include "StatementNode.h"

class ForEachNode : public StatementNode
{
public:
    ForEachNode(shared_ptr<VarDeclNode> keyDecl, shared_ptr<VarDeclNode> valueDecl, shared_ptr<ExpressionNode> iterable, shared_ptr<StatementNode> body);

    shared_ptr<VarDeclNode> getKeyDecl() const;
    shared_ptr<VarDeclNode> getValueDecl() const;
    shared_ptr<ExpressionNode> getIterable() const;
    shared_ptr<StatementNode> getBody() const;

    bool isArrayLike() const;
    bool isMapLike() const;

    virtual void visit(Visitor *visitor) override;
private:
    shared_ptr<VarDeclNode> keyDecl;
    shared_ptr<VarDeclNode> valueDecl;
    shared_ptr<ExpressionNode> iterable;
    shared_ptr<StatementNode> body;
};