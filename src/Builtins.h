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
    shared_ptr<Value> toNumber(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> toString(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> toBoolean(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> randomNumber(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> readFd(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> writeFd(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> closeFd(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> openFile(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> socket(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> listenSocket(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> acceptSocket(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> connectSocket(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> sendSocket(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> receiveSocket(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> getSocketAddress(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> getSocketPort(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> runShellCommand(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> dumpEnv(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> sleep(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> time(const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
}