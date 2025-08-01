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

using std::cout;
using std::dynamic_pointer_cast;
using std::endl;
using std::exception;
using std::fmod;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::vector;

#define MAX_PEEK 25

#define error(msg, range)                       \
    rangeError(msg, range, __FILE__, __LINE__); \
    hadError = true;                            \
    throw ErrorException(msg, range)

#define errorAt(msg, location, range)                             \
    locationRangeError(msg, location, range, __FILE__, __LINE__); \
    hadError = true;                                              \
    throw ErrorException(msg, range)

#define errorAtToken(msg, token, range)                     \
    tokenRangeError(msg, token, range, __FILE__, __LINE__); \
    hadError = true;                                        \
    throw ErrorException(msg, range)

#define alreadyDefined(node) \
    errorAtToken("'" + Utils::truncate(node->getName(), MAX_PEEK) + "' is already defined", node->getToken(), node->getRange())

#define notDefined(node) \
    errorAtToken("'" + Utils::truncate(node->getName(), MAX_PEEK) + "' is not defined", node->getToken(), node->getRange())

#define failedToLoadModule(token, range)                                            \
    errorAtToken("failed to load module '" + token.getValue() + "'", token, range); \
    return false

#define couldNotFindModule(token, range)                                            \
    errorAtToken("could not find module '" + token.getValue() + "'", token, range); \
    return false

Interpreter::Interpreter(bool isInteractive, shared_ptr<Environment> env, const vector<string> &args):
    isInteractive(isInteractive),
    hadError(false),
    returnValue(nullptr),
    moduleParser(),
    importedModules(),
    args(args),
    currentFunctionName(""),
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
    // Run aggressive cleanup before destroying the interpreter
    aggressiveCleanup();

    // Break circular references by clearing environment
    if (env)
    {
        // Clear all environments in the chain to break cycles
        auto current = env;
        while (current)
        {
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

    // Reset error state for each interpretation
    hadError = false;

    // Reset the return value for each interpretation
    returnValue.reset();

    // Visit the node to interpret it
    visitAllChildren(node);

    // Cleanup temporary environments at the end of interpretation
    finalCleanup();

    return !hadError;
}

void Interpreter::cleanupTempEnvironments()
{
    // Incremental cleanup during execution - safe to clean environments
    // that are no longer referenced by active code
    for (auto it = tempEnvironments.begin(); it != tempEnvironments.end();)
    {
        auto &tempEnv = *it;
        if (tempEnv)
        {
            long refCount = tempEnv.use_count();

            // During execution, only clean environments with very minimal references
            // Be more conservative than final cleanup to avoid breaking active closures
            if (refCount <= 1) // Only in tempEnvironments vector = definitely unused
            {
                tempEnv->clear();
                it = tempEnvironments.erase(it);
            }
            else
            {
                ++it;
            }
        }
        else
        {
            it = tempEnvironments.erase(it);
        }
    }
}

void Interpreter::finalCleanup()
{
    for (auto it = tempEnvironments.begin(); it != tempEnvironments.end();)
    {
        auto &tempEnv = *it;
        if (tempEnv)
        {
            long refCount = tempEnv.use_count();

            // Conservative cleanup: only clean environments with very few references
            // Much more conservative - only clean environments with count 1
            // This ensures ObjectValue environments are never cleaned
            if (refCount == 1)  // Only clean truly unused environments
            {
                tempEnv->clear();
                it = tempEnvironments.erase(it);
            }
            else
            {
                ++it;
            }
        }
        else
        {
            it = tempEnvironments.erase(it);
        }
    }
}

void Interpreter::aggressiveCleanup()
{
    // Final cleanup that runs only at the very end
    // This is more aggressive than finalCleanup() but still preserves ObjectValue environments

    for (auto it = tempEnvironments.begin(); it != tempEnvironments.end();)
    {
        auto &tempEnv = *it;
        if (tempEnv)
        {
            // More aggressive cleanup - only preserve environments with very high reference counts
            // which are likely to be ObjectValue environments that are still in use
            long refCount = tempEnv.use_count();

            // Only preserve environments that are very actively referenced
            // This allows us to clean up function call environments and other temporary scopes
            if (refCount <= 3)  // More aggressive than finalCleanup
            {
                tempEnv->clear();
                it = tempEnvironments.erase(it);
            }
            else
            {
                ++it;
            }
        }
        else
        {
            it = tempEnvironments.erase(it);
        }
    }
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
    env->declare("string", make_shared<BuiltinFunctionValue>(Builtins::toString), true);
    env->declare("print", make_shared<BuiltinFunctionValue>(Builtins::print), true);
    env->declare("println", make_shared<BuiltinFunctionValue>(Builtins::println), true);
    env->declare("env", make_shared<BuiltinFunctionValue>(Builtins::dumpenv), true);
    env->declare("dumpstack", make_shared<BuiltinFunctionValue>(Builtins::dumpstack), true);
}

void Interpreter::setupRuntimeValues()
{
    declare("__version__", INTERPRETER_VERSION, true);
}

bool Interpreter::import(const Token &moduleName, const Range &range)
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
            env->declare("listdir", make_shared<BuiltinFunctionValue>(Builtins::listdir), true);
            env->declare("close", make_shared<BuiltinFunctionValue>(Builtins::closeFd), true);
            env->declare("read", make_shared<BuiltinFunctionValue>(Builtins::readFd), true);
            env->declare("write", make_shared<BuiltinFunctionValue>(Builtins::writeFd), true);
            env->declare("shell", make_shared<BuiltinFunctionValue>(Builtins::runShellCommand), true);
            env->declare("getcwd", make_shared<BuiltinFunctionValue>(Builtins::getcwd), true);
            env->declare("chdir", make_shared<BuiltinFunctionValue>(Builtins::chdir), true);
            env->declare("getpid", make_shared<BuiltinFunctionValue>(Builtins::getpid), true);
            env->declare("getuser", make_shared<BuiltinFunctionValue>(Builtins::getuser), true);
            env->declare("getenv", make_shared<BuiltinFunctionValue>(Builtins::getenv), true);
            imported(module);
            return true;
        }
        else if (module == "socket")
        {
            env->declare("socket", make_shared<BuiltinFunctionValue>(Builtins::socket), true);
            env->declare("close", make_shared<BuiltinFunctionValue>(Builtins::closeFd), true);
            env->declare("listen", make_shared<BuiltinFunctionValue>(Builtins::listenSocket), true);
            env->declare("accept", make_shared<BuiltinFunctionValue>(Builtins::acceptSocket), true);
            env->declare("connect", make_shared<BuiltinFunctionValue>(Builtins::connectSocket), true);
            env->declare("send", make_shared<BuiltinFunctionValue>(Builtins::sendSocket), true);
            env->declare("receive", make_shared<BuiltinFunctionValue>(Builtins::receiveSocket), true);
            // env->declare("sock_addr", make_shared<BuiltinFunctionValue>(Builtins::getSocketAddress), true);
            // env->declare("sock_port", make_shared<BuiltinFunctionValue>(Builtins::getSocketPort), true);
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
        else if (module == "color")
        {
            shared_ptr<ObjectValue> colorObject = make_shared<ObjectValue>("Color", make_shared<Environment>());
            colorObject->addMember("red", make_shared<StringValue>("\033[31m"), true);
            colorObject->addMember("green", make_shared<StringValue>("\033[32m"), true);
            colorObject->addMember("yellow", make_shared<StringValue>("\033[33m"), true);
            colorObject->addMember("blue", make_shared<StringValue>("\033[34m"), true);
            colorObject->addMember("magenta", make_shared<StringValue>("\033[35m"), true);
            colorObject->addMember("cyan", make_shared<StringValue>("\033[36m"), true);
            colorObject->addMember("white", make_shared<StringValue>("\033[37m"), true);
            colorObject->addMember("reset", make_shared<StringValue>("\033[0m"), true);
            colorObject->addMember("rgb", make_shared<BuiltinFunctionValue>(Builtins::rgb), true);
            env->declare("Color", colorObject, true);

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
            auto function = make_shared<FunctionValue>(
                funcDecl->getName(),
                funcDecl->getParams(),
                funcDecl->getBody(),
                env);
            env->declare(funcDecl->getName(), function, funcDecl->isConst());
        }
    }

    // Second pass: execute all statements
    for (auto &statement : node->getStatements())
    {
        returnValue = nullptr;
        if (!statement)
            continue;
        if (hadError)
            break;
        statement->visit(this);
    }

    // In interactive mode, print only the last non-undefined (nullptr) result
    if (returnValue && isInteractive)
    {
        cout << returnValue->toString() << endl;
    }

    // Set the final return value to nullptr as 'statements' do not return a value
    returnValue = nullptr;
}

void Interpreter::visit(NumberNode *node)
{
    // Keep the actual range for error reporting purposes
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
    auto left = node->getLeft();
    auto opNode = node->getOperator();
    auto right = node->getRight();

    left->visit(this);
    if (hadError)
    {
        returnValue = nullptr;
        return;
    }
    auto leftValue = returnValue;
    if (!leftValue)
    {
        error("left operand of binary expression is null", left->getRange());
    }

    // short-circuit evaluation for logical operators
    if (opNode->getType() == Token::AND && leftValue->toBoolean() == false)
    {
        returnValue = leftValue;
        return;
    }
    else if (opNode->getType() == Token::OR && leftValue->toBoolean() == true)
    {
        returnValue = leftValue;
        return;
    }

    right->visit(this);
    if (hadError)
    {
        returnValue = nullptr;
        return;
    }
    auto rightValue = returnValue;
    if (!rightValue)
    {
        error("right operand of binary expression is null", right->getRange());
    }

    returnValue = nullptr;
    switch (opNode->getType())
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
        errorAt("unsupported binary operation: " + opNode->getToken().getValue(), opNode->getRange().getStart(), node->getRange());
    }

    if (returnValue && returnValue->getType() != Value::Type::null)
    {
        return;
    }

    if (returnValue && returnValue->getType() == Value::Type::null)
    {
        cout << "Warning: binary operation resulted in null value" << endl;
        // If the operation resulted in a null value, we can return it as is
        returnValue = nullptr;
        return;
    }

    errorAt("unsupported operation between " + leftValue->typeAsString() + " and " + rightValue->typeAsString(), opNode->getRange().getStart(), node->getRange());
}

void Interpreter::visit(UnaryExprNode *node)
{
    returnValue = evalUnaryExpression(node->getExpression(), node->getOperator(), node->isPrefix());
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
    if (!value && opNode->getType() != '?')
    {
        error("unary expression evaluation failed", expression->getRange());
        return nullptr;
    }

    switch (opNode->getType())
    {
    case Token::INC:
    case Token::DEC:
    {
        if (!expression->isLval())
        {
            error("expected a modifiable expression", expression->getRange());
            return nullptr;
        }

        return evalIncrementDecrement(expression, opNode, prefix);
    }
    case '!':
        returnValue = value->unaryNot();
        break;
    case '-':
        returnValue = value->unaryMinus();
        break;
    case '?':
        if (returnValue)
        {
            return returnValue;
        }
        else
        {
            return make_shared<NullValue>(opNode->getRange()); // If the value is undefined, return a NullValue instead
        }
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

shared_ptr<Value> Interpreter::evalIncrementDecrement(shared_ptr<ExpressionNode> expression, shared_ptr<OpNode> opNode, bool prefix)
{
    // Get current value for return (prefix or postfix)
    expression->visit(this);
    auto currentVal = returnValue;
    if (!currentVal)
    {
        error("Cannot increment/decrement null value", expression->getRange());
        return nullptr;
    }

    // For postfix, we return the original value
    shared_ptr<Value> returnVal = prefix ? nullptr : currentVal;

    // Perform the increment/decrement
    shared_ptr<Value> newVal;
    if (opNode->getType() == Token::INC)
    {
        if (currentVal->getType() == Value::Type::number)
        {
            auto numberValue = dynamic_pointer_cast<NumberValue>(currentVal);
            newVal = make_shared<NumberValue>(numberValue->getValue() + 1);
        }
        else
        {
            error("Cannot increment non-numeric value", expression->getRange());
            return nullptr;
        }
    }
    else if (opNode->getType() == Token::DEC)
    {
        if (currentVal->getType() == Value::Type::number)
        {
            auto numberValue = dynamic_pointer_cast<NumberValue>(currentVal);
            newVal = make_shared<NumberValue>(numberValue->getValue() - 1);
        }
        else
        {
            error("Cannot decrement non-numeric value", expression->getRange());
            return nullptr;
        }
    }
    else
    {
        error("Unsupported increment/decrement operator", opNode->getRange());
        return nullptr;
    }

    // Assign the new value back
    if (auto varExpr = dynamic_pointer_cast<VarExprNode>(expression))
    {
        env->assign(varExpr->getName(), newVal);
    }
    else if (auto arrayAccess = dynamic_pointer_cast<ArrayAccessNode>(expression))
    {
        auto arrayVal = arrayAccess->getArray();
        arrayVal->visit(this);
        auto arrayValue = returnValue;

        auto indexVal = arrayAccess->getIndex();
        indexVal->visit(this);
        auto indexValue = returnValue;

        if (auto arrayObj = dynamic_pointer_cast<ArrayValue>(arrayValue))
        {
            if (auto numberIdx = dynamic_pointer_cast<NumberValue>(indexValue))
            {
                int idx = (int)numberIdx->getValue();
                if (idx >= 0 && idx < (int)arrayObj->getElements().size())
                {
                    arrayObj->setElement(idx, newVal);
                }
                else
                {
                    error("Array index out of bounds", expression->getRange());
                    return nullptr;
                }
            }
            else
            {
                error("Array index must be a number", expression->getRange());
                return nullptr;
            }
        }
        else
        {
            error("Cannot index non-array value", expression->getRange());
            return nullptr;
        }
    }
    else if (auto memberAccess = dynamic_pointer_cast<MemberAccessNode>(expression))
    {
        auto objVal = memberAccess->getExpression();
        objVal->visit(this);
        auto objValue = returnValue;

        if (objValue)
        {
            auto memberName = memberAccess->getIdentifier().getValue();
            Result<Value> result = objValue->setMember(memberName, newVal);
            if (result.status == Value::MEMBER_IS_CONSTANT)
            {
                error("cannot assign to constant member '" + memberName + "'", expression->getRange());
                return nullptr;
            }
        }
        else
        {
            error("Cannot access member of null value", expression->getRange());
            return nullptr;
        }
    }
    else
    {
        error("Invalid lvalue for increment/decrement", expression->getRange());
        return nullptr;
    }

    // For prefix, return the new value; for postfix, return the original value
    return prefix ? newVal : returnVal;
}

bool Interpreter::validateFunctionArguments(shared_ptr<FunctionValue> function, const vector<shared_ptr<Value>>& args, const Range& nodeRange, const string& functionType)
{
    if (function->getParameters() && (size_t)function->getParameters()->getParamCount() != args.size())
    {
        error(functionType + " '" + function->getName() + "' expects " +
              to_string(function->getParameters()->getParamCount()) +
              " arguments, but got " + to_string(args.size()),
              nodeRange);
        return false;
    }
    else if (!function->getParameters() && !args.empty())
    {
        error(functionType + " '" + function->getName() + "' does not take any arguments, but got " + to_string(args.size()), nodeRange);
        return false;
    }
    return true;
}

shared_ptr<Value> Interpreter::callUserFunction(shared_ptr<FunctionValue> function, const vector<shared_ptr<Value>>& args, const Range& nodeRange)
{
    // Check recursion depth to prevent stack overflow
    if (recursionDepth >= MAX_RECURSION_DEPTH)
    {
        error("maximum recursion depth exceeded (" + to_string(MAX_RECURSION_DEPTH) + ")", nodeRange);
        return nullptr;
    }

    if (!validateFunctionArguments(function, args, nodeRange))
    {
        return nullptr;
    }

    if (!function->getBody())
    {
        error("function '" + function->getName() + "' has no body", nodeRange);
        return nullptr;
    }

    // Set up function call environment
    shared_ptr<Environment> scope = make_shared<Environment>(function->getEnvironment());
    for (size_t i = 0; i < args.size(); ++i)
    {
        scope->declare(function->getParameters()->getParam(i)->getName(), args[i]);
    }

    auto previousEnv = env;
    env = scope;

    // Track this function call scope for cleanup
    tempEnvironments.push_back(scope);

    // Increment recursion depth and update function name
    recursionDepth++;
    string oldFunctionName = currentFunctionName;
    currentFunctionName = function->getName();

    shared_ptr<Value> result = nullptr;
    try
    {
        callStack.push(currentFunctionName, function->getRange());
        function->getBody()->visit(this);
        result = nullptr; // Normal function completion without return
    }
    catch (const ReturnException &e)
    {
        result = e.value;
    }
    catch (const BaseException &e)
    {
        // Restore state before re-throwing
        recursionDepth--;
        currentFunctionName = oldFunctionName;
        env = previousEnv;
        callStack.pop();
        throw;
    }

    callStack.pop();

    // Restore state
    recursionDepth--;
    currentFunctionName = oldFunctionName;

    // Handle closure cleanup for returned functions
    if (result && result->getType() == Value::Type::function)
    {
        auto returnedFunc = dynamic_pointer_cast<FunctionValue>(result);
        if (returnedFunc && returnedFunc->getEnvironment() == scope)
        {
            // Break circular reference
            returnedFunc->clearClosureEnv();
        }
    }

    env = previousEnv;
    return result;
}

shared_ptr<Value> Interpreter::callClassConstructor(shared_ptr<ClassValue> classValue, const vector<shared_ptr<Value>>& args, const Range& nodeRange)
{
    // Visit the declarations of the class and capture them in a new environment
    auto previousEnv = env;
    shared_ptr<Environment> classEnv = make_shared<Environment>(env);
    env = classEnv;

    // Track this class environment for cleanup
    tempEnvironments.push_back(classEnv);

    auto cleanupAndReturn = [&](shared_ptr<Value> result = nullptr) -> shared_ptr<Value>
    {
        if (result == nullptr)
        {
            classEnv->clear();
        }
        env = previousEnv;
        return result;
    };

    try
    {
        shared_ptr<BlockNode> classBody = dynamic_pointer_cast<BlockNode>(classValue->getBody());
        if (!classBody)
        {
            error("class '" + classValue->getName() + "' has no body", nodeRange);
            return cleanupAndReturn();
        }

        // Visit the class body to populate the environment
        if (classBody->getStatements())
        {
            classBody->getStatements()->visit(this);
        }

        // Look for and call the constructor
        auto ctor = env->lookupLocal(classValue->getName());
        if (ctor)
        {
            if (ctor->getType() != Value::Type::function)
            {
                error("class '" + classValue->getName() + "' has no constructor", nodeRange);
                return cleanupAndReturn();
            }

            shared_ptr<FunctionValue> constructor = dynamic_pointer_cast<FunctionValue>(ctor);
            if (!constructor)
            {
                error("class '" + classValue->getName() + "' constructor could not be cast to FunctionValue", nodeRange);
                return cleanupAndReturn();
            }

            if (!validateFunctionArguments(constructor, args, nodeRange, "constructor"))
            {
                return cleanupAndReturn();
            }

            if (!constructor->getBody())
            {
                error("constructor '" + constructor->getName() + "' has no body", nodeRange);
                return cleanupAndReturn();
            }

            // Set up constructor environment
            shared_ptr<Environment> scope = make_shared<Environment>(constructor->getEnvironment());
            for (size_t i = 0; i < args.size(); ++i)
            {
                scope->declare(constructor->getParameters()->getParam(i)->getName(), args[i]);
            }

            auto constructorPrevEnv = env;
            env = scope;

            try
            {
                callStack.push(constructor->getName(), constructor->getRange());
                constructor->getBody()->visit(this);
            }
            catch (const ReturnException &e)
            {
                callStack.pop();
                if (e.value && e.value->getType() != Value::Type::null)
                {
                    error("constructor of class '" + classValue->getName() + "' returned a value, which is not allowed", nodeRange);
                }
            }
            catch (const BaseException &e)
            {
                callStack.pop();
                env = constructorPrevEnv;
                throw;
            }
            callStack.pop();

            env = constructorPrevEnv;
        }

        // Create and return the class instance
        // Optimize memory usage: if the class environment has no local variables,
        // use nullptr instead of creating an environment
        shared_ptr<Environment> instanceEnv = env;
        if (env->getMembers().empty() && env->getParent() == previousEnv)
        {
            instanceEnv = nullptr; // Use nullptr for empty objects to save maximum memory
        }
        
        auto instance = make_shared<ObjectValue>(classValue->getName(), instanceEnv);
        env = previousEnv;
        return instance;

    }
    catch (const BaseException &e)
    {
        return cleanupAndReturn();
    }
}

shared_ptr<Value> Interpreter::declare(const string &name, const string &value, bool constant)
{
    return env->declare(name, make_shared<StringValue>(value), constant);
}

shared_ptr<Value> Interpreter::declare(const string &name, const bool &value, bool constant)
{
    return env->declare(name, make_shared<BooleanValue>(value), constant);
}

shared_ptr<Value> Interpreter::declare(const string &name, const double &value, bool constant)
{
    return env->declare(name, make_shared<NumberValue>(value), constant);
}

shared_ptr<Value> Interpreter::evalVariableUnaryExpression(shared_ptr<VarExprNode> expression, shared_ptr<OpNode> opNode, bool prefix)
{
    auto value = env->lookup(expression->getName());
    if (!value)
    {
        error("variable " + expression->getName() + " is not defined", expression->getRange());
        return nullptr;
    }

    switch (opNode->getType())
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
    // Evaluate arguments
    vector<shared_ptr<Value>> args;
    if (node->getArgs())
    {
        for (auto &arg : node->getArgs()->getArgs())
        {
            if (hadError)
                break; // Stop processing arguments if an error occurred
            arg->visit(this);
            args.push_back(returnValue);
        }
    }

    // Evaluate the callee
    auto calleeNode = node->getCallee();
    calleeNode->visit(this);
    if (hadError)
    {
        returnValue = nullptr;
        return;
    }
    if (!returnValue)
    {
        return;
    }

    shared_ptr<Value> callee = returnValue;

    // Handle different types of callables
    if (callee->getType() == Value::Type::function)
    {
        shared_ptr<FunctionValue> function = dynamic_pointer_cast<FunctionValue>(callee);
        if (!function)
        {
            error("callee could not be cast to FunctionValue", calleeNode->getRange());
            returnValue = nullptr;
            return;
        }

        returnValue = callUserFunction(function, args, node->getRange());
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

        // For member access (e.g., obj.method()), use the identifier range to point to the method name
        Range callRange = calleeNode->getRange();
        if (auto memberAccess = dynamic_cast<MemberAccessNode *>(calleeNode.get()))
        {
            callRange = memberAccess->getIdentifier().getRange();
        }

        callStack.push(callee->toString(), callRange);
        returnValue = builtin->call(*this, args, env, callRange);
        callStack.pop();
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

        returnValue = callClassConstructor(classValue, args, node->getRange());
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
    if (!expr)
    {
        throw ReturnException(nullptr, node->getRange());
    }

    expr->visit(this);
    if (!returnValue)
    {
        throw ReturnException(make_shared<NullValue>(), node->getRange());
    }
    throw ReturnException(returnValue, node->getRange());
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

        // If an error occurred during expression evaluation, don't declare the variable
        if (hadError)
        {
            returnValue = nullptr;
            return;
        }
    }

    if (!returnValue)
    {
        errorAt("cannot declare a variable with no value", node->getExpr()->getRange().getStart(), node->getRange());
        return;
    }

    // Check for redeclaration - this catches built-in constants and imported variables
    auto result = env->declare(node->getName(), std::move(returnValue), node->isConst());
    if (!result)
    {
        errorAt("variable '" + node->getName() + "' is already declared in this scope", node->getToken().getRange().getStart(), node->getRange());
        return;
    }

    returnValue = nullptr;
}

void Interpreter::visit(VarExprNode *node)
{
    // Intercept special variables
    if (node->getName() == "__file__")
    {
        returnValue = make_shared<StringValue>(node->getRange().getStart().getFilename(), node->getRange());
        return;
    }

    if (node->getName() == "__line__")
    {
        returnValue = make_shared<NumberValue>(node->getRange().getStart().getLine(), node->getRange());
        return;
    }

    if (node->getName() == "__function__")
    {
        returnValue = make_shared<StringValue>(currentFunctionName, node->getRange());
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
    if (hadError)
    {
        returnValue = nullptr;
        return;
    }
    shared_ptr<Value> value = returnValue;
    if (!value)
    {
        error("assignment does not have a value", node->getExpr()->getRange());
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
            {
                // No additional operation, just assign the value
                break;
            }
            case Token::PLUS_EQUAL:
            {
                auto existingValue = env->lookup(asignee->getName());
                if (!existingValue)
                {
                    notDefined(asignee);
                    returnValue = nullptr;
                    return;
                }
                value = existingValue->add(value);
                break;
            }
            case Token::MINUS_EQUAL:
            {
                auto existingValue = env->lookup(asignee->getName());
                if (!existingValue)
                {
                    notDefined(asignee);
                    returnValue = nullptr;
                    return;
                }
                value = existingValue->sub(value);
                break;
            }
            case Token::MUL_EQUAL:
            {
                auto existingValue = env->lookup(asignee->getName());
                if (!existingValue)
                {
                    notDefined(asignee);
                    returnValue = nullptr;
                    return;
                }
                value = existingValue->mul(value);
                break;
            }
            case Token::DIV_EQUAL:
            {
                auto existingValue = env->lookup(asignee->getName());
                if (!existingValue)
                {
                    notDefined(asignee);
                    returnValue = nullptr;
                    return;
                }
                value = existingValue->div(value);
                break;
            }
            case Token::MOD_EQUAL:
            {
                auto existingValue = env->lookup(asignee->getName());
                if (!existingValue)
                {
                    notDefined(asignee);
                    returnValue = nullptr;
                    return;
                }
                value = existingValue->mod(value);
                break;
            }
            default:
            {
                error("invalid assignment operator", node->getRange());
                returnValue = nullptr;
                return;
            }
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
            {
                opName = "add";
                preposition = "to";
                leftType = returnValue->typeAsString();
                rightType = assigneeType;
                break;
            }
            case Token::MINUS_EQUAL:
            {
                opName = "subtract";
                preposition = "from";
                leftType = returnValue->typeAsString();
                rightType = assigneeType;
                break;
            }
            case Token::MUL_EQUAL:
            {
                opName = "multiply";
                preposition = "with";
                leftType = assigneeType;
                rightType = returnValue->typeAsString();
                break;
            }
            case Token::DIV_EQUAL:
            {
                opName = "divide";
                preposition = "by";
                leftType = assigneeType;
                rightType = returnValue->typeAsString();
                break;
            }
            case Token::MOD_EQUAL:
            {
                opName = "mod";
                preposition = "by";
                leftType = assigneeType;
                rightType = returnValue->typeAsString();
                break;
            }
            default:
            {
                opName = "operate";
                preposition = "with";
                leftType = assigneeType;
                rightType = returnValue->typeAsString();
                break;
            }
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
            error("array index out of bounds: " + to_string(index) + " for array of length: " + to_string(arrayValue->getElementCount()), access->getIndex()->getRange());
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

    shared_ptr<Environment> prevEnv = env;                            // Save previous environment
    shared_ptr<Environment> blockEnv = make_shared<Environment>(env); // Create block environment
    env = blockEnv;                                                   // push a new environment for the block

    // Track this as a temporary environment for later cleanup
    tempEnvironments.push_back(blockEnv);

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

    env = prevEnv; // pop the environment after visiting the block

    // Clean up temporary environments created during this block
    cleanupTempEnvironments();

    // Explicitly clear the block environment to trigger immediate cleanup
    // This is important for blocks with hoisted functions that create reference cycles
    blockEnv->clear();

    returnValue = nullptr;
}

void Interpreter::visit(BooleanNode *node)
{
    returnValue = make_shared<BooleanValue>(node->getValue());
}

void Interpreter::visit(IfStatementNode *node)
{
    node->getCondition()->visit(this);
    if (!returnValue)
    {
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
    auto function = make_shared<FunctionValue>(node->getName(), node->getParams(), node->getBody(), env, node->getRange());
    env->redeclare(node->getName(), function, node->isConst());
    returnValue = nullptr;
}

void Interpreter::visit(WhileNode *node)
{
    shared_ptr<Environment> originalEnv = env;

    while (true)
    {
        // Evaluate condition in the original environment
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
        if (!condition)
            break;

        // Create a new environment for this iteration
        {
            shared_ptr<Environment> iterationEnv = make_shared<Environment>(originalEnv);
            env = iterationEnv;

            // don't visit an empty body
            if (node->getBody())
            {
                try
                {
                    node->getBody()->visit(this);
                }
                catch (const BreakException &)
                {
                    env = originalEnv;
                    break;
                }
                catch (const ContinueException &)
                {
                    env = originalEnv;
                    continue;
                }
                catch (const ReturnException &e)
                {
                    env = originalEnv;
                    throw;
                }
                catch (const ErrorException &e)
                {
                    env = originalEnv;
                    throw;
                }
            }

            // Clear any references that might be holding onto values
            returnValue = nullptr;

            // Clean up temporary environments created during this iteration
            cleanupTempEnvironments();

            // Explicitly clear the iteration environment to trigger immediate cleanup
            iterationEnv->clear();

            // Restore original environment - iterationEnv goes out of scope here
            env = originalEnv;
        } // iterationEnv is destroyed here, allowing immediate cleanup
    }

    returnValue = nullptr; // reset return value after the loop
}

void Interpreter::visit(ForEachNode *node)
{
    shared_ptr<Environment> originalEnv = env;

    // Evaluate the iterable expression in the original environment
    node->getIterable()->visit(this);

    if (node->isArrayLike())
    {
        if (!returnValue || (returnValue->getType() != Value::Type::array && returnValue->getType() != Value::Type::string))
        {
            error("for-each loop iterable must be an array or string", node->getIterable()->getRange());
            return;
        }

        if (returnValue->getType() == Value::Type::array)
        {
            auto arrayValue = dynamic_pointer_cast<ArrayValue>(returnValue);
            if (!arrayValue)
            {
                error("for-each loop iterable could not be cast to ArrayValue", node->getIterable()->getRange());
                return;
            }

            for (int i = 0; i < arrayValue->getElementCount(); ++i)
            {
                // Create a new environment for this iteration
                {
                    shared_ptr<Environment> iterationEnv = make_shared<Environment>(originalEnv);
                    env = iterationEnv;

                    env->redeclare(node->getKeyDecl()->getName(), arrayValue->getElement(i), node->getKeyDecl()->isConst());

                    try
                    {
                        if (node->getBody())
                        {
                            node->getBody()->visit(this);
                        }
                    }
                    catch (const BreakException &)
                    {
                        env = originalEnv;
                        break;
                    }
                    catch (const ContinueException &)
                    {
                        env = originalEnv;
                        continue;
                    }
                    catch (const ReturnException &e)
                    {
                        env = originalEnv;
                        throw;
                    }
                    catch (const ErrorException &e)
                    {
                        env = originalEnv;
                        throw;
                    }

                    // Clear any references that might be holding onto values
                    returnValue = nullptr;

                    // Environment is automatically cleaned up when iterationEnv goes out of scope
                    env = originalEnv;
                } // iterationEnv is destroyed here, allowing immediate cleanup
            }
        }
        else if (returnValue->getType() == Value::Type::string)
        {
            auto stringValue = dynamic_pointer_cast<StringValue>(returnValue);
            if (!stringValue)
            {
                error("for-each loop iterable could not be cast to StringValue", node->getIterable()->getRange());
                return;
            }

            for (int i = 0; i < static_cast<int>(stringValue->length()); ++i)
            {
                // Create a new environment for this iteration
                {
                    shared_ptr<Environment> iterationEnv = make_shared<Environment>(originalEnv);
                    env = iterationEnv;

                    // Create a StringValue for the character at position i
                    auto charValue = make_shared<StringValue>(stringValue->getCharAt(i));
                    env->redeclare(node->getKeyDecl()->getName(), charValue, node->getKeyDecl()->isConst());

                    try
                    {
                        if (node->getBody())
                        {
                            node->getBody()->visit(this);
                        }
                    }
                    catch (const BreakException &)
                    {
                        env = originalEnv;
                        break;
                    }
                    catch (const ContinueException &)
                    {
                        env = originalEnv;
                        continue;
                    }
                    catch (const ReturnException &e)
                    {
                        env = originalEnv;
                        throw;
                    }
                    catch (const ErrorException &e)
                    {
                        env = originalEnv;
                        throw;
                    }

                    // Clear any references that might be holding onto values
                    returnValue = nullptr;

                    // Environment is automatically cleaned up when iterationEnv goes out of scope
                    env = originalEnv;
                } // iterationEnv is destroyed here, allowing immediate cleanup
            }
        }
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

            // Create a new environment for this iteration
            {
                shared_ptr<Environment> iterationEnv = make_shared<Environment>(originalEnv);
                env = iterationEnv;

                env->redeclare(node->getKeyDecl()->getName(), make_shared<StringValue>(pair.first, Range{}), node->getKeyDecl()->isConst());
                env->redeclare(node->getValueDecl()->getName(), pair.second, node->getValueDecl()->isConst());

                try
                {
                    if (node->getBody())
                    {
                        // Visit the body of the for-each loop
                        node->getBody()->visit(this);
                    }
                }
                catch (const BreakException &)
                {
                    env = originalEnv;
                    break;
                }
                catch (const ContinueException &)
                {
                    env = originalEnv;
                    continue;
                }
                catch (const ReturnException &e)
                {
                    env = originalEnv;
                    throw;
                }
                catch (const ErrorException &e)
                {
                    env = originalEnv;
                    throw;
                }

                // Clear any references that might be holding onto values
                returnValue = nullptr;

                // Clean up temporary environments created during this iteration
                cleanupTempEnvironments();

                // Explicitly clear the iteration environment to trigger immediate cleanup
                iterationEnv->clear();

                env = originalEnv;
            } // iterationEnv is destroyed here, allowing immediate cleanup
        }
    }
    else
    {
        error("for-each loop iterable must be an array or an object", node->getIterable()->getRange());
    }

    env = originalEnv;
    returnValue = nullptr;
}

void Interpreter::visit(ForStatementNode *node)
{
    shared_ptr<Environment> originalEnv = env;

    // Create environment for the entire for loop (for init variable)
    shared_ptr<Environment> forEnv = make_shared<Environment>(originalEnv);
    env = forEnv;

    try
    {
        // Initialize the loop variable
        if (node->getInit())
        {
            node->getInit()->visit(this);
        }

        while (true)
        {
            // Evaluate condition in the for environment
            if (node->getCondition())
            {
                node->getCondition()->visit(this);
                if (!returnValue || (returnValue->getType() != Value::Type::boolean &&
                    returnValue->getType() != Value::Type::number))
                {
                    error("for loop condition must be a boolean expression", node->getCondition()->getRange());
                    break;
                }

                bool condition = returnValue->toBoolean();
                if (!condition)
                    break;
            }

            // Create a new environment for this iteration's body
            {
                shared_ptr<Environment> iterationEnv = make_shared<Environment>(forEnv);
                env = iterationEnv;

                // don't visit an empty body
                if (node->getBody())
                {
                    try
                    {
                        node->getBody()->visit(this);
                    }
                    catch (const BreakException &)
                    {
                        env = originalEnv;
                        return;
                    }
                    catch (const ContinueException &)
                    {
                        // Restore for environment and execute increment
                        env = forEnv;
                        if (node->getIncrement())
                        {
                            node->getIncrement()->visit(this);
                        }
                        continue;
                    }
                    catch (const ReturnException &e)
                    {
                        env = originalEnv;
                        throw;
                    }
                    catch (const ErrorException &e)
                    {
                        env = originalEnv;
                        throw;
                    }
                }

                // Clear any references that might be holding onto values
                returnValue = nullptr;

                // Clean up temporary environments created during this iteration
                cleanupTempEnvironments();

                // Explicitly clear the iteration environment to trigger immediate cleanup
                iterationEnv->clear();

                // Restore for environment - iterationEnv goes out of scope here
                env = forEnv;
            } // iterationEnv is destroyed here, allowing immediate cleanup
            if (node->getIncrement())
            {
                node->getIncrement()->visit(this);
            }
        }
    }
    catch (const BreakException &)
    {
        // handle break statement
    }
    catch (const ContinueException &)
    {
        // handle continue statement
    }
    catch (const ReturnException &e)
    {
        env = originalEnv;
        throw;
    }
    catch (const ErrorException &e)
    {
        env = originalEnv;
        throw;
    }

    // Reset the environment after the loop
    env = originalEnv;
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
        if (hadError)
            break; // Stop processing elements if an error occurred
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

    returnValue = nullptr;
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
            error("array index out of bounds: " + to_string(index) + " for array of length: " + to_string(arrayValue->getElementCount()), node->getIndex()->getRange());
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

    if (returnValue->getType() != Value::Type::class_)
    {
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
            if (lhs->getType() == Value::Type::object)
            {
                auto objValue = dynamic_pointer_cast<ObjectValue>(lhs);
                errorAtToken("class '" + objValue->getClassName() + "' has no member '" + node->getIdentifier().getValue() + "'", node->getIdentifier(), node->getRange());
            }
            else
            {
                errorAtToken(lhs->typeAsString() + " has no member '" + node->getIdentifier().getValue() + "'", node->getIdentifier(), node->getRange());
            }
            return;
        }
    }
    else if (returnValue->getType() == Value::Type::class_)
    {
        auto classValue = dynamic_pointer_cast<ClassValue>(lhs);
        if (!classValue)
        {
            returnValue = nullptr;
            error("left-hand side of member access could not be cast to ClassValue", node->getExpression()->getRange());
            return;
        }

        shared_ptr<BlockNode> classBody = dynamic_pointer_cast<BlockNode>(classValue->getBody());
        if (!classBody)
        {
            error("class '" + classValue->getName() + "' has no body", node->getRange());
            returnValue = nullptr;
            return;
        }

        env = make_shared<Environment>(env); // Inherit from current environment to access imports

        try
        {
            // we don't want to push extra scope for class body, so we just visit the statements directly
            if (classBody->getStatements())
            {
                classBody->getStatements()->visit(this);
            }
        }
        catch (const ErrorException &e)
        {
            env = env->getParent();
            throw e;
        }

        // lookup the member in the class environment
        returnValue = env->lookupLocal(node->getIdentifier().getValue());
        env = env->getParent();
        if (!returnValue)
        {
            errorAtToken("class '" + classValue->getName() + "' has no member '" + node->getIdentifier().getValue() + "'", node->getIdentifier(), node->getRange());
        }
    }
}

void Interpreter::visit(ContinueNode *node)
{
    UNUSED(node);
    throw ContinueException(node->getRange());
}

void Interpreter::visit(DeleteNode *node)
{
    const string &name = node->getName();

    // Check if the variable exists in this scope or any parent scope
    if (!env->resolve(name))
    {
        errorAtToken("cannot delete variable '" + name + "' it is not defined", node->getIdentifier(), node->getRange());
    }

    // Try to remove the variable
    shared_ptr<Value> removedValue = env->remove(name);
    if (!removedValue)
    {
        errorAtToken("cannot delete '" + name + "'", node->getIdentifier(), node->getRange());
    }

    returnValue = nullptr;
}

void Interpreter::visit(ClassNode *node)
{
    // Create a new class value and declare it in the environment
    auto classValue = make_shared<ClassValue>(node->getName(), node->getBody());
    env->redeclare(node->getName(), classValue, node->isConst());

    returnValue = nullptr; // No return value for class declaration
}