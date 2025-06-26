#include "Exceptions.h"
#include "Utils.h"

using std::move;
using std::string;

BaseException::BaseException(Range range):
    range(range)
{ }

ReturnException::ReturnException(shared_ptr<Value> val, Range range):
    BaseException(range),
    value(move(val))
{ }

BreakException::BreakException(Range range):
    BaseException(range)
{ }

ExitException::ExitException(int code, Range range):
    BaseException(range),
    exitCode(code)
{ }

ContinueException::ContinueException(Range range):
    BaseException(range)
{ }

ErrorException::ErrorException(const string &message, Range range):
    BaseException(range)
{
    UNUSED(message);
}
