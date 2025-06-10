#pragma once

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;

template<typename T>
struct Result
{
    int status = 0;
    shared_ptr<T> value;
};
