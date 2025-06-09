#pragma once

#include <exception>
#include <memory>

#include "Range.h"
#include "Values.h"

using std::shared_ptr;
using std::exception;

struct BaseException : public exception
{
    Range range;
    BaseException(Range range = {});
};

// exception to handle function returns
struct ReturnException : public BaseException
{
    shared_ptr<Value> value;
    ReturnException(shared_ptr<Value> val, Range range = {});
};

struct BreakException : public BaseException
{
    BreakException(Range range = {});
};

// exception to handle exit calls
struct ExitException : public BaseException
{
    int exitCode;
    ExitException(int code = 0, Range = {});
};