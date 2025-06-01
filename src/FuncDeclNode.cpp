#include "FuncDeclNode.h"

FuncDeclNode::FuncDeclNode(const Token &token, ParamListNode *params, StatementNode *body):
    token(token)
{
    setRange(token.getRange());

    addChild(params);
    if (params)
    {
        setRangeStart(params->getRange().getStart());
    }

    addChild(body);
    if (body)
    {
        setRangeEnd(body->getRange().getEnd());
    }
}

const Token &FuncDeclNode::getToken() const
{
    return token;
}

string FuncDeclNode::getName() const
{
    return token.getValue();
}

ParamListNode *FuncDeclNode::getParams() const
{
    if (getChildCount() > 0)
    {
        return dynamic_cast<ParamListNode*>(getChild(0));
    }
    return nullptr;
}

StatementNode *FuncDeclNode::getBody() const
{
    if (getChildCount() > 1)
    {
        return dynamic_cast<StatementNode*>(getChild(1));
    }
    return nullptr;
}

void FuncDeclNode::dropBody()
{
    if (getChildCount() > 1)
    {
        removeChild(1); // Remove the body node
    }
}

void FuncDeclNode::dropParams()
{
    if (getChildCount() > 0)
    {
        removeChild(0); // Remove the parameters node
    }
}

bool FuncDeclNode::isConst() const
{
    // Function declarations are not const by default
    return false;
}

void FuncDeclNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}