#pragma once

#include <memory>
#include <vector>

#include "Values.h"
#include "Environment.h"
#include "Range.h"
#include "Exceptions.h"

using std::shared_ptr;
using std::vector;

namespace Builtins
{
    shared_ptr<Value> print(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> println(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> printf(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> type(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> exit(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> input(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> len(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
}