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
    
    // Destructor to break reference cycles
    virtual ~ObjectValue();

    // Get the class name of this object instance
    inline const string& getClassName() const { return name; }

    // intercept the getMember and setMember calls to use the environment
    virtual shared_ptr<Value> getMember(const string &name) const override;

    // intercept the setMember call to use the environment
    virtual Result<Value> setMember(const string &name, const shared_ptr<Value> &value) override;

    // add a member to the environment, returns true if successful
    virtual bool addMember(const string &name, const shared_ptr<Value> &value, bool isConst) override;

    virtual const std::map<string, shared_ptr<Value>> &getMembers() const override;

    // Addition operations (for string concatenation)
    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const override;

    // Equality operations
    virtual shared_ptr<Value> eq(const shared_ptr<NullValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<NullValue> &other) const override;
    virtual shared_ptr<Value> eq(const shared_ptr<ObjectValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<ObjectValue> &other) const override;

    virtual string toString() const override;
    virtual string typeAsString() const override;
private:
    string name;
    shared_ptr<Environment> env;
};