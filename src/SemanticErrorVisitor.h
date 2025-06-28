#pragma once

#include <memory>
#include <set>
#include <string>

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
    virtual void visit(ForStatementNode *node) override;
    virtual void visit(WhileNode *node) override;
    virtual void visit(BreakNode *node) override;
    virtual void visit(CallNode *node) override;
    virtual void visit(ClassNode *node) override;
    virtual void visit(AssignNode *node) override;
    virtual void visit(MemberAccessNode *node) override;
    virtual void visit(ArrayAccessNode *node) override;
public:
    int errorCount;
    int functionDepth;
    int loopDepth;

    // Recursion detection
    std::set<std::string> currentFunctionStack;
    std::string currentFunctionName;

    // Type tracking for semantic analysis
    std::set<std::string> declaredClasses;
    std::set<std::string> declaredFunctions;
    std::set<std::string> declaredVariables;
};