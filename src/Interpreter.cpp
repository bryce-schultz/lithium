#include "Interpreter.h"
#include "Nodes.h"
#include "Value.h"
#include "Error.h"

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__); \

#define errorAt(msg, token, range) \
    tokenRangeError(msg, token, range, __FILE__, __LINE__); \

#define variableDefined(node) errorAt("variable " + node->getName() + " is not defined", node->getToken(), node->getRange())

Interpreter::Interpreter(Environment *env)
{
    if (!env)
    {
        this->env = new Environment(); // Create a new environment if none is provided
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

void Interpreter::visit(StatementNode *node)
{
    node->visitAllChildren(this);
}

void Interpreter::visit(StatementsNode *node)
{
    for (int i = 0; i < node->getChildCount(); ++i)
    {
        Node *child = node->getChild(i);
        if (child)
        {
            returnValue = nullptr;
            child->visit(this);
            if (!returnValue) continue;

            // if a statement returns a value we will assume it is to be printed
            if (returnValue->getType() != Value::Type::null)
            {
                cout << returnValue->toString() << "\n";
            }
            // No need to delete returnValue, shared_ptr handles it
        }
    }
}

void Interpreter::visit(PrintStatementNode *node)
{
    node->getExpr()->visit(this);
    auto result = returnValue;
    if (result)
    {
        cout << result->toString() << "\n";
    }
    else
    {
        cout << "null\n";
    }
    // No need to delete result, shared_ptr handles it
    returnValue = nullptr; // reset return value
}

void Interpreter::visit(NumberNode *node)
{
    returnValue = std::make_shared<NumberValue>(node->getValue());
}

void Interpreter::visit(StringNode *node)
{
    returnValue = std::make_shared<StringValue>(node->getValue());
}

void Interpreter::visit(BinaryExpressionNode *node)
{
    node->getLeft()->visit(this);
    auto leftValue = std::dynamic_pointer_cast<Value>(returnValue);

    node->getRight()->visit(this);
    auto rightValue = std::dynamic_pointer_cast<Value>(returnValue);

    OpNode *opNode = node->getOperator();

    if (!node->isUnary())
    {
        if (leftValue->getType() == Value::Type::number &&
            rightValue->getType() == Value::Type::number)
        {
            returnValue = evalNumericBinaryExpression(
                std::dynamic_pointer_cast<NumberValue>(leftValue),
                opNode,
                std::dynamic_pointer_cast<NumberValue>(rightValue));
        }
        else if (leftValue->getType() == Value::Type::string &&
                 rightValue->getType() == Value::Type::string)
        {
            returnValue = evalStringBinaryExpression(leftValue, opNode, rightValue);
        }
        else if (leftValue->getType() == Value::Type::boolean &&
                 rightValue->getType() == Value::Type::boolean)
        {
            returnValue = evalBooleanBinaryExpression(leftValue, opNode, rightValue);
        }
        else
        {
            // Handle unsupported types or throw an error
            returnValue = nullptr; // or throw an error
        }
    }
    else
    {
        if (leftValue->getType() == Value::Type::number)
        {
            returnValue = evalNumericUnaryExpression(
                std::dynamic_pointer_cast<NumberValue>(leftValue),
                opNode,
                node->isPrefix());
        }
        else
        {
            // Handle unsupported unary operation or throw an error
            returnValue = nullptr; // or throw an error
        }
    }
}

std::shared_ptr<Value> Interpreter::evalNumericBinaryExpression(std::shared_ptr<NumberValue> left, OpNode *opNode, std::shared_ptr<NumberValue> right)
{
    if (opNode->getType() == '+')
    {
        auto result = left->getValue() + right->getValue();
        return std::make_shared<NumberValue>(result);
    }
    else if (opNode->getType() == '-')
    {
        auto result = left->getValue() - right->getValue();
        return std::make_shared<NumberValue>(result);
    }
    else if (opNode->getType() == '*')
    {
        auto result = left->getValue() * right->getValue();
        return std::make_shared<NumberValue>(result);
    }
    else if (opNode->getType() == '/')
    {
        if (right->getValue() == 0)
        {
            // Handle division by zero error
            return nullptr;
        }
        auto result = left->getValue() / right->getValue();
        return std::make_shared<NumberValue>(result);
    }
    // Add more operations as needed
    return nullptr; // Unsupported operation
}

std::shared_ptr<Value> Interpreter::evalStringBinaryExpression(std::shared_ptr<Value> left, OpNode *opNode, std::shared_ptr<Value> right)
{
    if (opNode->getType() == '+')
    {
        // Concatenate strings
        return std::make_shared<StringValue>(
            std::dynamic_pointer_cast<StringValue>(left)->getValue() +
            std::dynamic_pointer_cast<StringValue>(right)->getValue());
    }
    // Add more string operations as needed
    return nullptr; // Unsupported operation
}

std::shared_ptr<Value> Interpreter::evalBooleanBinaryExpression(std::shared_ptr<Value> left, OpNode *opNode, std::shared_ptr<Value> right)
{
    if (opNode->getType() == Token::AND)
    {
        return std::make_shared<BooleanValue>(
            std::dynamic_pointer_cast<BooleanValue>(left)->getValue() &&
            std::dynamic_pointer_cast<BooleanValue>(right)->getValue());
    }
    else if (opNode->getType() == Token::OR)
    {
        return std::make_shared<BooleanValue>(
            std::dynamic_pointer_cast<BooleanValue>(left)->getValue() ||
            std::dynamic_pointer_cast<BooleanValue>(right)->getValue());
    }
    // Add more boolean operations as needed
    return nullptr; // Unsupported operation
}

std::shared_ptr<Value> Interpreter::evalNumericUnaryExpression(std::shared_ptr<NumberValue> value, OpNode *opNode, bool prefix)
{
    if (opNode->getType() == '-')
    {
        return std::make_shared<NumberValue>(-value->getValue());
    }
    // Add more unary operations as needed
    return nullptr; // Unsupported operation
}

void Interpreter::visit(CallNode *node)
{
    vector<Value*> args;
    if (node->getArgs())
    {
        for (Node *arg : node->getArgs()->getChildren())
        {
            arg->visit(this);
            if (returnValue)
            {
                args.push_back(returnValue);
            }
        }
    }

    ExpressionNode *calleeNode = node->getCallee();
    calleeNode->visit(this);
    Value *callee = returnValue;
    if (!callee)
    {
        for (Value *arg : args)
        {
            delete arg; // Clean up arguments
        }
        returnValue = nullptr; // or throw an error
        return;
    }

    if (callee->getType() != Value::Type::function)
    {
        delete callee;
        for (Value *arg : args)
        {
            delete arg;
        }
        returnValue = nullptr; // or throw an error
        return;
    }

    FunctionValue *function = dynamic_cast<FunctionValue*>(callee);
    if (!function)
    {
        delete callee;
        for (Value *arg : args)
        {
            delete arg;
        }
        returnValue = nullptr; // or throw an error
        return;
    }

    if (args.size() != function->getParameters().size())
    {
        delete callee;
        for (Value *arg : args)
        {
            delete arg;
        }
        returnValue = nullptr; // or throw an error
        return;
    }

    Environment *scope = new Environment(function->getEnvironment());

    for (size_t i = 0; i < args.size(); ++i)
    {
        scope->declare(function->getParameters()[i], args[i]);
    }

    // Execute the function body
    function->getBody()->visitAllChildren(this);
    if (!returnValue)
    {
        returnValue = nullptr;
    }
}

void Interpreter::visit(VarDeclNode *node)
{
    if (!node->getExpr())
    {
        returnValue = nullptr;
    }
    else
    {
       node->getExpr()->visit(this);
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
}

void Interpreter::visit(AssignNode *node)
{
    node->getExpr()->visit(this);
    Value *value = returnValue;

    if (!node->getAsignee()->isVariable())
    {
        error("cannot assign to a non-variable expression", node->getRange());
        returnValue = nullptr;
        return;
    }

    VarExprNode *asignee = dynamic_cast<VarExprNode*>(node->getAsignee());
    if (!asignee)
    {
        error("invalid assignment target", node->getRange());
        returnValue = nullptr;
        return;
    }

    env->dump();

    returnValue = env->assign(asignee->getName(), value);

    env->dump();
    if (!returnValue)
    {
        variableDefined(asignee);
    }
}