#pragma once

#include <memory>

#include "Visitor.h"

class SemanticErrorVisitor : public Visitor
{
public:
    SemanticErrorVisitor();

    virtual void visitAllChildren(Node *node) override;
    bool hasErrors() const;
public:
    virtual void visit(FuncDeclNode *node) override;
    virtual void visit(ReturnStatementNode *node) override;
public:
    int errorCount;
    int functionDepth;
};