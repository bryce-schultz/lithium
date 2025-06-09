#include "Exceptions.h"

BaseException::BaseException(Range range):
    range(range)
{ }

ReturnException::ReturnException(shared_ptr<Value> val, Range range):
    BaseException(range),
    value(std::move(val))
{ }

BreakException::BreakException(Range range):
    BaseException(range)
{ }

ExitException::ExitException(int code, Range range):
    BaseException(range),
    exitCode(code)
{ }
