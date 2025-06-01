//**************************************************
// File: Node.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the Node base class for AST
// nodes, including child management and range info.
//**************************************************

#include "Node.h"

Node::~Node()
{
    clearChildren(); // Ensure all children are deleted
}

void Node::addChild(Node *child)
{
    children.push_back(child);
}

const vector<Node *> &Node::getChildren() const
{
    return children;
}

Node *Node::getChild(int index) const
{
    if (index < 0 || index >= static_cast<int>(children.size()))
    {
        return nullptr;
    }

    return children[index];
}

void Node::setChild(int index, Node *child)
{
    if (index < 0 || index >= static_cast<int>(children.size()) || !child)
    {
        return;
    }

    children[index] = child;
}

void Node::removeChild(int index)
{
    if (index < 0 || index >= static_cast<int>(children.size()))
    {
        return;
    }

    children.erase(children.begin() + index);
}

void Node::clearChildren()
{
    for (Node *child : children)
    {
        delete child; // parents have ownership of children
    }
    children.clear();
}

int Node::getChildCount() const
{
    return static_cast<int>(children.size());
}

Range Node::getRange() const
{
    return range;
}

void Node::setRange(const Range &range)
{
    this->range = range;
}

void Node::setRange(const Location &start, const Location &end)
{
    range.setStart(start);
    range.setEnd(end);
}

void Node::setRangeStart(const Location &start)
{
    range.setStart(start);
}

void Node::setRangeEnd(const Location &end)
{
    range.setEnd(end);
}

void Node::visitAllChildren(Visitor *visitor)
{
    for (Node *child : children) 
    {
        if (child) 
        {
            child->visit(visitor);
        }
    }
}
