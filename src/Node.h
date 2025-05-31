//**************************************************
// File: Node.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the Node base class for AST
// nodes, supporting children and range management.
//**************************************************

#pragma once

#include <vector>

#include "Range.h"
#include "Visitor.h"

using std::vector;

class Node
{
public:
    Node() = default;
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&&) = default;
    Node& operator=(Node&&) = default;
    virtual ~Node();

    void addChild(Node *child);
    const vector<Node*> &getChildren() const;
    Node *getChild(int index) const;
    void setChild(int index, Node *child);
    void removeChild(int index);
    void clearChildren();
    int getChildCount() const;
    Range getRange() const;
    void setRange(const Range &range);
    void setRange(const Location &start, const Location &end);
    void setRangeStart(const Location &start);
    void setRangeEnd(const Location &end);

    virtual void visit(Visitor *visitor) = 0; // Pure virtual function for visiting nodes
    void visitAllChildren(Visitor *visitor);
private:
    Range range;
    vector<Node*> children;
};