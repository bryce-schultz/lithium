//**************************************************
// File: Node.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the Node base class for AST
// nodes, including child management and range info.
//**************************************************

#include "Node.h"

Node::Node():
    range()
{ }

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
