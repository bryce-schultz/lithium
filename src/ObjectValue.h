#pragma once

#include <memory>
#include <string>

#include "Value.h"

using std::shared_ptr;
using std::string;
using std::make_shared;

class ObjectValue : public Value
{
public:
    // takes the variables out of the environment and uses them as members
    ObjectValue(const string &name, shared_ptr<Environment> env);

    // intercept the getMember and setMember calls to use the environment
    virtual shared_ptr<Value> getMember(const string &name) const override
    {
        return env->lookup(name);
    }

    // intercept the setMember call to use the environment
    virtual Result<Value> setMember(const string &name, const shared_ptr<Value> &value) override
    {
        return env->assign(name, value);
    }

    // add a member to the environment, returns true if successful
    virtual bool addMember(const string &name, const shared_ptr<Value> &value, bool isConst) override
    {
        return env->declare(name, value, isConst) != nullptr;
    }

    virtual string toString() const override;
private:
    string name;
    shared_ptr<Environment> env;
};