#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>

using std::string;
using std::map;
using std::set;

class Value; // forward declaration

class Environment
{
public:
    Environment(Environment *parent = nullptr);

    std::shared_ptr<Value> declare(const string &name, std::shared_ptr<Value> value, bool constant = false);
    std::shared_ptr<Value> assign(const string &name, std::shared_ptr<Value> value);
    std::shared_ptr<Value> lookup(const string &name) const;
    Environment *resolve(const string &name) const;
    Environment *getParent() const;

    void dump() const; // For debugging purposes, prints all variables and constants
private:
    bool hasVariable(const string &name) const;
    bool hasConstant(const string &name) const;
private:
    Environment *parent;
    map<string, std::shared_ptr<Value>> variables; // runtime variables
    set<string> constants; // constants
};