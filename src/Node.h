//**************************************************
// File: Node.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the Node base class for AST
// nodes, supporting children and range management.
//**************************************************

#pragma once

#include <memory>
#include <vector>

#include "Range.h"
#include "Visitor.h"

using std::shared_ptr;
using std::vector;

class Node
{
public:
    using Ptr = shared_ptr<Node>;
public:
    Node();
    virtual ~Node() = default;

    Range getRange() const;
    void setRange(const Range &range);
    void setRange(const Location &start, const Location &end);
    void setRangeStart(const Location &start);
    void setRangeEnd(const Location &end);

    virtual void visit(Visitor *visitor) = 0;
private:
    Range range;
};

using NodePtr = shared_ptr<Node>;