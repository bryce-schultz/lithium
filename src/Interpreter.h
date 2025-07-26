#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <set>
#include <functional>

#include "Visitor.h"
#include "Environment.h"
#include "Nodes.h"
#include "Value.h"
#include "Parser.h"
#include "CallStack.h"

using std::string;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::set;
using std::vector;

constexpr const char *INTERPRETER_VERSION = "0.4";

class Interpreter : public Visitor
{
public:
    Interpreter(bool isInteractive, shared_ptr<Environment> env = nullptr, const vector<string> &args = {});
    ~Interpreter(); // Add destructor for cleanup

    bool interpret(Node *node);

    shared_ptr<Environment> getEnvironment() const { return env; }
    void setEnvironment(shared_ptr<Environment> newEnv) { env = newEnv; }

    const CallStack &getCallStack() const { return callStack; }

    virtual void visitAllChildren(Node *node) override;
public:
    // Override visit methods for different node types
    virtual void visit(ArrayAccessNode *node) override;
    virtual void visit(ArrayNode *node) override;
    virtual void visit(AssertNode *node) override;
    virtual void visit(AssignNode *node) override;
    virtual void visit(BinaryExprNode *node) override;
    virtual void visit(BlockNode *node) override;
    virtual void visit(BooleanNode *node) override;
    virtual void visit(BreakNode *node) override;
    virtual void visit(CallNode *node) override;
    virtual void visit(ClassNode *node) override;
    virtual void visit(ContinueNode *node) override;
    virtual void visit(DeleteNode *node) override;
    virtual void visit(ForEachNode *node) override;
    virtual void visit(ForStatementNode *node) override;
    virtual void visit(FuncDeclNode *node) override;
    virtual void visit(IfStatementNode *node) override;
    virtual void visit(ImportNode *node) override;
    virtual void visit(MemberAccessNode *node) override;
    virtual void visit(NullNode *node) override;
    virtual void visit(NumberNode *node) override;
    virtual void visit(ReturnStatementNode *node) override;
    virtual void visit(StatementsNode *node) override;
    virtual void visit(StringNode *node) override;
    virtual void visit(UnaryExprNode *node) override;
    virtual void visit(VarDeclNode *node) override;
    virtual void visit(VarExprNode *node) override;
    virtual void visit(WhileNode *node) override;
private:
    shared_ptr<Value> evalUnaryExpression(shared_ptr<ExpressionNode> expression, shared_ptr<OpNode> opNode, bool prefix = false);
    shared_ptr<Value> evalVariableUnaryExpression(shared_ptr<VarExprNode> expression, shared_ptr<OpNode> opNode, bool prefix = false);
    shared_ptr<Value> evalIncrementDecrement(shared_ptr<ExpressionNode> expression, shared_ptr<OpNode> opNode, bool prefix = false);
    
    // Call node helper methods
    bool validateFunctionArguments(shared_ptr<FunctionValue> function, const vector<shared_ptr<Value>>& args, const Range& nodeRange, const string& functionType = "function");
    shared_ptr<Value> callUserFunction(shared_ptr<FunctionValue> function, const vector<shared_ptr<Value>>& args, const Range& nodeRange);
    shared_ptr<Value> callClassConstructor(shared_ptr<ClassValue> classValue, const vector<shared_ptr<Value>>& args, const Range& nodeRange);
private:
    void setupEnvironment();
    void setupBuiltInFunctions();
    void setupRuntimeValues();

    bool import(const Token &moduleName, const Range &range = {});
    void imported(const string &module);
private:
    bool isInteractive;
    bool hadError;
    shared_ptr<Environment> env;
    shared_ptr<Value> returnValue;
    Parser moduleParser;
    set<string> importedModules; // to avoid re-importing the same module
    vector<string> args; // command line arguments passed to the interpreter
    string currentFunctionName;

    // Recursion depth tracking to prevent stack overflow
    int recursionDepth;
    static const int MAX_RECURSION_DEPTH = 1000;

    // Track temporary environments for cleanup during chained calls
    std::vector<std::shared_ptr<Environment>> tempEnvironments;

    // Call stack for debugging and error reporting
    CallStack callStack;

    // Helper to clean up any temporary environments
    void cleanupTempEnvironments();
    void finalCleanup();
    void aggressiveCleanup();
};