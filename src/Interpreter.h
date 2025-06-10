#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <set>

#include "Visitor.h"
#include "Environment.h"
#include "Nodes.h"
#include "Value.h"

using std::string;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::set;

constexpr const char *INTERPRETER_VERSION = "1.0";

class Interpreter : public Visitor
{
public:
    Interpreter(bool isInteractive, std::shared_ptr<Environment> env = nullptr);
    virtual void visitAllChildren(Node *node) override;
public:
    // Override visit methods for different node types
    virtual void visit(StatementsNode *node) override;
    virtual void visit(NumberNode *node) override;
    virtual void visit(StringNode *node) override;
    virtual void visit(BinaryExprNode *node) override;
    virtual void visit(UnaryExprNode *node) override;
    virtual void visit(CallNode *node) override;
    virtual void visit(VarDeclNode *node) override;
    virtual void visit(VarExprNode *node) override;
    virtual void visit(AssignNode *node) override;
    virtual void visit(BlockNode *node) override;
    virtual void visit(IfStatementNode *node) override;
    virtual void visit(FuncDeclNode *node) override;
    virtual void visit(WhileNode *node) override;
    virtual void visit(ReturnStatementNode *node) override;
    virtual void visit(ForStatementNode *node) override;
    virtual void visit(NullNode *node) override;
    virtual void visit(BreakNode *node) override;
    virtual void visit(ImportNode *node) override;
    virtual void visit(ArrayNode *node) override;
    virtual void visit(ArrayAccessNode *node) override;
private:
    shared_ptr<Value> evalBinaryExpression(shared_ptr<ExpressionNode> left, shared_ptr<OpNode> opNode, shared_ptr<ExpressionNode> right);

    shared_ptr<Value> evalUnaryExpression(shared_ptr<ExpressionNode> expression, shared_ptr<OpNode> opNode, bool prefix = false);
    shared_ptr<Value> evalVariableUnaryExpression(shared_ptr<VarExprNode> expression, shared_ptr<OpNode> opNode, bool prefix = false);
private:
    void setupEnvironment();
    void setupBuiltInFunctions();
    void setupRuntimeValues();

    bool import(const Token &moduleName, const Range &range = {});
private:
    bool isInteractive;
    shared_ptr<Environment> env;
    shared_ptr<Value> returnValue;
    set<string> importedModules; // to avoid re-importing the same module
};