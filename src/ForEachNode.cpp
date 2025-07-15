#include "ForEachNode.h"

#include "Nodes.h"

ForEachNode::ForEachNode(shared_ptr<VarDeclNode> keyDecl, shared_ptr<VarDeclNode> valueDecl, shared_ptr<ExpressionNode> iterable, shared_ptr<StatementNode> body):
    keyDecl(keyDecl),
    valueDecl(valueDecl),
    iterable(iterable),
    body(body)
{
    setRangeStart(keyDecl->getRange().getStart());
    if (valueDecl)
    {
        setRangeEnd(valueDecl->getRange().getEnd());
    }
    else
    {
        setRangeEnd(keyDecl->getRange().getEnd());
    }

    if (body)
    {
        setRangeEnd(body->getRange().getEnd());
    }
    else if (iterable)
    {
        setRangeEnd(iterable->getRange().getEnd());
    }
    else
    {
        setRangeEnd(keyDecl->getRange().getEnd());
    }
}

shared_ptr<VarDeclNode> ForEachNode::getKeyDecl() const
{
    return keyDecl;
}

shared_ptr<VarDeclNode> ForEachNode::getValueDecl() const
{
    return valueDecl;
}

shared_ptr<ExpressionNode> ForEachNode::getIterable() const
{
    return iterable;
}

shared_ptr<StatementNode> ForEachNode::getBody() const
{
    return body;
}

bool ForEachNode::isArrayLike() const
{
    return (keyDecl && !valueDecl);
}

bool ForEachNode::isMapLike() const
{
    return (keyDecl && valueDecl);
}

void ForEachNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}