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

    virtual string toString() const override;
};