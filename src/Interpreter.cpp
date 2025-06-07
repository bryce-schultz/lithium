#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>

#include "Interpreter.h"
#include "Nodes.h"
#include "Value.h"
#include "Error.h"
#include "Utils.h"

using std::shared_ptr;
using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::dynamic_pointer_cast;
using std::make_shared;
using std::fmod;

#define MAX_PEEK 25

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__)

#define errorAt(msg, location, range) \
    locationRangeError(msg, location, range, __FILE__, __LINE__)

#define errorAtToken(msg, token, range) \
    tokenRangeError(msg, token, range, __FILE__, __LINE__)

#define variableDefined(node) errorAtToken("variable '" + Utils::truncate(node->getName(), MAX_PEEK) + "' is already defined", node->getToken(), node->getRange())

#define variableNotDefined(node) errorAtToken("variable '" + Utils::truncate(node->getName(), MAX_PEEK) + "' is not defined", node->getToken(), node->getRange())

// Exception to handle function returns
struct ReturnException : public exception
{
    shared_ptr<Value> value;
    Range range;
    ReturnException(shared_ptr<Value> val, Range range = {}): value(val), range(range) {}
};

// Exception to handle exit calls
struct ExitException : public exception
{
    int exitCode;
    ExitException(int code = 0): exitCode(code) {}
};

Interpreter::Interpreter(shared_ptr<Environment> env)
{
    if (!env)
    {
        this->env = make_shared<Environment>(); // Create a new environment if none is provided
    }
    else
    {
        this->env = env; // Use the provided environment
    }
}

void Interpreter::visitAllChildren(Node *node)
{
    node->visit(this);
}

void Interpreter::visit(StatementsNode *node)
{
    for (auto &statement : node->getStatements())
    {
        if (statement)
        {
            returnValue = nullptr;
            statement->visit(this);
        }
    }
}

void Interpreter::visit(PrintStatementNode *node)
{
    node->getExpr()->visit(this);
    auto result = returnValue;
    if (!result)
    {
        return;
    }
    cout << result->toString() << "\n";
    returnValue = nullptr;
}

void Interpreter::visit(NumberNode *node)
{
    returnValue = make_shared<NumberValue>(node->getValue());
}

void Interpreter::visit(StringNode *node)
{
    returnValue = make_shared<StringValue>(node->getValue());
}

void Interpreter::visit(BinaryExprNode *node)
{
    if (node->isUnary())
    {
        if (node->isPrefix())
        {
            returnValue = evalUnaryExpression(node->getRight(), node->getOperator(), true);
        }
        else
        {
            returnValue = evalUnaryExpression(node->getLeft(), node->getOperator(), false);
        }
    }
    else
    {
        returnValue = evalBinaryExpression(node->getLeft(), node->getOperator(), node->getRight());
    }
}

shared_ptr<Value> Interpreter::evalBinaryExpression(shared_ptr<ExpressionNode> left, shared_ptr<OpNode> opNode, shared_ptr<ExpressionNode> right)
{
    left->visit(this);
    auto leftValue = returnValue;
    if (!leftValue)
    {
        error("left operand of binary expression is null", left->getRange());
        return nullptr; // or throw an error
    }

    right->visit(this);
    auto rightValue = returnValue;
    if (!rightValue)
    {
        error("right operand of binary expression is null", right->getRange());
        return nullptr; // or throw an error
    }

    if (leftValue->getType() == Value::Type::number && rightValue->getType() == Value::Type::number)
    {
        return evalNumericBinaryExpression(
            dynamic_pointer_cast<NumberValue>(leftValue),
            opNode,
            dynamic_pointer_cast<NumberValue>(rightValue));
    }
    else if (leftValue->getType() == Value::Type::string && rightValue->getType() == Value::Type::string)
    {
        return evalStringBinaryExpression(
            dynamic_pointer_cast<StringValue>(leftValue),
            opNode,
            dynamic_pointer_cast<StringValue>(rightValue));
    }
    else if (leftValue->getType() == Value::Type::boolean && rightValue->getType() == Value::Type::boolean)
    {
        return evalBooleanBinaryExpression(
            dynamic_pointer_cast<BooleanValue>(leftValue),
            opNode,
            dynamic_pointer_cast<BooleanValue>(rightValue));
    }
    // Add more type checks as needed
    return nullptr; // Unsupported operation
}

shared_ptr<Value> Interpreter::evalNumericBinaryExpression(shared_ptr<NumberValue> left, shared_ptr<OpNode> opNode, shared_ptr<NumberValue> right)
{
    if (opNode->getType() == '+')
    {
        auto result = left->getValue() + right->getValue();
        return make_shared<NumberValue>(result);
    }
    else if (opNode->getType() == '-')
    {
        auto result = left->getValue() - right->getValue();
        return make_shared<NumberValue>(result);
    }
    else if (opNode->getType() == '*')
    {
        auto result = left->getValue() * right->getValue();
        return make_shared<NumberValue>(result);
    }
    else if (opNode->getType() == '/')
    {
        if (right->getValue() == 0)
        {
            // Handle division by zero error
            return nullptr;
        }
        auto result = left->getValue() / right->getValue();
        return make_shared<NumberValue>(result);
    }
    else if (opNode->getType() == '%')
    {
        if (right->getValue() == 0)
        {
            // Handle modulo by zero error
            return nullptr;
        }
        return make_shared<NumberValue>(fmod(left->getValue(), right->getValue()));
    }
    else if (opNode->getType() == Token::EQ)
    {
        return make_shared<BooleanValue>(left->getValue() == right->getValue());
    }
    else if (opNode->getType() == Token::NE)
    {
        return make_shared<BooleanValue>(left->getValue() != right->getValue());
    }
    else if (opNode->getType() == Token::LE)
    {
        return make_shared<BooleanValue>(left->getValue() <= right->getValue());
    }
    else if (opNode->getType() == Token::GE)
    {
        return make_shared<BooleanValue>(left->getValue() >= right->getValue());
    }
    else if (opNode->getType() == '<')
    {
        return make_shared<BooleanValue>(left->getValue() < right->getValue());
    }
    else if (opNode->getType() == '>')
    {
        return make_shared<BooleanValue>(left->getValue() > right->getValue());
    }

    error("unsupported numeric operation: " + opNode->getToken().getValue(), opNode->getRange());
    return nullptr;
}

shared_ptr<Value> Interpreter::evalStringBinaryExpression(shared_ptr<StringValue> left, shared_ptr<OpNode> opNode, shared_ptr<StringValue> right)
{
    if (opNode->getType() == '+')
    {
        // Concatenate strings
        return make_shared<StringValue>(left->getValue() + right->getValue());
    }
    // Add more string operations as needed
    return nullptr; // Unsupported operation
}

shared_ptr<Value> Interpreter::evalBooleanBinaryExpression(shared_ptr<BooleanValue> left, shared_ptr<OpNode> opNode, shared_ptr<BooleanValue> right)
{
    if (opNode->getType() == Token::AND)
    {
        return make_shared<BooleanValue>(
            dynamic_pointer_cast<BooleanValue>(left)->getValue() &&
            dynamic_pointer_cast<BooleanValue>(right)->getValue());
    }
    else if (opNode->getType() == Token::OR)
    {
        return make_shared<BooleanValue>(
            dynamic_pointer_cast<BooleanValue>(left)->getValue() ||
            dynamic_pointer_cast<BooleanValue>(right)->getValue());
    }
    else if (opNode->getType() == Token::EQ)
    {
        return make_shared<BooleanValue>(
            dynamic_pointer_cast<BooleanValue>(left)->getValue() ==
            dynamic_pointer_cast<BooleanValue>(right)->getValue());
    }
    else if (opNode->getType() == Token::NE)
    {
        return make_shared<BooleanValue>(
            dynamic_pointer_cast<BooleanValue>(left)->getValue() !=
            dynamic_pointer_cast<BooleanValue>(right)->getValue());
    }
    else if (opNode->getType() == Token::LE)
    {
        // Assuming boolean values are treated as 0 (false) and 1 (true)
        return make_shared<BooleanValue>(
            dynamic_pointer_cast<BooleanValue>(left)->getValue() <=
            dynamic_pointer_cast<BooleanValue>(right)->getValue());
    }
    else if (opNode->getType() == Token::GE)
    {
        return make_shared<BooleanValue>(
            dynamic_pointer_cast<BooleanValue>(left)->getValue() >=
            dynamic_pointer_cast<BooleanValue>(right)->getValue());
    }
    else if (opNode->getType() == '<')
    {
        return make_shared<BooleanValue>(
            dynamic_pointer_cast<BooleanValue>(left)->getValue() <
            dynamic_pointer_cast<BooleanValue>(right)->getValue());
    }
    else if (opNode->getType() == '>')
    {
        return make_shared<BooleanValue>(
            dynamic_pointer_cast<BooleanValue>(left)->getValue() >
            dynamic_pointer_cast<BooleanValue>(right)->getValue());
    }
    // Add more boolean operations as needed
    return nullptr; // Unsupported operation
}

shared_ptr<Value> Interpreter::evalUnaryExpression(shared_ptr<ExpressionNode> expression, shared_ptr<OpNode> opNode, bool prefix)
{
    if (!expression) {
        error("null expression in unary operation", Range());
        return nullptr;
    }
    if (expression->isLval())
    {
        shared_ptr<VarExprNode> varExpr = dynamic_pointer_cast<VarExprNode>(expression);
        if (!varExpr)
        {
            error("expected variable expression", expression->getRange());
            return nullptr; // or throw an error
        }
        return evalVariableUnaryExpression(varExpr, opNode, prefix);
    }
    expression->visit(this);
    auto value = returnValue;
    if (!value)
    {
        error("unary expression evaluation failed", expression->getRange());
        return nullptr; // or throw an error
    }
    if (value->getType() == Value::Type::number)
    {
        return evalNumericUnaryExpression(dynamic_pointer_cast<NumberValue>(value), opNode, prefix);
    }
    // Add more type checks for other unary operations as needed
    return nullptr; // Unsupported operation
}

shared_ptr<Value> Interpreter::evalNumericUnaryExpression(shared_ptr<NumberValue> value, shared_ptr<OpNode> opNode, bool prefix)
{
    if (opNode->getType() == '-' && prefix)
    {
        return make_shared<NumberValue>(-value->getValue());
    }
    // Add more unary operations as needed
    return nullptr; // Unsupported operation
}

shared_ptr<Value> Interpreter::evalVariableUnaryExpression(shared_ptr<VarExprNode> expression, shared_ptr<OpNode> opNode, bool prefix)
{
    if (opNode->getType() == Token::INC)
    {
        auto value = env->lookup(expression->getName());
        if (!value)
        {
            error("variable " + expression->getName() + " is not defined", expression->getRange());
            return nullptr; // or throw an error
        }

        if (value->getType() != Value::Type::number)
        {
            error("variable " + expression->getName() + " is not a number", expression->getRange());
            return nullptr; // or throw an error
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
    }
    else if (opNode->getType() == Token::DEC)
    {
        auto value = env->lookup(expression->getName());
        if (!value || value->getType() != Value::Type::number)
        {
            error("variable " + expression->getName() + " is not a number", expression->getRange());
            return nullptr; // or throw an error
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
    }
    // Add more unary operations as needed
    return nullptr; // Unsupported operation
}

void Interpreter::visit(CallNode *node)
{
    vector<shared_ptr<Value>> args;
    if (node->getArgs())
    {
        for (auto &arg : node->getArgs()->getArgs())
        {
            arg->visit(this);
            if (!returnValue) {
                error("function argument evaluated to null", arg->getRange());
                return;
            }
            args.push_back(returnValue);
        }
    }

    auto calleeNode = node->getCallee();
    calleeNode->visit(this);
    if (!returnValue) {
        error("function callee evaluated to null", calleeNode->getRange());
        return;
    }
    shared_ptr<Value> callee = returnValue;
    if (callee->getType() != Value::Type::function)
    {
        error("callee is not a function", calleeNode->getRange());
        returnValue = nullptr;
        return;
    }

    shared_ptr<FunctionValue> function = dynamic_pointer_cast<FunctionValue>(callee);
    if (!function)
    {
        error("callee could not be cast to FunctionValue", calleeNode->getRange());
        returnValue = nullptr;
        return;
    }

    if (args.size() != (size_t)function->getParameters()->getParamCount())
    {
        error("argument count mismatch in function call", node->getRange());
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
    try
    {
        function->getBody()->visit(this);
        returnValue = make_shared<NullValue>();
    }
    catch (const ReturnException& e)
    {
        returnValue = e.value;
    }
    env = previousEnv;
}

void Interpreter::visit(ReturnStatementNode *node)
{
    auto expr = node->getExpression();
    if (expr)
    {
        expr->visit(this);
        if (!returnValue) {
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
        returnValue = nullptr; // reset return value if expression evaluation failed
        return;
    }

    returnValue = env->declare(node->getName(), returnValue, node->isConst());
    if (!returnValue)
    {
        variableDefined(node);
    }

    returnValue = nullptr;
}

void Interpreter::visit(VarExprNode *node)
{
    returnValue = env->lookup(node->getName());
    if (!returnValue)
    {
        variableNotDefined(node);
        returnValue = nullptr;
    }
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
    if (!asignee)
    {
        error("invalid assignment target", node->getRange());
        returnValue = nullptr;
        return;
    }
    returnValue = env->assign(asignee->getName(), value);
    if (!returnValue) {
        error("assignment failed (possibly assigning to constant or undefined variable)", node->getRange());
    }
}

void Interpreter::visit(BlockNode *node)
{
    shared_ptr<Environment> blockEnv = make_shared<Environment>(env);
    auto previousEnv = env;
    env = blockEnv;
    node->getStatements()->visit(this);
    env = previousEnv;
}

void Interpreter::visit(IfStatementNode *node)
{
    node->getCondition()->visit(this);
    if (!returnValue)
    {
        returnValue = nullptr;
        return;
    }

    if (returnValue->getType() != Value::Type::boolean &&
        returnValue->getType() != Value::Type::number)
    {
        error("condition must be a boolean expression",
            node->getCondition()->getRange());
        returnValue = nullptr;
        return;
    }

    bool condition = returnValue->toBoolean();

    if (condition)
    {
        node->getThenBranch()->visit(this);
    }
    else if (node->getElseBranch())
    {
        node->getElseBranch()->visit(this);
    }

    returnValue = nullptr; // reset return value after visiting branches
}

void Interpreter::visit(FuncDeclNode *node)
{
    if (env->lookup(node->getName()))
    {
        variableDefined(node);
        return;
    }
    auto function = make_shared<FunctionValue>(node->getName(), node->getParams(), node->getBody(), env);
    returnValue = env->declare(node->getName(), function, node->isConst());
}

void Interpreter::visit(WhileNode *node)
{
    while (true)
    {
        node->getCondition()->visit(this);
        if (!returnValue)
        {
            error("condition evaluation failed", node->getCondition()->getRange());
            returnValue = nullptr;
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

        node->getBody()->visit(this);
    }

    returnValue = nullptr; // reset return value after the loop
}

void Interpreter::visit(ForStatementNode *node)
{
    // Initialize the loop variable
    if (node->getInit())
    {
        node->getInit()->visit(this);
    }

    while (true)
    {
        // Check the loop condition
        if (node->getCondition())
        {
            node->getCondition()->visit(this);
            if (!returnValue)
            {
                error("for loop condition evaluation failed", node->getCondition()->getRange());
                returnValue = nullptr;
                break;
            }

            if (returnValue->getType() != Value::Type::boolean &&
                returnValue->getType() != Value::Type::number)
            {
                error("for loop condition must be a boolean expression", node->getCondition()->getRange());
                returnValue = nullptr;
                break;
            }

            bool condition = returnValue->toBoolean();
            if (!condition) break;
        }

        // Execute the loop body
        node->getBody()->visit(this);

        if (node->getIncrement())
        {
            node->getIncrement()->visit(this);
        }
    }

    returnValue = nullptr; // reset return value after the loop
}