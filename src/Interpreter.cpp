#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>

#include "Interpreter.h"
#include "Nodes.h"
#include "Values.h"
#include "Error.h"
#include "Utils.h"
#include "Environment.h"
#include "Visitor.h"
#include "Builtins.h"
#include "Exceptions.h"
#include "Parser.h"
#include "SemanticErrorVisitor.h"
#include "ArrayBuilder.h"

using std::shared_ptr;
using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::dynamic_pointer_cast;
using std::make_shared;
using std::fmod;
using std::vector;
using std::to_string;

#define MAX_PEEK 25

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__); \
    hadError = true; \
    throw ErrorException(msg, range)

#define errorAt(msg, location, range) \
    locationRangeError(msg, location, range, __FILE__, __LINE__); \
    hadError = true; \
    throw ErrorException(msg, range)

#define errorAtToken(msg, token, range) \
    tokenRangeError(msg, token, range, __FILE__, __LINE__); \
    hadError = true; \
    throw ErrorException(msg, range)

#define alreadyDefined(node) \
    errorAtToken("'" + Utils::truncate(node->getName(), MAX_PEEK) + "' is already defined", node->getToken(), node->getRange())

#define notDefined(node) \
    errorAtToken("'" + Utils::truncate(node->getName(), MAX_PEEK) + "' is not defined", node->getToken(), node->getRange())

#define failedToLoadModule(token, range) \
    errorAtToken("failed to load module '" + token.getValue() + "'", token, range); \
    return false

#define couldNotFindModule(token, range) \
    errorAtToken("could not find module '" + token.getValue() + "'", token, range); \
    return false

Interpreter::Interpreter(bool isInteractive, shared_ptr<Environment> env, const vector<string> &args):
    isInteractive(isInteractive),
    hadError(false),
    returnValue(nullptr),
    moduleParser(),
    importedModules(),
    args(args),
    recursionDepth(0)
{
    if (!env)
    {
        this->env = make_shared<Environment>(); // Create a new environment if none is provided
    }
    else
    {
        this->env = env; // Use the provided environment
    }

    // runtime values defined in all environments
    setupEnvironment();
}

Interpreter::~Interpreter()
{
    // Break circular references by clearing environment
    if (env) {
        // Clear all environments in the chain to break cycles
        auto current = env;
        while (current) {
            auto parent = current->getParent();
            current->clear();
            current = parent;
        }
    }
    env.reset();
    returnValue.reset();
}

bool Interpreter::interpret(Node *node)
{
    if (!node)
    {
        return false; // Nothing to interpret
    }

    // Reset the return value for each interpretation
    returnValue.reset();

    // Visit the node to interpret it
    visitAllChildren(node);

    return !hadError;
}

void Interpreter::setupEnvironment()
{
    setupBuiltInFunctions();
    setupRuntimeValues();
}

//***********************************************
// Built-in functions setup
//***********************************************
void Interpreter::setupBuiltInFunctions()
{
    env->declare("type", make_shared<BuiltinFunctionValue>(Builtins::type), true);
    env->declare("exit", make_shared<BuiltinFunctionValue>(Builtins::exit), true);
    env->declare("len", make_shared<BuiltinFunctionValue>(Builtins::len), true);
    env->declare("number", make_shared<BuiltinFunctionValue>(Builtins::toNumber), true);
    env->declare("print", make_shared<BuiltinFunctionValue>(Builtins::print), true);
    env->declare("println", make_shared<BuiltinFunctionValue>(Builtins::println), true);
    env->declare("dumpenv", make_shared<BuiltinFunctionValue>(Builtins::dumpEnv), true);
}

void Interpreter::setupRuntimeValues()
{
    // FILE is also declared, but its setup in the main file for now.
    env->declare("null", make_shared<NullValue>(), true);
    env->declare("true", make_shared<BooleanValue>(true), true);
    env->declare("false", make_shared<BooleanValue>(false), true);
    env->declare("VERSION", make_shared<StringValue>(INTERPRETER_VERSION), true);
}

bool Interpreter::import(const Token& moduleName, const Range &range)
{
    string module = moduleName.getValue();
    string modulePath = Utils::getModulePath(module);

    // check if the module is already imported in the interpreter
    if (importedModules.find(module) != importedModules.end())
    {
        return false;
    }

    if (modulePath.empty())
    {
        // check if its an interpreter internal module (io, math, etc.)
        if (module == "args")
        {
            ArrayBuilder builder;

            for (const string &arg : args)
            {
                builder.add(arg);
            }

            env->declare("args", builder.build(), true);
            imported(module);
            return true;
        }
        if (module == "io")
        {
            env->declare("printf", make_shared<BuiltinFunctionValue>(Builtins::printf), true);
            env->declare("input", make_shared<BuiltinFunctionValue>(Builtins::input), true);
            imported(module);
            return true;
        }
        else if (module == "math")
        {
            env->declare("PI", make_shared<NumberValue>(M_PI), true);
            env->declare("E", make_shared<NumberValue>(M_E), true);
            imported(module);
            return true;
        }
        else if (module == "random")
        {
            env->declare("random", make_shared<BuiltinFunctionValue>(Builtins::randomNumber), true);
            imported(module);
            return true;
        }
        else if (module == "os")
        {
            env->declare("open", make_shared<BuiltinFunctionValue>(Builtins::openFile), true);
            env->declare("close", make_shared<BuiltinFunctionValue>(Builtins::closeFd), true);
            env->declare("read", make_shared<BuiltinFunctionValue>(Builtins::readFd), true);
            env->declare("write", make_shared<BuiltinFunctionValue>(Builtins::writeFd), true);
            env->declare("shell", make_shared<BuiltinFunctionValue>(Builtins::runShellCommand), true);
            imported(module);
            return true;
        }
        else if (module == "socket")
        {
            env->declare("socket", make_shared<BuiltinFunctionValue>(Builtins::openSocket), true);
            env->declare("close", make_shared<BuiltinFunctionValue>(Builtins::closeFd), true);
            env->declare("listen", make_shared<BuiltinFunctionValue>(Builtins::listenSocket), true);
            env->declare("accept", make_shared<BuiltinFunctionValue>(Builtins::acceptSocket), true);
            env->declare("connect", make_shared<BuiltinFunctionValue>(Builtins::connectSocket), true);
            env->declare("send", make_shared<BuiltinFunctionValue>(Builtins::sendSocket), true);
            env->declare("receive", make_shared<BuiltinFunctionValue>(Builtins::receiveSocket), true);
            //env->declare("sock_addr", make_shared<BuiltinFunctionValue>(Builtins::getSocketAddress), true);
            //env->declare("sock_port", make_shared<BuiltinFunctionValue>(Builtins::getSocketPort), true);
            imported(module);
            return true;
        }
        else if (module == "time")
        {
            env->declare("time", make_shared<BuiltinFunctionValue>(Builtins::time), true);
            env->declare("sleep", make_shared<BuiltinFunctionValue>(Builtins::sleep), true);
            imported(module);
            return true;
        }

        couldNotFindModule(moduleName, range);
    }

    const string moduleContent = Utils::readWholeFile(modulePath);
    if (moduleContent.empty())
    {
        // empty modules are fine, just return true, TODO: maybe add a warning later.
        imported(module);
        return true;
    }

    try
    {
        // Normalize the module path by removing ./ prefix
        string normalizedModulePath = modulePath;
        Utils::removePrefix(normalizedModulePath, "./");
        
        auto moduleNode = moduleParser.parse(moduleContent, normalizedModulePath);
        if (!moduleNode.status)
        {
            failedToLoadModule(moduleName, range);
        }

        // Check for semantic errors in the module
        SemanticErrorVisitor semanticVisitor;
        semanticVisitor.visitAllChildren(moduleNode.value.get());
        if (semanticVisitor.hasErrors())
        {
            failedToLoadModule(moduleName, range);
        }

        // to import the module we simply visit it as though it was a node in the current ast.
        visitAllChildren(moduleNode.value.get());

        imported(module);
    }
    catch (const exception &e)
    {
        errorAtToken("error while importing module '" + moduleName.getValue() + "': " + e.what(), moduleName, range);
        return false;
    }

    imported(module);
    return true;
}

void Interpreter::imported(const string &module)
{
    importedModules.insert(module);
}

void Interpreter::visitAllChildren(Node *node)
{
    try
    {
        node->visit(this);
    }
    catch (const ExitException &e)
    {
        // Re-throw to let main handle the cleanup and exit
        throw;
    }
    catch (const ErrorException &e)
    {
        // do nothing
    }
}

void Interpreter::visit(StatementsNode *node)
{
    // First pass: hoist all function declarations
    for (auto &statement : node->getStatements())
    {
        if (auto funcDecl = dynamic_pointer_cast<FuncDeclNode>(statement))
        {
            shared_ptr<Value> value = env->lookupLocal(funcDecl->getName());
            // Only declare if not already declared (to avoid redeclaration error)
            if (value)
            {
                alreadyDefined(funcDecl);
                continue;
            }

            auto function = make_shared<FunctionValue>(
                funcDecl->getName(),
                funcDecl->getParams(),
                funcDecl->getBody(),
                env
            );
            env->declare(funcDecl->getName(), function, funcDecl->isConst());
        }
    }

    // Second pass: execute all statements
    for (auto &statement : node->getStatements())
    {
        if (!statement) continue;
        returnValue = nullptr;
        statement->visit(this);
        if (returnValue && isInteractive)
        {
            cout << returnValue->toString() << endl;
        }
    }
}

void Interpreter::visit(NumberNode *node)
{
    returnValue = make_shared<NumberValue>(node->getValue(), node->getRange());
}

void Interpreter::visit(StringNode *node)
{
    if (!node)
    {
        error("StringNode is null", Range());
        return;
    }

    returnValue = make_shared<StringValue>(node->getValue());
}

void Interpreter::visit(BinaryExprNode *node)
{
    returnValue = evalBinaryExpression(node->getLeft(), node->getOperator(), node->getRight());
}

void Interpreter::visit(UnaryExprNode *node)
{
    returnValue = evalUnaryExpression(node->getExpression(), node->getOperator(), node->isPrefix());
}

shared_ptr<Value> Interpreter::evalBinaryExpression(shared_ptr<ExpressionNode> left, shared_ptr<OpNode> opNode, shared_ptr<ExpressionNode> right)
{
    left->visit(this);
    auto leftValue = returnValue;
    if (!leftValue)
    {
        error("left operand of binary expression is null", left->getRange());
        return nullptr;
    }

    // short-circuit evaluation for logical operators
    if (opNode->getType() == Token::AND && leftValue->toBoolean() == false)
    {
        returnValue = leftValue;
        return leftValue;
    }
    else if (opNode->getType() == Token::OR && leftValue->toBoolean() == true)
    {
        returnValue = leftValue;
        return leftValue;
    }

    right->visit(this);
    auto rightValue = returnValue;
    if (!rightValue)
    {
        error("right operand of binary expression is null", right->getRange());
        return nullptr;
    }

    returnValue = nullptr;
    switch(opNode->getType())
    {
        case '+':
            returnValue = leftValue->add(rightValue);
            break;
        case '-':
            returnValue = leftValue->sub(rightValue);
            break;
        case '*':
            returnValue = leftValue->mul(rightValue);
            break;
        case '/':
            returnValue = leftValue->div(rightValue);
            break;
        case '%':
            returnValue = leftValue->mod(rightValue);
            break;
        case Token::EQ:
            returnValue = leftValue->eq(rightValue);
            break;
        case Token::NE:
            returnValue = leftValue->ne(rightValue);
            break;
        case Token::LE:
            returnValue = leftValue->le(rightValue);
            break;
        case Token::GE:
            returnValue = leftValue->ge(rightValue);
            break;
        case '<':
            returnValue = leftValue->lt(rightValue);
            break;
        case '>':
            returnValue = leftValue->gt(rightValue);
            break;
        case Token::AND:
            returnValue = leftValue->logicalAnd(rightValue);
            break;
        case Token::OR:
            returnValue = leftValue->logicalOr(rightValue);
            break;
        default:
            error("unsupported binary operation: " + opNode->getToken().getValue(), opNode->getRange());
            return nullptr;
    }

    if (returnValue && returnValue->getType() != Value::Type::null)
    {
        return returnValue;
    }

    if (returnValue && returnValue->getType() == Value::Type::null)
    {
        // If the operation resulted in a null value, we can return it as is
        return nullptr;
    }

    error("unsupported binary operation between " + leftValue->typeAsString() + " and " + rightValue->typeAsString(), opNode->getRange());
    return nullptr;
}

shared_ptr<Value> Interpreter::evalUnaryExpression(shared_ptr<ExpressionNode> expression, shared_ptr<OpNode> opNode, bool prefix)
{
    if (!expression)
    {
        error("null expression in unary operation", opNode->getRange());
        return nullptr;
    }

    expression->visit(this);
    auto value = returnValue;
    if (!value)
    {
        error("unary expression evaluation failed", expression->getRange());
        return nullptr;
    }

    switch(opNode->getType())
    {
        case Token::INC:
        case Token::DEC:
        {
            if (!expression->isLval())
            {
                error("expected a modifiable expression", expression->getRange());
                return nullptr;
            }

            shared_ptr<VarExprNode> varExpr = dynamic_pointer_cast<VarExprNode>(expression);
            if (!varExpr)
            {
                error("expected a modifiable expression", expression->getRange());
                return nullptr;
            }

            return evalVariableUnaryExpression(varExpr, opNode, prefix);
        }
        case '!':
            returnValue = value->unaryNot();
            break;
        case '-':
            returnValue = value->unaryMinus();
            break;
    }

    if (returnValue && returnValue->getType() != Value::Type::null)
    {
        return returnValue;
    }

    if (returnValue && returnValue->getType() == Value::Type::null)
    {
        // If the operation resulted in a null value, we can return it as is
        return nullptr;
    }

    error("unsupported unary operation on " + value->typeAsString(), opNode->getRange());
    return nullptr;
}

shared_ptr<Value> Interpreter::evalVariableUnaryExpression(shared_ptr<VarExprNode> expression, shared_ptr<OpNode> opNode, bool prefix)
{
    auto value = env->lookup(expression->getName());
    if (!value)
    {
        error("variable " + expression->getName() + " is not defined", expression->getRange());
        return nullptr;
    }

    switch(opNode->getType())
    {
        case Token::INC:
        {
            if (value->getType() != Value::Type::number)
            {
                error("variable " + expression->getName() + " is not a number", expression->getRange());
                return nullptr;
            }
            auto numberValue = dynamic_pointer_cast<NumberValue>(value);
            auto tmp = make_shared<NumberValue>(numberValue->getValue() + 1);
            env->assign(expression->getName(), tmp);

            if (prefix)
            {
                return tmp; // return the incremented value if prefix
            }
            else
            {
                return numberValue; // return the original value if postfix
            }

            break;
        }
        case Token::DEC:
        {
            if (value->getType() != Value::Type::number)
            {
                error("variable " + expression->getName() + " is not a number", expression->getRange());
                return nullptr;
            }
            auto numberValue = dynamic_pointer_cast<NumberValue>(value);
            auto tmp = make_shared<NumberValue>(numberValue->getValue() - 1);
            env->assign(expression->getName(), tmp);

            if (prefix)
            {
                return tmp; // return the incremented value if prefix
            }
            else
            {
                return numberValue; // return the original value if postfix
            }

            break;
        }
    }

    return nullptr;
}

void Interpreter::visit(CallNode *node)
{
    vector<shared_ptr<Value>> args;
    if (node->getArgs()) {
        for (auto &arg : node->getArgs()->getArgs()) {
            arg->visit(this);
            args.push_back(returnValue);
        }
    }

    auto calleeNode = node->getCallee();
    calleeNode->visit(this);
    if (!returnValue)
    {
        return;
    }

    shared_ptr<Value> callee = returnValue;
    if (callee->getType() == Value::Type::function)
    {
        shared_ptr<FunctionValue> function = dynamic_pointer_cast<FunctionValue>(callee);
        if (!function)
        {
            error("callee could not be cast to FunctionValue", calleeNode->getRange());
            returnValue = nullptr;
            return;
        }

        if (function->getParameters() && (size_t)function->getParameters()->getParamCount() != args.size())
        {
            error("function '" + function->getName() + "' expects " +
                to_string(function->getParameters()->getParamCount()) +
                " arguments, but got " + to_string(args.size()), node->getRange());
            returnValue = nullptr;
            return;
        }
        else if (!function->getParameters() && !args.empty())
        {
            errorAt("function '" + function->getName() + "' does not take any arguments, but got " + to_string(args.size()), args[0]->getRange().getStart(), node->getRange());
            returnValue = nullptr;
            return;
        }

        // Check recursion depth to prevent stack overflow
        if (recursionDepth >= MAX_RECURSION_DEPTH)
        {
            error("maximum recursion depth exceeded (" + to_string(MAX_RECURSION_DEPTH) + ")", node->getRange());
            returnValue = nullptr;
            return;
        }

        shared_ptr<Environment> scope = make_shared<Environment>(function->getEnvironment());
        for (size_t i = 0; i < args.size(); ++i)
        {
            scope->declare(function->getParameters()->getParam(i)->getName(), args[i]);
        }
        auto previousEnv = env;
        env = scope;
        
        // Increment recursion depth
        recursionDepth++;
        
        try
        {
            if (!function->getBody()) {
                errorAt("function '" + function->getName() + "' has no body", node->getCallee()->getRange().getStart(), node->getRange());
                returnValue = nullptr;
                recursionDepth--;
                env = previousEnv;
                return;
            }
            
            function->getBody()->visit(this);
            returnValue = nullptr;
        }
        catch (const ReturnException& e)
        {
            returnValue = e.value;
        }
        catch (const BreakException &e)
        {
            recursionDepth--;
            env = previousEnv;
            throw;
        }
        catch (const ContinueException &e)
        {
            recursionDepth--;
            env = previousEnv;
            throw;
        }
        catch (const ExitException &e)
        {
            recursionDepth--;
            env = previousEnv;
            throw;
        }
        catch (const ErrorException &e)
        {
            recursionDepth--;
            env = previousEnv;
            throw;
        }
        
        // Decrement recursion depth
        recursionDepth--;
        
        // Don't clear the scope in normal flow - let RAII handle it  
        env = previousEnv;
    }
    else if (callee->getType() == Value::Type::builtin)
    {
        shared_ptr<BuiltinFunctionValue> builtin = dynamic_pointer_cast<BuiltinFunctionValue>(callee);
        if (!builtin)
        {
            error("callee could not be cast to BuiltinFunctionValue", calleeNode->getRange());
            returnValue = nullptr;
            return;
        }

        returnValue = builtin->call(args, env, node->getRange());
    }
    else if (callee->getType() == Value::Type::class_)
    {
        shared_ptr<ClassValue> classValue = dynamic_pointer_cast<ClassValue>(callee);
        if (!classValue)
        {
            error("callee could not be cast to ClassValue", calleeNode->getRange());
            returnValue = nullptr;
            return;
        }

        // visit the declarations of the class and capture them in a new environment
        auto previousEnv = env;
        shared_ptr<Environment> classEnv = make_shared<Environment>(env);
        env = classEnv;
        
        try
        {
            shared_ptr<BlockNode> classBody = dynamic_pointer_cast<BlockNode>(classValue->getBody());
            if (!classBody)
            {
                error("class '" + classValue->getName() + "' has no body", calleeNode->getRange());
                returnValue = nullptr;
                env = previousEnv;
                return;
            }

            // we don't want to push extra scope for class body, so we just visit the statements directly
            if (classBody->getStatements())
            {
                classBody->getStatements()->visit(this);
            }

            // call the constructor of the class
            auto ctor = env->lookupLocal(classValue->getName());

            if (ctor)
            {
                if (ctor->getType() != Value::Type::function)
                {
                    error("class '" + classValue->getName() + "' has no constructor", calleeNode->getRange());
                    returnValue = nullptr;
                    env = previousEnv;
                    return;
                }

                shared_ptr<FunctionValue> constructor = dynamic_pointer_cast<FunctionValue>(ctor);
                if (!constructor)
                {
                    error("class '" + classValue->getName() + "' constructor could not be cast to FunctionValue", calleeNode->getRange());
                    returnValue = nullptr;
                    env = previousEnv;
                    return;
                }

                // TODO: come back here
                if (constructor->getParameters() && (size_t)constructor->getParameters()->getParamCount() != args.size())
                {
                    error("function '" + constructor->getName() + "' expects " +
                        to_string(constructor->getParameters()->getParamCount()) +
                        " arguments, but got " + to_string(args.size()), node->getRange());
                    returnValue = nullptr;
                    env = previousEnv;
                    return;
                }
                else if (!constructor->getParameters() && !args.empty())
                {
                    error("function '" + constructor->getName() + "' does not take any arguments, but got " + to_string(args.size()), node->getRange());
                    returnValue = nullptr;
                    env = previousEnv;
                    return;
                }

                shared_ptr<Environment> scope = make_shared<Environment>(constructor->getEnvironment());
                for (size_t i = 0; i < args.size(); ++i)
                {
                    scope->declare(constructor->getParameters()->getParam(i)->getName(), args[i]);
                }
                auto constructorPrevEnv = env;
                env = scope;
                try
                {
                    if (!constructor->getBody()) {
                        error("constructor '" + constructor->getName() + "' has no body", node->getRange());
                        env = constructorPrevEnv;
                        return;
                    }
                    
                    constructor->getBody()->visit(this);
                }
                catch (const ReturnException& e)
                {
                    if (e.value && e.value->getType() != Value::Type::null)
                    {
                        error("constructor of class '" + classValue->getName() + "' returned a value, which is not allowed", node->getRange());
                    }
                }
                catch (const BreakException &e)
                {
                    env = constructorPrevEnv;
                    throw;
                }
                catch (const ContinueException &e)
                {
                    env = constructorPrevEnv;
                    throw;
                }
                catch (const ExitException &e)
                {
                    env = constructorPrevEnv;
                    throw;
                }
                catch (const ErrorException &e)
                {
                    env = constructorPrevEnv;
                    throw;
                }
                
                // Don't clear the scope in normal flow - let RAII handle it
                env = constructorPrevEnv;
            }

            // create a new instance of the class
            auto instance = make_shared<ObjectValue>(classValue->getName(), env);

            // Don't clear the class environment in normal flow - let RAII handle it
            env = previousEnv;
            returnValue = instance;
        }
        catch (const BreakException &e)
        {
            // Clear the class environment before restoring on exception
            classEnv->clear();
            env = previousEnv;
            throw;
        }
        catch (const ContinueException &e)
        {
            // Clear the class environment before restoring on exception
            classEnv->clear();
            env = previousEnv;
            throw;
        }
        catch (const ExitException &e)
        {
            // Clear the class environment before restoring on exception
            classEnv->clear();
            env = previousEnv;
            throw;
        }
        catch (const ErrorException &e)
        {
            // Clear the class environment before restoring on exception
            classEnv->clear();
            env = previousEnv;
            throw;
        }
        return;
    }
    else
    {
        error("cannot call non-function value: " + callee->typeAsString(), node->getRange());
        returnValue = nullptr;
    }
}

void Interpreter::visit(ReturnStatementNode *node)
{
    auto expr = node->getExpression();
    if (expr)
    {
        expr->visit(this);
        if (!returnValue)
        {
            error("return expression evaluated to null", node->getRange());
            throw ReturnException(make_shared<NullValue>(), node->getRange());
        }
        throw ReturnException(returnValue, node->getRange());
    }
    else
    {
        throw ReturnException(make_shared<NullValue>(), node->getRange());
    }
}

void Interpreter::visit(VarDeclNode *node)
{
    if (!node->getExpr())
    {
        returnValue = make_shared<NullValue>(); // If no expression, declare as null
    }
    else
    {
       node->getExpr()->visit(this);
    }

    if (!returnValue)
    {
        errorAt("invalid assignment", node->getExpr()->getRange().getStart(), node->getRange());
        return;
    }

    returnValue = env->declare(node->getName(), returnValue, node->isConst());
    if (!returnValue)
    {
        alreadyDefined(node);
    }

    returnValue = nullptr;
}

void Interpreter::visit(VarExprNode *node)
{
    // Intercept special variables
    if (node->getName() == "FILE")
    {
        returnValue = make_shared<StringValue>(node->getRange().getStart().getFilename());
        returnValue->setRange(node->getRange());
        return;
    }
    
    if (node->getName() == "LINE")
    {
        returnValue = make_shared<NumberValue>(node->getRange().getStart().getLine());
        returnValue->setRange(node->getRange());
        return;
    }
    
    returnValue = env->lookup(node->getName());
    if (!returnValue)
    {
        notDefined(node);
        returnValue = nullptr;
        return;
    }

    returnValue->setRange(node->getRange());
}

void Interpreter::visit(AssignNode *node)
{
    node->getExpr()->visit(this);
    shared_ptr<Value> value = returnValue;
    if (!value) {
        error("assignment right-hand side evaluated to null", node->getExpr()->getRange());
        returnValue = nullptr;
        return;
    }
    if (!node->getAsignee()->isLval())
    {
        error("cannot assign to a non-variable expression", node->getRange());
        returnValue = nullptr;
        return;
    }
    auto asignee = dynamic_pointer_cast<VarExprNode>(node->getAsignee());
    if (asignee)
    {
        switch (node->getOp())
        {
            case '=':
                // No additional operation, just assign the value
                break;
            case Token::PLUS_EQUAL:
                value = env->lookup(asignee->getName())->add(value);
                break;
            case Token::MINUS_EQUAL:
                value = env->lookup(asignee->getName())->sub(value);
                break;
            case Token::MUL_EQUAL:
                value = env->lookup(asignee->getName())->mul(value);
                break;
            case Token::DIV_EQUAL:
                value = env->lookup(asignee->getName())->div(value);
                break;
            case Token::MOD_EQUAL:
                value = env->lookup(asignee->getName())->mod(value);
                break;
            default:
                error("invalid assignment operator", node->getRange());
                returnValue = nullptr;
                return;
        }

        if (!value)
        {
            string opName;
            string preposition;
            string leftType, rightType;
            auto assigneeValue = env->lookup(asignee->getName());
            string assigneeType = assigneeValue ? assigneeValue->typeAsString() : "unknown";
            
            switch (node->getOp())
            {
                case Token::PLUS_EQUAL:
                    opName = "add";
                    preposition = "to";
                    leftType = returnValue->typeAsString();
                    rightType = assigneeType;
                    break;
                case Token::MINUS_EQUAL:
                    opName = "subtract";
                    preposition = "from";
                    leftType = returnValue->typeAsString();
                    rightType = assigneeType;
                    break;
                case Token::MUL_EQUAL:
                    opName = "multiply";
                    preposition = "with";
                    leftType = assigneeType;
                    rightType = returnValue->typeAsString();
                    break;
                case Token::DIV_EQUAL:
                    opName = "divide";
                    preposition = "by";
                    leftType = assigneeType;
                    rightType = returnValue->typeAsString();
                    break;
                case Token::MOD_EQUAL:
                    opName = "mod";
                    preposition = "by";
                    leftType = assigneeType;
                    rightType = returnValue->typeAsString();
                    break;
                default:
                    opName = "operate";
                    preposition = "with";
                    leftType = assigneeType;
                    rightType = returnValue->typeAsString();
                    break;
            }
            
            errorAtToken("invalid assignment type, can't " + opName + " " + leftType + " " + preposition + " " + rightType, node->getToken(), node->getRange());
            returnValue = nullptr;
            return;
        }

        auto result = env->assign(asignee->getName(), value);
        if (result.status == Environment::VARIABLE_NOT_FOUND)
        {
            notDefined(asignee);
            returnValue = nullptr;
            return;
        }
        else if (result.status == Environment::VARIABLE_IS_CONSTANT)
        {
            errorAtToken("cannot assign to constant variable '" + asignee->getName() + "'", asignee->getToken(), node->getRange());
            returnValue = nullptr;
            return;
        }

        returnValue = result.value;
        return;
    }
    shared_ptr<ArrayAccessNode> access = dynamic_pointer_cast<ArrayAccessNode>(node->getAsignee());
    if (access)
    {
        access->getArray()->visit(this);
        if (!returnValue)
        {
            error("array access left-hand side evaluated to null", access->getArray()->getRange());
            returnValue = nullptr;
            return;
        }

        if (returnValue->getType() != Value::Type::array)
        {
            error("left-hand side of array access is not an array", access->getArray()->getRange());
            returnValue = nullptr;
            return;
        }

        auto arrayValue = dynamic_pointer_cast<ArrayValue>(returnValue);
        if (!arrayValue)
        {
            error("left-hand side of array access could not be cast to ArrayValue", access->getArray()->getRange());
            returnValue = nullptr;
            return;
        }

        access->getIndex()->visit(this);
        if (!returnValue)
        {
            error("array access index evaluated to null", access->getIndex()->getRange());
            returnValue = nullptr;
            return;
        }

        if (returnValue->getType() != Value::Type::number)
        {
            error("array access index must be a number", access->getIndex()->getRange());
            returnValue = nullptr;
            return;
        }

        auto indexValue = dynamic_pointer_cast<NumberValue>(returnValue);
        int index = indexValue ? static_cast<int>(indexValue->getValue()) : 0;

        if (index < 0 || index >= arrayValue->getElementCount())
        {
            error("array index out of bounds: " + to_string(index), access->getIndex()->getRange());
            returnValue = nullptr;
            return;
        }

        switch (node->getOp())
        {
            case '=':
                // No additional operation, just assign the value
                break;
            case Token::PLUS_EQUAL:
                value = arrayValue->getElement(index)->add(value);
                break;
            case Token::MINUS_EQUAL:
                value = arrayValue->getElement(index)->sub(value);
                break;
            case Token::MUL_EQUAL:
                value = arrayValue->getElement(index)->mul(value);
                break;
            case Token::DIV_EQUAL:
                value = arrayValue->getElement(index)->div(value);
                break;
            case Token::MOD_EQUAL:
                value = arrayValue->getElement(index)->mod(value);
                break;
            default:
                error("invalid assignment operator", node->getRange());
                returnValue = nullptr;
                return;
        }

        // assign the value to the specified index
        arrayValue->setElement(index, value);
        returnValue = value;
        return;
    }
    shared_ptr<MemberAccessNode> memberAccess = dynamic_pointer_cast<MemberAccessNode>(node->getAsignee());
    if (memberAccess)
    {
        memberAccess->getExpression()->visit(this);
        if (!returnValue)
        {
            error("member access left-hand side evaluated to null", memberAccess->getExpression()->getRange());
            returnValue = nullptr;
            return;
        }

        const string &memberName = memberAccess->getIdentifier().getValue();
        if (returnValue->getMember(memberName) == nullptr)
        {
            errorAtToken("member '" + memberName + "' does not exist in the object", memberAccess->getIdentifier(), node->getRange());
            returnValue = nullptr;
            return;
        }

        switch (node->getOp())
        {
            case '=':
                // No additional operation, just assign the value
                break;
            case Token::PLUS_EQUAL:
                value = returnValue->getMember(memberName)->add(value);
                break;
            case Token::MINUS_EQUAL:
                value = returnValue->getMember(memberName)->sub(value);
                break;
            case Token::MUL_EQUAL:
                value = returnValue->getMember(memberName)->mul(value);
                break;
            case Token::DIV_EQUAL:
                value = returnValue->getMember(memberName)->div(value);
                break;
            case Token::MOD_EQUAL:
                value = returnValue->getMember(memberName)->mod(value);
                break;
            default:
                error("invalid assignment operator", node->getRange());
                returnValue = nullptr;
                return;
        }

        // assign the value to the specified member
        Result<Value> result = returnValue->setMember(memberName, value);
        if (result.status == Value::MEMBER_IS_CONSTANT)
        {
            errorAtToken("cannot assign to constant member '" + memberName + "'", memberAccess->getIdentifier(), node->getRange());
            returnValue = nullptr;
            return;
        }

        returnValue = result.value;
        return;
    }

    error("invalid assignment target", node->getAsignee()->getRange());
    returnValue = nullptr;
}

void Interpreter::visit(BlockNode *node)
{
    if (!node->getStatements())
    {
        returnValue = nullptr;
        return;
    }

    shared_ptr<Environment> prevEnv = env; // Save previous environment
    shared_ptr<Environment> blockEnv = make_shared<Environment>(env); // Create block environment
    env = blockEnv; // push a new environment for the block
    
    try
    {
        node->getStatements()->visit(this);
    }
    catch (const ReturnException &e)
    {
        env = prevEnv;
        throw;
    }
    catch (const BreakException &e)
    {
        env = prevEnv;
        throw;
    }
    catch (const ContinueException &e)
    {
        env = prevEnv;
        throw;
    }
    catch (const ExitException &e)
    {
        env = prevEnv;
        throw;
    }
    catch (const ErrorException &e)
    {
        env = prevEnv;
        throw;
    }

    // Before exiting block, rewrite function closures to point to parent environment
    // This prevents block environments from being kept alive unnecessarily
    for (const auto& pair : blockEnv->getMembers()) 
    {
        if (pair.second && pair.second->getType() == Value::Type::function) 
        {
            auto func = dynamic_pointer_cast<FunctionValue>(pair.second);
            if (func && func->getEnvironment() == blockEnv) 
            {
                // This function was declared in this block and captures the block environment
                // Rewrite its closure to use the parent environment instead
                func->rewriteClosureEnv(prevEnv);
            }
        }
    }

    env = prevEnv; // pop the environment after visiting the block

    returnValue = nullptr;
}

void Interpreter::visit(IfStatementNode *node)
{
    node->getCondition()->visit(this);
    if (!returnValue)
    {
        return;
    }

    if (returnValue->getType() != Value::Type::boolean &&
        returnValue->getType() != Value::Type::number &&
        returnValue->getType() != Value::Type::string)
    {
        error("condition evaluate to a boolean expression", node->getCondition()->getRange());
        returnValue = nullptr;
        return;
    }

    bool condition = returnValue->toBoolean();

    if (condition && node->getThenBranch())
    {
        // if the condition is true, visit the then branch
        node->getThenBranch()->visit(this);
    }
    else if (!condition && node->getElseBranch())
    {
        // if the condition is false, visit the else branch if it exists
        node->getElseBranch()->visit(this);
    }

    returnValue = nullptr;
}

void Interpreter::visit(FuncDeclNode *node)
{
    /*if (env->lookup(node->getName()))
    {
        alreadyDefined(node);
        return;
    }*/
    auto function = make_shared<FunctionValue>(node->getName(), node->getParams(), node->getBody(), env);
    env->redeclare(node->getName(), function, node->isConst());
    returnValue = nullptr;
}

void Interpreter::visit(WhileNode *node)
{
    while (true)
    {
        node->getCondition()->visit(this);
        if (!returnValue)
        {
            break;
        }

        if (returnValue->getType() != Value::Type::boolean &&
            returnValue->getType() != Value::Type::number)
        {
            error("condition must be a boolean expression", node->getCondition()->getRange());
            returnValue = nullptr;
            break;
        }

        bool condition = returnValue->toBoolean();
        if (!condition) break;

        // don't visit an empty body
        if (node->getBody())
        {
            try
            {
                node->getBody()->visit(this);
            }
            catch (const BreakException &)
            {
                // handle break statement
                break;
            }
            catch (const ContinueException &)
            {
                // handle continue statement
                continue;
            }
        }
    }

    returnValue = nullptr; // reset return value after the loop
}

void Interpreter::visit(ForEachNode *node)
{
    env = make_shared<Environment>(env);

    node->getIterable()->visit(this);

    if (node->isArrayLike())
    {
        if (!returnValue || returnValue->getType() != Value::Type::array)
        {
            error("for-each loop iterable must be an array", node->getIterable()->getRange());
            return;
        }

        auto arrayValue = dynamic_pointer_cast<ArrayValue>(returnValue);
        if (!arrayValue)
        {
            error("for-each loop iterable could not be cast to ArrayValue", node->getIterable()->getRange());
            return;
        }

        for (int i = 0; i < arrayValue->getElementCount(); ++i)
        {
            env->redeclare(node->getKeyDecl()->getName(), arrayValue->getElement(i), node->getKeyDecl()->isConst());

            try
            {
                node->getBody()->visit(this);
            }
            catch (const BreakException &)
            {
                break;
            }
            catch (const ContinueException &)
            {
                continue;
            }
        }

        // Reset the environment after the loo
    }
    else if (node->isMapLike())
    {
        if (!returnValue || returnValue->getType() != Value::Type::object)
        {
            error("for-each loop iterable must be an object", node->getIterable()->getRange());
            return;
        }

        auto objectValue = dynamic_pointer_cast<ObjectValue>(returnValue);
        if (!objectValue)
        {
            error("for-each loop iterable could not be cast to ObjectValue", node->getIterable()->getRange());
            return;
        }

        for (const auto &pair : objectValue->getMembers())
        {
            if (pair.second->getType() == Value::Type::function)
            {
                continue; // skip functions in the object
            }

            if (pair.first == "LINE" || pair.first == "FILE")
            {
                continue; // skip special variables
            }

            env->redeclare(node->getKeyDecl()->getName(), make_shared<StringValue>(pair.first), node->getKeyDecl()->isConst());
            env->redeclare(node->getValueDecl()->getName(), pair.second, node->getValueDecl()->isConst());

            try
            {
                node->getBody()->visit(this);
            }
            catch (const BreakException &)
            {
                break;
            }
            catch (const ContinueException &)
            {
                continue;
            }
        }
    }
    else
    {
        error("for-each loop iterable must be an array or an object", node->getIterable()->getRange());
    }

    env = env->getParent();
    returnValue = nullptr;
}

void Interpreter::visit(ForStatementNode *node)
{
    shared_ptr<Environment> forEnv = make_shared<Environment>(env);
    env = forEnv;
    // Initialize the loop variable
    if (node->getInit())
    {
        node->getInit()->visit(this);
    }

    while (true)
    {
        if (node->getCondition())
        {
            node->getCondition()->visit(this);
            if (!returnValue)
            {
                error("for loop condition evaluation failed", node->getCondition()->getRange());
                break;
            }

            if (returnValue->getType() != Value::Type::boolean &&
                returnValue->getType() != Value::Type::number)
            {
                error("for loop condition must be a boolean expression", node->getCondition()->getRange());
                break;
            }

            bool condition = returnValue->toBoolean();
            if (!condition) break;
        }

        // don't visit an empty body
        if (node->getBody())
        {
            try
            {
                node->getBody()->visit(this);
            }
            catch (const BreakException &)
            {
                // handle break statement
                break;
            }
            catch (const ContinueException &)
            {
                // handle continue statement
                continue;
            }
        }

        if (node->getIncrement())
        {
            node->getIncrement()->visit(this);
        }
    }

    // Reset the environment after the loop
    env = env->getParent();
    returnValue = nullptr; // reset return value after the loop
}

void Interpreter::visit(NullNode *node)
{
    UNUSED(node);
    returnValue = make_shared<NullValue>();
}

void Interpreter::visit(BreakNode *node)
{
    UNUSED(node);
    throw BreakException(node->getRange());
}

void Interpreter::visit(ImportNode *node)
{
    const string &moduleName = node->getToken().getValue();
    if (moduleName.empty())
    {
        error("imported module name is empty", node->getToken().getRange());
        return;
    }

    // Attempt to import the module
    import(node->getToken(), node->getRange());
}

void Interpreter::visit(ArrayNode *node)
{
    vector<shared_ptr<Value>> elements;
for (const auto &element : node->getElements())
    {
        element->visit(this);
        if (!returnValue)
        {
            return;
        }
        elements.push_back(returnValue);
    }

    returnValue = make_shared<ArrayValue>(elements, node->getRange());
}

void Interpreter::visit(AssertNode *node)
{
    node->getCondition()->visit(this);
    if (!returnValue)
    {
        error("assertion condition evaluated to null", node->getCondition()->getRange());
        return;
    }

    auto condition = returnValue;

    if (condition->getType() != Value::Type::boolean &&
        condition->getType() != Value::Type::number &&
        condition->getType() != Value::Type::string)
    {
        error("assertion condition must be a boolean expression", node->getCondition()->getRange());
        return;
    }

    auto message = node->getMessage();
    string messageStr = "";
    if (message)
    {
        message->visit(this);
        if (returnValue)
        {
            messageStr = returnValue->toString();
        }
    }

    bool conditionValue = condition->toBoolean();
    if (!conditionValue)
    {
        error("assertion failed: " + messageStr, node->getCondition()->getRange());
    }
}

void Interpreter::visit(ArrayAccessNode *node)
{
    node->getArray()->visit(this);
    if (!returnValue)
    {
        error("array access left-hand side evaluated to null", node->getArray()->getRange());
        return;
    }

    if (returnValue->getType() == Value::Type::array)
    {
        auto arrayValue = dynamic_pointer_cast<ArrayValue>(returnValue);
        if (!arrayValue)
        {
            error("left-hand side of array access could not be cast to ArrayValue", node->getArray()->getRange());
            returnValue = nullptr;
            return;
        }

        node->getIndex()->visit(this);
        if (!returnValue)
        {
            error("array access index evaluated to null", node->getIndex()->getRange());
            return;
        }

        if (returnValue->getType() != Value::Type::number)
        {
            error("array access index must be a number", node->getIndex()->getRange());
            returnValue = nullptr;
            return;
        }

        auto indexValue = dynamic_pointer_cast<NumberValue>(returnValue);
        int index = indexValue ? static_cast<int>(indexValue->getValue()) : 0;

        if (index < 0 || index >= static_cast<int>(arrayValue->getElementCount()))
        {
            error("array index out of bounds: " + to_string(index), node->getIndex()->getRange());
            returnValue = nullptr;
            return;
        }

        returnValue = arrayValue->getElement(index);
        return;
    }

    if (returnValue->getType() == Value::Type::string)
    {
        auto stringValue = dynamic_pointer_cast<StringValue>(returnValue);
        if (!stringValue)
        {
            error("left-hand side of array access could not be cast to StringValue", node->getArray()->getRange());
            returnValue = nullptr;
            return;
        }

        node->getIndex()->visit(this);
        if (!returnValue)
        {
            error("array access index evaluated to null", node->getIndex()->getRange());
            return;
        }

        if (returnValue->getType() != Value::Type::number)
        {
            error("array access index must be a number", node->getIndex()->getRange());
            returnValue = nullptr;
            return;
        }

        auto indexValue = dynamic_pointer_cast<NumberValue>(returnValue);
        int index = indexValue ? static_cast<int>(indexValue->getValue()) : 0;

        if (index < 0 || index >= static_cast<int>(stringValue->length()))
        {
            error("string index out of bounds: " + to_string(index), node->getIndex()->getRange());
            returnValue = nullptr;
            return;
        }

        returnValue = make_shared<StringValue>(stringValue->getCharAt(index));
        return;
    }
    else
    {
        error("left-hand side of array access is not an array or string", node->getArray()->getRange());
        returnValue = nullptr;
    }
}

void Interpreter::visit(MemberAccessNode *node)
{
    node->getExpression()->visit(this);
    if (!returnValue)
    {
        error("member access left-hand side evaluated to null", node->getExpression()->getRange());
        return;
    }

    auto lhs = returnValue;
    auto member = returnValue->getMember(node->getIdentifier().getValue());
    if (member && member->getType() == Value::Type::builtin)
    {
        // if the member is a builtin function, we need to bind it to the left-hand side so it can access the object it belongs to.
        auto builtin = dynamic_pointer_cast<BuiltinFunctionValue>(member);
        if (!builtin)
        {
            returnValue = nullptr;
            return;
        }

        returnValue = builtin->bind(lhs);
        return;
    }

    returnValue = member;

    if (!returnValue)
    {
        errorAtToken("member '" + node->getIdentifier().getValue() + "' not found", node->getIdentifier(), node->getRange());
        return;
    }
}

void Interpreter::visit(ContinueNode *node)
{
    UNUSED(node);
    throw ContinueException(node->getRange());
}

void Interpreter::visit(ClassNode *node)
{
    // Create a new class value and declare it in the environment
    auto classValue = make_shared<ClassValue>(node->getName(), node->getBody());
    auto result = env->declare(node->getName(), classValue, node->isConst());
    if (!result)
    {
        alreadyDefined(node);
        return;
    }

    returnValue = nullptr; // No return value for class declaration
}