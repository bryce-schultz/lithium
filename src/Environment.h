#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>

#include "Result.h"

using std::string;
using std::map;
using std::set;
using std::shared_ptr;
using std::weak_ptr;
using std::enable_shared_from_this;

class Value; // forward declaration

class Environment : public enable_shared_from_this<Environment>
{
public:
    enum ResultStatus
    {
        SUCCESS = 0,
        VARIABLE_NOT_FOUND = 1,
        VARIABLE_ALREADY_DECLARED = 2,
        VARIABLE_IS_CONSTANT = 3,
    };
public:
    Environment(shared_ptr<Environment> parent = nullptr);

    shared_ptr<Value> declare(const string &name, shared_ptr<Value> value, bool constant = false);
    shared_ptr<Value> redeclare(const string &name, shared_ptr<Value> value, bool constant = false);
    Result<Value> assign(const string &name, shared_ptr<Value> value);
    shared_ptr<Value> lookup(const string &name) const;
    shared_ptr<Value> lookupLocal(const string &name) const;
    shared_ptr<Environment> resolve(const string &name) const;
    shared_ptr<Environment> getParent() const;

    void dump() const; // for debugging purposes, prints all variables and constants
private:
    bool hasVariable(const string &name) const;
    bool hasConstant(const string &name) const;
private:
    shared_ptr<Environment> parent; // changed from shared_ptr to weak_ptr
    map<string, shared_ptr<Value>> variables; // runtime variables
    set<string> constants; // constants
};