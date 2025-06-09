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
    setupEnvironment();
}

void Interpreter::setupEnvironment()
{
    setupBuiltInFunctions();
    setupRuntimeValues();
}

void Interpreter::setupBuiltInFunctions()
{
    env->declare("printall", make_shared<BuiltinFunctionValue>(Builtins::printall), true);
    env->declare("type", make_shared<BuiltinFunctionValue>(Builtins::type), true);
    env->declare("exit", make_shared<BuiltinFunctionValue>(Builtins::exit), true);
    env->declare("input", make_shared<BuiltinFunctionValue>(Builtins::input), true);
}

void Interpreter::setupRuntimeValues()
{
    // FILE is also declared, but its setup in the main file for now.
    env->declare("null", make_shared<NullValue>(), true);
    env->declare("true", make_shared<BooleanValue>(true), true);
    env->declare("false", make_shared<BooleanValue>(false), true);
    env->declare("PI", make_shared<NumberValue>(M_PI), true);
    env->declare("E", make_shared<NumberValue>(M_E), true);
    env->declare("VERSION", make_shared<StringValue>(INTERPRETER_VERSION), true);
}

void Interpreter::visitAllChildren(Node *node)
{
    try
    {
        node->visit(this);
    }
    catch (const ExitException & e)
    {
        exit(e.exitCode);
    }
}

void Interpreter::visit(StatementsNode *node)
{
    for (auto &statement : node->getStatements())
    {
        if (!statement)
        {
            continue;
        }

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

void Interpreter::visit(PrintStatementNode *node)
{
    if (!node->getExpr())
    {
        cout << "\n"; // print a newline if no expression is provided
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

    error("unsupported unary operation on " + value->typeAsString() + " and ", opNode->getRange());
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
    if (node->getArgs())
    {
        for (auto &arg : node->getArgs()->getArgs())
        {
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

        returnValue = builtin->call(args, env, node->getRange());
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
        error("invalid operators in assignment", node->getRange());
        returnValue = nullptr;
        return;
    }

    returnValue = env->assign(asignee->getName(), value);
}

void Interpreter::visit(BlockNode *node)
{
    if (!node->getStatements())
    {
        returnValue = nullptr;
        return;
    }

    env = make_shared<Environment>(env); // push a new environment for the block
    node->getStatements()->visit(this);
    env = env->getParent(); // pop the environment after visiting the block

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
        }
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