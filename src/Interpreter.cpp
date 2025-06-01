#include "Interpreter.h"
#include "Nodes.h"
#include "Value.h"
#include "Error.h"
#include "Utils.h"

#define MAX_PEEK 25

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__)

#define errorAt(msg, location, range) \
    locationRangeError(msg, location, range, __FILE__, __LINE__)

#define errorAtToken(msg, token, range) \
    tokenRangeError(msg, token, range, __FILE__, __LINE__)

#define variableDefined(node) errorAtToken("variable '" + Utils::truncate(node->getName(), MAX_PEEK) + "' is already defined", node->getToken(), node->getRange())

#define variableNotDefined(node) errorAtToken("variable '" + Utils::truncate(node->getName(), MAX_PEEK) + "' is not defined", node->getToken(), node->getRange())

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
        returnValue = nullptr;
    }
}

void Interpreter::visit(NumberNode *node)
{
    returnValue = std::make_shared<NumberValue>(node->getValue());
}

void Interpreter::visit(StringNode *node)
{
    returnValue = std::make_shared<StringValue>(node->getValue());
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

std::shared_ptr<Value> Interpreter::evalBinaryExpression(ExpressionNode *left, OpNode *opNode, ExpressionNode *right)
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
            std::dynamic_pointer_cast<NumberValue>(leftValue),
            opNode,
            std::dynamic_pointer_cast<NumberValue>(rightValue));
    }
    else if (leftValue->getType() == Value::Type::string && rightValue->getType() == Value::Type::string)
    {
        return evalStringBinaryExpression(leftValue, opNode, rightValue);
    }
    else if (leftValue->getType() == Value::Type::boolean && rightValue->getType() == Value::Type::boolean)
    {
        return evalBooleanBinaryExpression(leftValue, opNode, rightValue);
    }
    // Add more type checks as needed
    return nullptr; // Unsupported operation
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

std::shared_ptr<Value> Interpreter::evalUnaryExpression(ExpressionNode *expression, OpNode *opNode, bool prefix)
{
    if (expression->isVariable())
    {
        auto varExpr = dynamic_cast<VarExprNode*>(expression);
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
        return evalNumericUnaryExpression(std::dynamic_pointer_cast<NumberValue>(value), opNode, prefix);
    }
    // Add more type checks for other unary operations as needed
    return nullptr; // Unsupported operation
}

std::shared_ptr<Value> Interpreter::evalNumericUnaryExpression(std::shared_ptr<NumberValue> value, OpNode *opNode, bool prefix)
{
    if (opNode->getType() == '-' && prefix)
    {
        return std::make_shared<NumberValue>(-value->getValue());
    }
    // Add more unary operations as needed
    return nullptr; // Unsupported operation
}

std::shared_ptr<Value> Interpreter::evalVariableUnaryExpression(VarExprNode *expression, OpNode *opNode, bool prefix)
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
        auto numberValue = std::dynamic_pointer_cast<NumberValue>(value);
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
        auto numberValue = std::dynamic_pointer_cast<NumberValue>(value);
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
    shared_ptr<Value>callee = returnValue;
    if (!callee)
    {
        returnValue = nullptr;
        return;
    }

    if (callee->getType() != Value::Type::function)
    {
        returnValue = nullptr; // or throw an error
        return;
    }

    FunctionValue *function = dynamic_cast<FunctionValue*>(callee.get());
    if (!function)
    {
        returnValue = nullptr; // or throw an error
        return;
    }

    if (args.size() != (size_t)function->getParameters()->getParamCount())
    {
        returnValue = nullptr; // or throw an error
        return;
    }

    Environment *scope = new Environment(function->getEnvironment());

    for (size_t i = 0; i < args.size(); ++i)
    {
        scope->declare(function->getParameters()->getParam(i)->getName(), args[i]);
    }

    env = scope; // Set the current environment to the function's environment
    // Execute the function body
    function->getBody()->visit(this);
    env = scope->getParent(); // Restore the previous environment after executing the function
    if (!returnValue)
    {
        returnValue = nullptr;
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

    returnValue = env->assign(asignee->getName(), value);

    if (!returnValue)
    {
        variableNotDefined(asignee);
    }
}

void Interpreter::visit(BlockNode *node)
{
    Environment *blockEnv = new Environment(env);
    env = blockEnv;
    node->visitAllChildren(this);
    env = blockEnv->getParent();
}

void Interpreter::visit(IfStatementNode *node)
{
    node->getCondition()->visit(this);
    if (!returnValue)
    {
        error("condition evaluation failed",
            node->getCondition()->getRange());
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
        node->getThenBranch()->visitAllChildren(this);
    }
    else if (node->getElseBranch())
    {
        node->getElseBranch()->visitAllChildren(this);
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

    auto function = std::make_shared<FunctionValue>(node->getName(), node->getParams(), node->getBody(), env);
    node->dropBody();
    node->dropParams();
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

        node->visitAllChildren(this);
    }

    returnValue = nullptr; // reset return value after the loop
}