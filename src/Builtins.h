#pragma once

#include <memory>
#include <vector>

#include "Values.h"
#include "Environment.h"
#include "Range.h"
#include "Exceptions.h"
#include "Interpreter.h"

using std::shared_ptr;
using std::vector;

namespace Builtins
{
    shared_ptr<Value> print(Interpreter& interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> println(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> printf(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> type(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> exit(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> input(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> len(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> toNumber(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> toString(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> toBoolean(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> randomNumber(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> readFd(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> writeFd(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> closeFd(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> openFile(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> socket(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> listenSocket(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> acceptSocket(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> connectSocket(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> sendSocket(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> receiveSocket(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> getSocketAddress(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> getSocketPort(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> runShellCommand(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> dumpEnv(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> sleep(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> time(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> rgb(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> listdir(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> getcwd(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> chdir(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> getpid(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> getuser(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
    shared_ptr<Value> getenv(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {});
}