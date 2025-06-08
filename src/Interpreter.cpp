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

// exception to handle function returns
struct ReturnException : public exception
{
    shared_ptr<Value> value;
    Range range;
    ReturnException(shared_ptr<Value> val, Range range = {}): value(val), range(range) {}
};

// exception to handle exit calls
struct ExitException : public exception
{
    int exitCode;
    ExitException(int code = 0): exitCode(code) {}
};

Interpreter::Interpreter(bool isInteractive, shared_ptr<Environment> env):
    isInteractive(isInteractive),
    returnValue(nullptr)
{
    if (!env)
    {
        this->env = make_shared<Environment>(); // Create a new environment if none is provided
    }
    else
    {
        this->env = env; // Use the provided environment
    }

    this->env->declare("LINE", make_shared<NumberValue>(1), true);

    // runtime values defined in all environments
    this->env->declare("null", make_shared<NullValue>(), true);
    this->env->declare("true", make_shared<BooleanValue>(true), true);
    this->env->declare("false", make_shared<BooleanValue>(false), true);
    this->env->declare("PI", make_shared<NumberValue>(M_PI), true);
    this->env->declare("E", make_shared<NumberValue>(M_E), true);
    this->env->declare("VERSION", make_shared<StringValue>(INTERPRETER_VERSION), true);
    this->env->declare("printall", make_shared<BuiltinFunctionValue>([](const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env) -> shared_ptr<Value>
    {
        UNUSED(env);
        if (args.empty())
        {
            cout << "\n";
            return nullptr; // indicate no return value, this will prevent printing "null" in interactive mode
        }

        for (const auto &arg : args)
        {
            if (arg)
            {
                cout << arg->toString() << " ";
            }
        }
        cout << "\n";
        return nullptr;
    }), true);
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
            env->redeclare("LINE", make_shared<NumberValue>(statement->getRange().getStart().getLine()), true); // Declare a constant variable for the current line number
            statement->visit(this);

            if (returnValue && isInteractive)
            {
                // if in interactive mode, print the return value
                cout << returnValue->toString() << endl;
            }
        }
    }
}

void Interpreter::visit(PrintStatementNode *node)
{
    if (!node->getExpr())
    {
        cout << "\n"; // Print a newline if no expression is provided
        returnValue = nullptr;
        return;
    }

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
    if (!node)
    {
        error("StringNode is null", Range());
        return;
    }

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
        return nullptr;
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

    if (returnValue)
    {
        return returnValue;
    }

    error("unsupported binary operation between " + leftValue->typeAsString() + " and " + rightValue->typeAsString(), opNode->getRange());
    return nullptr;
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
            return nullptr;
        }
        return evalVariableUnaryExpression(varExpr, opNode, prefix);
    }
    expression->visit(this);
    auto value = returnValue;
    if (!value)
    {
        error("unary expression evaluation failed", expression->getRange());
        return nullptr;
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
            return nullptr;
        }

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
    }
    else if (opNode->getType() == Token::DEC)
    {
        auto value = env->lookup(expression->getName());
        if (!value || value->getType() != Value::Type::number)
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
            if (!returnValue)
            {
                error("function argument evaluated to null", arg->getRange());
                return;
            }
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
                std::to_string(function->getParameters()->getParamCount()) +
                " arguments, but got " + std::to_string(args.size()), node->getRange());
            returnValue = nullptr;
            return;
        }
        else if (!function->getParameters() && !args.empty())
        {
            error("function '" + function->getName() + "' does not take any arguments, but got " + std::to_string(args.size()), node->getRange());
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
            returnValue = nullptr;
        }
        catch (const ReturnException& e)
        {
            returnValue = e.value;
        }
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

        try
        {
            returnValue = builtin->call(args, env);
        }
        catch (const ExitException& e)
        {
            throw e; // rethrow exit exception
        }
    }
    else
    {
        error("cannot call non-function value: " + callee->typeAsString(), calleeNode->getRange());
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
    if (!returnValue)
    {
        error("assignment failed (possibly assigning to constant or undefined variable)", node->getRange());
    }
}

void Interpreter::visit(BlockNode *node)
{
    if (!node->getStatements())
    {
        returnValue = nullptr; // Empty block, nothing to do
        return;
    }

    env = make_shared<Environment>(env);
    node->getStatements()->visit(this);
    env = env->getParent();

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
        error("condition must be a boolean expression", node->getCondition()->getRange());
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
    env->declare(node->getName(), function, node->isConst());
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

        node->getBody()->visit(this);
    }

    returnValue = nullptr; // reset return value after the loop
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

        node->getBody()->visit(this);

        if (node->getIncrement())
        {
            node->getIncrement()->visit(this);
        }
    }

    // Reset the environment after the loop
    env = forEnv->getParent();
    returnValue = nullptr; // reset return value after the loop
}

void Interpreter::visit(NullNode *node)
{
    UNUSED(node);
    returnValue = make_shared<NullValue>();
}