#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>

using std::string;
using std::map;
using std::set;

class Value; // forward declaration

class Environment : public std::enable_shared_from_this<Environment>
{
public:
    Environment(std::shared_ptr<Environment> parent = nullptr);

    std::shared_ptr<Value> declare(const string &name, std::shared_ptr<Value> value, bool constant = false);
    std::shared_ptr<Value> assign(const string &name, std::shared_ptr<Value> value);
    std::shared_ptr<Value> lookup(const string &name) const;
    std::shared_ptr<Environment> resolve(const string &name) const;
    std::shared_ptr<Environment> getParent() const;

    void dump() const; // For debugging purposes, prints all variables and constants
private:
    bool hasVariable(const string &name) const;
    bool hasConstant(const string &name) const;
private:
    std::weak_ptr<Environment> parent; // changed from shared_ptr to weak_ptr
    map<string, std::shared_ptr<Value>> variables; // runtime variables
    set<string> constants; // constants
};