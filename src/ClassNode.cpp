#include "ClassNode.h"
#include "Visitor.h"
#include "StatementNode.h"
#include "DeclNode.h"

ClassNode::ClassNode(const Token &token, shared_ptr<StatementNode> body)
{
    setRange(token.getRange());
    if (body)
    {
        setRangeEnd(body->getRange().getEnd());
    }
}

const Token &ClassNode::getToken() const
{
    return token;
}

const string &ClassNode::getName() const
{
    return token.getValue();
}

shared_ptr<StatementNode> ClassNode::getBody() const
{
    return body;
}

void ClassNode::setBody(shared_ptr<StatementNode> body)
{
    this->body = body;
    if (body)
    {
        setRangeEnd(body->getRange().getEnd());
    }
}

bool ClassNode::isConst() const
{
    return false;
}

void ClassNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}