#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <limits>
#include <stdexcept>
#include <ios>

#include "Builtins.h"
#include "Environment.h"
#include "Values.h"
#include "Utils.h"
#include "Error.h"

#ifdef _WIN32
    // Windows-specific includes would go here
#else
    // Unix-like system includes for socket structures
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
#endif

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::dynamic_pointer_cast;
using std::to_string;
using std::getline;
using std::stod;
using std::invalid_argument;
using std::numeric_limits;
using std::streamsize;

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__)

shared_ptr<Value> Builtins::print(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);
    UNUSED(range);

    if (args.empty())
    {
        return nullptr; // return nullptr to prevent printing "null" in interactive mode
    }

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (!args[i])
        {
            continue;
        }

        if (args[i]->getMember("string") && args[i]->getMember("string")->getType() == Value::Type::function)
        {
            // visit the string method if it exists
            auto stringMethod = dynamic_pointer_cast<FunctionValue>(args[i]->getMember("string"));
            if (!stringMethod)
            {
                error("Expected a function for 'string' method", args[i]->getRange());
                return nullptr;
            }

            auto oldEnv = interpreter.getEnvironment();
            interpreter.setEnvironment(stringMethod->getEnvironment());
            try
            {
                stringMethod->getBody()->visit(&interpreter);
            }
            catch (const ReturnException &e)
            {
                if (e.value)
                {
                    cout << e.value->toString();
                }
            }
            catch (const ErrorException &e)
            {
                // Handle any errors that occur during the method call
                interpreter.setEnvironment(oldEnv); // restore the environment
                error(e.what(), e.getRange());
                return nullptr;
            }

            interpreter.setEnvironment(oldEnv); // restore the environment after visiting the method
        }
        else
        {
            // Fallback to toString if no print method exists
            cout << args[i]->toString();
        }

        if (i != args.size() - 1)
        {
            cout << " ";
        }
    }
    cout.flush();
    return nullptr; // return nullptr to prevent printing "null" in interactive mode
}

shared_ptr<Value> Builtins::println(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);
    UNUSED(range);

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (!args[i])
        {
            continue;
        }

        if (args[i]->getMember("string") && args[i]->getMember("string")->getType() == Value::Type::function)
        {
            // visit the string method if it exists
            auto stringMethod = dynamic_pointer_cast<FunctionValue>(args[i]->getMember("string"));
            if (!stringMethod)
            {
                error("Expected a function for 'string' method", args[i]->getRange());
                return nullptr;
            }

            auto oldEnv = interpreter.getEnvironment();
            interpreter.setEnvironment(stringMethod->getEnvironment());
            try
            {
                stringMethod->getBody()->visit(&interpreter);
            }
            catch (const ReturnException &e)
            {
                if (e.value)
                {
                    cout << e.value->toString();
                }
            }
            catch (const ErrorException &e)
            {
                // Handle any errors that occur during the method call
                interpreter.setEnvironment(oldEnv); // restore the environment
                error(e.what(), e.getRange());
                return nullptr;
            }

            interpreter.setEnvironment(oldEnv); // restore the environment after visiting the method
        }
        else
        {
            cout << args[i]->toString();
        }

        if (i != args.size() - 1)
        {
            cout << " ";
        }
    }
    cout << endl;
    return nullptr;
}

shared_ptr<Value> Builtins::printf(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.empty())
    {
        error("printf() expects at least 1 argument, but got 0", range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::string)
    {
        error("printf() expects a string format as the first argument, but got " + args[0]->typeAsString(), args[0]->getRange());
        return nullptr;
    }

    const auto &format = dynamic_pointer_cast<StringValue>(args[0])->getValue();

    vector<string> formatArgs;
    for (size_t i = 1; i < args.size(); ++i)
    {
        if (args[i])
        {
            formatArgs.emplace_back(args[i]->toString());
        }
    }
    string result = Utils::formatString(format, formatArgs);

    cout << result;
    cout.flush();
    return nullptr;
}

shared_ptr<Value> Builtins::type(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 1)
    {
        error("type() expects exactly 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    const auto &arg = args[0];
    if (!arg)
    {
        return make_shared<StringValue>("undefined", range);
    }

    return make_shared<StringValue>(arg->typeAsString(), arg->getRange());
}

shared_ptr<Value> Builtins::exit(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() > 1)
    {
        error("exit() expects at most 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    int exitCode = 0;
    if (args.size() == 1)
    {
        if (args[0]->getType() != Value::Type::number)
        {
            error("exit() expects a number argument, but got " + args[0]->typeAsString(), args[0]->getRange());
            return nullptr;
        }
        exitCode = dynamic_pointer_cast<NumberValue>(args[0])->getValue();
    }

    throw ExitException(exitCode, range);
}

shared_ptr<Value> Builtins::input(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() > 1)
    {
        error("input() expects at most 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    string prompt;
    if (args.size() == 1 && args[0])
    {
        if (args[0]->getType() != Value::Type::string)
        {
            error("input() expects a string argument, but got " + args[0]->typeAsString(), args[0]->getRange());
            return nullptr;
        }
        prompt = dynamic_pointer_cast<StringValue>(args[0])->getValue();
    }

    cout << prompt;
    cout.flush();

    string userInput;
    getline(cin, userInput);

    if (cin.eof())
    {
        return make_shared<NullValue>(range);
    }

    return make_shared<StringValue>(userInput, range);
}

shared_ptr<Value> Builtins::len(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 1)
    {
        error("len() expects exactly 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    const auto &arg = args[0];
    if (!arg)
    {
        return make_shared<NullValue>(range);
    }

    switch (arg->getType())
    {
        case Value::Type::string:
            return make_shared<NumberValue>(dynamic_pointer_cast<StringValue>(arg)->length(), range);
        case Value::Type::array:
            return make_shared<NumberValue>(dynamic_pointer_cast<ArrayValue>(arg)->getElementCount(), range);
        case Value::Type::null:
            return make_shared<NumberValue>(0, range);
        default:
            error("len() expects a string or an array, but got " + arg->typeAsString(), arg->getRange());
            return nullptr;
    }
}

// attempts to convert the first argument to a number if it fails it returns NullValue, but not nullptr
shared_ptr<Value> Builtins::toNumber(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 1)
    {
        error("expected exactly 1 argument, but got " + to_string(args.size()), range);
        return make_shared<NullValue>(range);
    }

    const auto &arg = args[0];
    if (!arg)
    {
        return make_shared<NullValue>(range);
    }

    if (arg->getType() == Value::Type::number)
    {
        return arg; // already a number
    }

    try
    {
        double value = stod(arg->toString());
        return make_shared<NumberValue>(value, range);
    }
    catch (const invalid_argument &)
    {
        return make_shared<NullValue>(range); // conversion failed
    }
}

shared_ptr<Value> Builtins::toString(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 1)
    {
        error("expected exactly 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    const auto &arg = args[0];
    if (!arg)
    {
        return make_shared<StringValue>("null", range);
    }

    if (arg->getMember("string") && arg->getMember("string")->getType() == Value::Type::function)
    {
        // visit the string method if it exists
        auto stringMethod = dynamic_pointer_cast<FunctionValue>(arg->getMember("string"));
        if (!stringMethod)
        {
            error("Expected a function for 'string' method", arg->getRange());
            return nullptr;
        }

        auto oldEnv = interpreter.getEnvironment();
        interpreter.setEnvironment(stringMethod->getEnvironment());
        try
        {
            stringMethod->getBody()->visit(&interpreter);
        }
        catch (const ReturnException &e)
        {
            if (e.value)
            {
                interpreter.setEnvironment(oldEnv); // restore the environment after visiting the method
                return make_shared<StringValue>(e.value->toString(), arg->getRange());
            }
        }
        catch (const ErrorException &e)
        {
            // Handle any errors that occur during the method call
            interpreter.setEnvironment(oldEnv); // restore the environment
            error(e.what(), e.getRange());
            return nullptr;
        }
    }

    return make_shared<StringValue>(arg->toString(), arg->getRange());
}

shared_ptr<Value> Builtins::toBoolean(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 1)
    {
        error("expected exactly 1 argument, but got " + to_string(args.size()), range);
        return make_shared<NullValue>(range);
    }

    const auto &arg = args[0];
    if (!arg)
    {
        return make_shared<BooleanValue>(false, range);
    }

    return make_shared<BooleanValue>(arg->toBoolean(), arg->getRange());
}

// this should function like random() in c
shared_ptr<Value> Builtins::randomNumber(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 0)
    {
        error("expected no arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    return make_shared<NumberValue>(static_cast<double>(random()), range);
}

// read (fd, size) - returns a string with a max size of size
shared_ptr<Value> Builtins::readFd(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 2)
    {
        error("expected exactly 2 arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::number || args[1]->getType() != Value::Type::number)
    {
        error("expected two number arguments, but got " + args[0]->typeAsString() + " and " + args[1]->typeAsString(), range);
        return nullptr;
    }
    auto fdValue = dynamic_pointer_cast<NumberValue>(args[0]);
    auto sizeValue = dynamic_pointer_cast<NumberValue>(args[1]);
    int fd = static_cast<int>(fdValue->getValue());
    int size = static_cast<int>(sizeValue->getValue());
    if (fd < 0 || size <= 0)
    {
        error("expected a valid file descriptor and a positive size", range);
        return nullptr;
    }

    string buffer(size, '\0'); // create a buffer of the specified size
    ssize_t bytesRead = Utils::readFromFd(fd, &buffer[0], size);
    if (bytesRead < 0)
    {
        error("failed to read from file descriptor " + to_string(fd), range);
        return nullptr;
    }
    buffer.resize(bytesRead); // resize the buffer to the actual number of bytes read
    return make_shared<StringValue>(buffer, range);
}

// write (fd, data) - writes data to the file descriptor fd
shared_ptr<Value> Builtins::writeFd(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 2)
    {
        error("expected exactly 2 arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::number || args[1]->getType() != Value::Type::string)
    {
        error("expected a number and a string argument, but got " + args[0]->typeAsString() + " and " + args[1]->typeAsString(), range);
        return nullptr;
    }
    auto fdValue = dynamic_pointer_cast<NumberValue>(args[0]);
    auto dataValue = dynamic_pointer_cast<StringValue>(args[1]);
    int fd = static_cast<int>(fdValue->getValue());
    const string &data = dataValue->getValue();
    if (fd < 0)
    {
        error("expected a valid file descriptor", range);
        return nullptr;
    }

    ssize_t bytesWritten = Utils::writeToFd(fd, data.c_str(), data.size());
    if (bytesWritten < 0)
    {
        error("failed to write to file descriptor " + to_string(fd), range);
        return nullptr;
    }
    return make_shared<NumberValue>(static_cast<double>(bytesWritten), range);
}

// bool close(fd) - closes the file descriptor fd
shared_ptr<Value> Builtins::closeFd(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 1)
    {
        error("expected exactly 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::number)
    {
        error("expected a number argument, but got " + args[0]->typeAsString(), range);
        return nullptr;
    }
    auto fdValue = dynamic_pointer_cast<NumberValue>(args[0]);
    int fd = static_cast<int>(fdValue->getValue());
    if (fd < 0)
    {
        error("expected a valid file descriptor", range);
        return nullptr;
    }

    if (Utils::closeFd(fd) < 0)
    {
        error("failed to close file descriptor " + to_string(fd), range);
        return nullptr;
    }
    return make_shared<BooleanValue>(true, range);
}

// number open(filename, mode) - opens a file and returns a file descriptor
shared_ptr<Value> Builtins::openFile(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 2)
    {
        error("expected exactly 2 arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::string || args[1]->getType() != Value::Type::string)
    {
        error("expected two string arguments, but got " + args[0]->typeAsString() + " and " + args[1]->typeAsString(), range);
        return nullptr;
    }
    auto filenameValue = dynamic_pointer_cast<StringValue>(args[0]);
    auto modeValue = dynamic_pointer_cast<StringValue>(args[1]);
    const string &filename = filenameValue->getValue();
    const string &mode = modeValue->getValue();

    int flags = Utils::parseOpenMode(mode);
    int fd = Utils::openFile(filename, flags, 0644);

    if (fd < 0)
    {
        return make_shared<NullValue>(range);
    }
    return make_shared<NumberValue>(static_cast<double>(fd), range);
}

// number socket(type, address, port) - opens a socket connection to the given address and port
shared_ptr<Value> Builtins::socket(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 3)
    {
        error("expected exactly 3 arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::string)
    {
        error("expected a string for the type, but got " + args[0]->typeAsString(), range);
        return nullptr;
    }

    if (args[1]->getType() != Value::Type::string)
    {
        error("expected a string for the address, but got " + args[1]->typeAsString(), range);
        return nullptr;
    }

    if (args[2]->getType() != Value::Type::number)
    {
        error("expected a number for the port, but got " + args[2]->typeAsString(), range);
        return nullptr;
    }

    auto typeValue = dynamic_pointer_cast<StringValue>(args[0]);
    auto addressValue = dynamic_pointer_cast<StringValue>(args[1]);
    auto portValue = dynamic_pointer_cast<NumberValue>(args[2]);
    const string &type = typeValue->getValue();
    const string &address = addressValue->getValue();
    int port = static_cast<int>(portValue->getValue());

    if (type != "tcp" && type != "udp")
    {
        error("socket type must be 'tcp' or 'udp', but got '" + type + "'", range);
        return nullptr;
    }

    if (port < 0 || port > 65535)
    {
        error("port number must be between 0 and 65535", range);
        return nullptr;
    }

    int sockfd = Utils::createSocket(AF_INET, (type == "tcp" ? SOCK_STREAM : SOCK_DGRAM), 0);
    if (sockfd < 0)
    {
        error("failed to create socket", range);
        return nullptr;
    }

    // set SO_REUSEADDR
    int optval = 1;
    if (Utils::setSocketOption(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        error("failed to set SO_REUSEADDR on socket", range);
        Utils::closeFd(sockfd);
        return nullptr;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = Utils::hostToNetworkShort(port);
    if (Utils::convertAddress(AF_INET, address.c_str(), &serverAddr.sin_addr) <= 0)
    {
        error("invalid address: '" + address + "'", range);
        Utils::closeFd(sockfd);
        return nullptr;
    }

    // bind the socket to the address and port
    if (Utils::bindSocket(sockfd, &serverAddr, sizeof(serverAddr)) < 0)
    {
        error("failed to bind socket to " + address + ":" + to_string(port), range);
        Utils::closeFd(sockfd);
        return nullptr;
    }

    /*// set the socket to non-blocking mode
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0)
    {
        error("failed to get socket flags", range);
        close(sockfd);
        return nullptr;
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        error("failed to set socket to non-blocking mode", range);
        close(sockfd);
        return nullptr;
    }*/

    return make_shared<NumberValue>(static_cast<double>(sockfd), range);
}

// bool listen(socket, backlog = SOMAXCONN)
shared_ptr<Value> Builtins::listenSocket(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() < 1 || args.size() > 2)
    {
        error("expected 1 or 2 arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::number)
    {
        error("expected a number for the socket, but got " + args[0]->typeAsString(), range);
        return nullptr;
    }

    auto socketValue = dynamic_pointer_cast<NumberValue>(args[0]);
    int sockfd = static_cast<int>(socketValue->getValue());
    if (sockfd < 0)
    {
        error("invalid socket file descriptor", range);
        return nullptr;
    }

    int backlog = (args.size() == 2 && args[1]->getType() == Value::Type::number)
                      ? static_cast<int>(dynamic_pointer_cast<NumberValue>(args[1])->getValue())
                      : SOMAXCONN;
    if (Utils::listenSocket(sockfd, backlog) < 0)
    {
        error("failed to listen on socket " + to_string(sockfd), range);
        return nullptr;
    }
    return make_shared<BooleanValue>(true, range);
}

// int accept(socket) - accepts a connection on the socket and returns a new socket file descriptor
shared_ptr<Value> Builtins::acceptSocket(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 1)
    {
        error("expected exactly 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::number)
    {
        error("expected a number for the socket, but got " + args[0]->typeAsString(), range);
        return nullptr;
    }

    auto socketValue = dynamic_pointer_cast<NumberValue>(args[0]);
    int sockfd = static_cast<int>(socketValue->getValue());
    if (sockfd < 0)
    {
        error("invalid socket file descriptor", range);
        return nullptr;
    }

    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int clientSockfd = Utils::acceptSocket(sockfd, &clientAddr, &addrLen);
    if (clientSockfd < 0)
    {
        error("failed to accept connection on socket " + to_string(sockfd), range);
        return nullptr;
    }

    // create a new NumberValue for the client socket
    return make_shared<NumberValue>(static_cast<double>(clientSockfd), range);
}

// int connectSocket(socket, address, port) - connects to a server socket
shared_ptr<Value> Builtins::connectSocket(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 3)
    {
        error("expected exactly 3 arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::number)
    {
        error("expected a number for the socket, but got " + args[0]->typeAsString(), range);
        return nullptr;
    }

    auto socketValue = dynamic_pointer_cast<NumberValue>(args[0]);
    int sockfd = static_cast<int>(socketValue->getValue());
    if (sockfd < 0)
    {
        error("invalid socket file descriptor", range);
        return nullptr;
    }

    if (args[1]->getType() != Value::Type::string)
    {
        error("expected a string for the address, but got " + args[1]->typeAsString(), range);
        return nullptr;
    }

    if (args[2]->getType() != Value::Type::number)
    {
        error("expected a number for the port, but got " + args[2]->typeAsString(), range);
        return nullptr;
    }

    const auto &address = dynamic_pointer_cast<StringValue>(args[1])->getValue();
    int port = static_cast<int>(dynamic_pointer_cast<NumberValue>(args[2])->getValue());

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = Utils::hostToNetworkShort(port);
    if (Utils::convertAddress(AF_INET, address.c_str(), &serverAddr.sin_addr) <= 0)
    {
        error("invalid address: " + address, range);
        return nullptr;
    }

    if (Utils::connectSocket(sockfd, &serverAddr, sizeof(serverAddr)) < 0)
    {
        error("failed to connect to " + address + ":" + to_string(port), range);
        Utils::closeFd(sockfd);
        return nullptr;
    }

    return make_shared<NumberValue>(static_cast<double>(sockfd), range);
}

// sendSocket(socket, data) - sends data to the socket
shared_ptr<Value> Builtins::sendSocket(Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range & range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 2)
    {
        error("expected exactly 2 arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::number)
    {
        error("expected a number for the socket, but got " + args[0]->typeAsString(), range);
        return nullptr;
    }

    if (args[1]->getType() != Value::Type::string)
    {
        error("expected a string for the data, but got " + args[1]->typeAsString(), range);
        return nullptr;
    }

    auto socketValue = dynamic_pointer_cast<NumberValue>(args[0]);
    int sockfd = static_cast<int>(socketValue->getValue());
    if (sockfd < 0)
    {
        error("invalid socket file descriptor", range);
        return nullptr;
    }

    const auto &data = dynamic_pointer_cast<StringValue>(args[1])->getValue();
    ssize_t bytesSent = Utils::sendSocket(sockfd, data.c_str(), data.size(), 0);
    if (bytesSent < 0)
    {
        error("failed to send data on socket " + to_string(sockfd), range);
        return nullptr;
    }

    return make_shared<NumberValue>(static_cast<double>(bytesSent), range);
}

// receiveSocket(socket, size) - receives data from the socket and returns it as a string
shared_ptr<Value> Builtins::receiveSocket(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 2)
    {
        error("expected exactly 2 arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::number)
    {
        error("expected a number for the socket, but got " + args[0]->typeAsString(), range);
        return nullptr;
    }

    if (args[1]->getType() != Value::Type::number)
    {
        error("expected a number for the size, but got " + args[1]->typeAsString(), range);
        return nullptr;
    }

    auto socketValue = dynamic_pointer_cast<NumberValue>(args[0]);
    int sockfd = static_cast<int>(socketValue->getValue());
    if (sockfd < 0)
    {
        error("invalid socket file descriptor", range);
        return nullptr;
    }

    int size = static_cast<int>(dynamic_pointer_cast<NumberValue>(args[1])->getValue());
    char *buffer = new char[size];
    ssize_t bytesReceived = Utils::receiveSocket(sockfd, buffer, size, 0);
    if (bytesReceived < 0)
    {
        error("failed to receive data on socket " + to_string(sockfd), range);
        delete[] buffer;
        return nullptr;
    }

    auto result = make_shared<StringValue>(string(buffer, bytesReceived), range);
    delete[] buffer; // clean up the buffer

    return result;
}

shared_ptr<Value> Builtins::runShellCommand(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 1)
    {
        error("expected exactly 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (!args[0])
    {
        error("expected a non-null argument", range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::string)
    {
        error("expected a string argument, but got " + args[0]->typeAsString(), range);
        return nullptr;
    }

    const auto &command = dynamic_pointer_cast<StringValue>(args[0])->getValue();

    // use execvp to run the command and pipe to get the string output
    int pipefd[2];
    if (Utils::createPipe(pipefd) < 0)
    {
        error("failed to create pipe for command execution", range);
        return nullptr;
    }
    pid_t pid = Utils::forkProcess();
    if (pid < 0)
    {
        error("failed to fork process for command execution", range);
        Utils::closeFd(pipefd[0]);
        Utils::closeFd(pipefd[1]);
        return nullptr;
    }
    if (pid == 0) // child process
    {
        Utils::closeFd(pipefd[0]); // close read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to the pipe
        dup2(pipefd[1], STDERR_FILENO); // redirect stderr to the pipe
        Utils::closeFd(pipefd[1]); // close write end of the pipe after duplicating

        // split command into argv format
        vector<char*> argv;
        string commandCopy = command;
        size_t pos = 0;
        while ((pos = commandCopy.find(' ')) != string::npos)
        {
            string token = commandCopy.substr(0, pos);
            argv.push_back(const_cast<char*>(token.c_str()));
            commandCopy.erase(0, pos + 1);
        }
        argv.push_back(const_cast<char*>(commandCopy.c_str())); // last token
        argv.push_back(nullptr); // null-terminate the argv array
        if (argv.empty() || argv[0] == nullptr)
        {
            error("command cannot be empty", range);
            ::exit(EXIT_FAILURE);
        }

        Utils::executeProgram(argv[0], argv.data());
        perror("execvp failed"); // execvp only returns on error
        ::exit(EXIT_FAILURE);
    }

    Utils::closeFd(pipefd[1]); // close write end of the pipe in the parent process
    string output;
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = Utils::readFromFd(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = '\0'; // null-terminate the buffer
        output += buffer; // append to output string
    }
    if (bytesRead < 0)
    {
        return make_shared<NullValue>(range);
    }
    Utils::closeFd(pipefd[0]); // close read end of the pipe
    int status;
    if (Utils::waitForProcess(pid, &status, 0) < 0)
    {
        return make_shared<NullValue>(range);
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
        return make_shared<NullValue>(range);
    }
    return make_shared<StringValue>(output, range);
}

shared_ptr<Value> Builtins::dumpenv(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(range);

    // if we have 1 argument and its an object or a class, we will dump its environment
    if (args.size() == 1)
    {
        if (args[0]->getType() == Value::Type::object)
        {
            auto object = dynamic_pointer_cast<ObjectValue>(args[0]);
            if (object)
            {
                env = object->getEnvironment();
            }
        }
        else if (args[0]->getType() != Value::Type::null)
        {
            error("dumpenv() expects an object or nothing, but got " + args[0]->typeAsString(), args[0]->getRange());
            return nullptr;
        }
    }

    // this function is for debugging purposes, it dumps the environment variables
    // to the standard output, so we don't need to return anything
    if (env)
    {
        env->dump();
    }
    else
    {
        cout << "No environment available." << endl;
    }
    return nullptr;
}

shared_ptr<Value> Builtins::sleep(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 1)
    {
        error("sleep() expects exactly 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::number)
    {
        error("sleep() expects a number argument, but got " + args[0]->typeAsString(), args[0]->getRange());
        return nullptr;
    }

    double seconds = dynamic_pointer_cast<NumberValue>(args[0])->getValue();
    if (seconds < 0)
    {
        error("sleep() expects a non-negative number, but got " + to_string(seconds), args[0]->getRange());
        return nullptr;
    }

    Utils::sleepSeconds(seconds);
    return nullptr;
}

shared_ptr<Value> Builtins::time(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 0)
    {
        error("time() expects no arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    // get the current time in seconds since the epoch
    time_t now = Utils::getCurrentTime();
    if (now == -1)
    {
        error("failed to get current time", range);
        return nullptr;
    }

    return make_shared<NumberValue>(static_cast<double>(now), range);
}

// string rgb(r, g, b) - creates a color value from RGB components
shared_ptr<Value> Builtins::rgb(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 3)
    {
        error("rgb() expects exactly 3 arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    for (const auto &arg : args)
    {
        if (arg->getType() != Value::Type::number)
        {
            error("rgb() expects all arguments to be numbers", arg->getRange());
            return nullptr;
        }
    }

    double r = dynamic_pointer_cast<NumberValue>(args[0])->getValue();
    double g = dynamic_pointer_cast<NumberValue>(args[1])->getValue();
    double b = dynamic_pointer_cast<NumberValue>(args[2])->getValue();

    // create ansi rgb string and return it
    return make_shared<StringValue>("\033[38;2;" + to_string(static_cast<int>(r)) + ";" + to_string(static_cast<int>(g)) + ";" + to_string(static_cast<int>(b)) + "m", range);
}

// [string] listdir(path) - lists the contents of the directory at the given path
shared_ptr<Value> Builtins::listdir(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);
    if (args.size() != 1)
    {
        error("listdir() expects exactly 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::string)
    {
        error("listdir() expects a string argument, but got " + args[0]->typeAsString(), args[0]->getRange());
        return nullptr;
    }

    string path = dynamic_pointer_cast<StringValue>(args[0])->getValue();
    vector<string> contents = Utils::listDirectory(path);
    if (contents.empty())
    {
        return make_shared<ArrayValue>(vector<shared_ptr<Value>>{}, range);
    }

    vector<shared_ptr<Value>> items;
    for (const auto &item : contents)
    {
        items.push_back(make_shared<StringValue>(item, range));
    }
    return make_shared<ArrayValue>(items, range);
}

shared_ptr<Value> Builtins::getcwd(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);
    UNUSED(args);

    if (args.size() != 0)
    {
        error("getcwd() expects no arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    string cwd = Utils::getCurrentDirectory();
    if (cwd.empty())
    {
        error("failed to get current working directory", range);
        return nullptr;
    }

    return make_shared<StringValue>(cwd, range);
}

shared_ptr<Value> Builtins::chdir(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);

    if (args.size() != 1)
    {
        error("chdir() expects exactly 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::string)
    {
        error("chdir() expects a string argument, but got " + args[0]->typeAsString(), args[0]->getRange());
        return nullptr;
    }

    string path = dynamic_pointer_cast<StringValue>(args[0])->getValue();
    if (!Utils::changeDirectory(path))
    {
        return make_shared<BooleanValue>(false, range);
    }
    return make_shared<BooleanValue>(true, range);
}

shared_ptr<Value> Builtins::getpid(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(args);
    UNUSED(env);
    return make_shared<NumberValue>(static_cast<double>(Utils::getpid()), range);
}

// string getuser() - returns the username of the current user
shared_ptr<Value> Builtins::getuser(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);
    UNUSED(args);

    string user = Utils::getCurrentUser();
    if (user.empty())
    {
        error("failed to get current user", range);
        return nullptr;
    }

    return make_shared<StringValue>(user, range);
}

// string getenv(variable) - returns the value of the environment variable
shared_ptr<Value> Builtins::getenv(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(interpreter);
    UNUSED(env);
    if (args.size() != 1)
    {
        error("getenv() expects exactly 1 argument, but got " + to_string(args.size()), range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::string)
    {
        error("getenv() expects a string argument, but got " + args[0]->typeAsString(), args[0]->getRange());
        return nullptr;
    }

    string var = dynamic_pointer_cast<StringValue>(args[0])->getValue();
    const char *value = ::getenv(var.c_str());
    if (value)
    {
        return make_shared<StringValue>(value, range);
    }
    return make_shared<NullValue>(range);
}

shared_ptr<Value> Builtins::dumpstack(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);
    UNUSED(range);

    string format = "table"; // default format
    
    if (args.size() == 1)
    {
        if (args[0]->getType() != Value::Type::string)
        {
            error("dumpstack() format argument must be a string", args[0]->getRange());
            return nullptr;
        }
        format = dynamic_pointer_cast<StringValue>(args[0])->getValue();
        
        if (format != "table" && format != "raw")
        {
            error("dumpstack() format must be 'table' or 'raw'", args[0]->getRange());
            return nullptr;
        }
    }
    else if (args.size() > 1)
    {
        error("dumpstack() expects 0 or 1 arguments, but got " + to_string(args.size()), range);
        return nullptr;
    }

    cout << interpreter.getCallStack().toString(format);

    return nullptr; // this function is for debugging purposes, it dumps the stack trace to the standard output
}
