#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <fcntl.h> // platform-specific header for file operations
#include <unistd.h> // platform-specific header for file operations
#include <sys/socket.h> // platform-specific header for socket operations
#include <netinet/in.h> // platform-specific header for socket operations
#include <arpa/inet.h> // platform-specific header for socket operations
#include <sys/wait.h>

#include "Builtins.h"
#include "Environment.h"
#include "Values.h"
#include "Utils.h"
#include "Error.h"

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::dynamic_pointer_cast;

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__)

// returns nullptr, this will prevent printing "null" in interactive mode
shared_ptr<Value> Builtins::print(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);
    UNUSED(range);

    if (args.empty())
    {
        return nullptr;
    }

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (args[i])
        {
            cout << args[i]->toString();
        }

        if (i != args.size() - 1)
        {
            cout << " ";
        }
    }
    cout.flush();
    return nullptr;
}

shared_ptr<Value> Builtins::println(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);
    UNUSED(range);

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (args[i])
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

shared_ptr<Value> Builtins::printf(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
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
            formatArgs.push_back(args[i]->toString());
        }
    }
    string result = Utils::formatString(format, formatArgs);

    cout << result;
    cout.flush();
    return nullptr;
}

shared_ptr<Value> Builtins::type(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 1)
    {
        error("type() expects exactly 1 argument, but got " + std::to_string(args.size()), range);
        return nullptr;
    }

    const auto &arg = args[0];
    if (!arg)
    {
        return make_shared<StringValue>("undefined", range);
    }

    return make_shared<StringValue>(arg->typeAsString(), arg->getRange());
}

shared_ptr<Value> Builtins::exit(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() > 1)
    {
        error("exit() expects at most 1 argument, but got " + std::to_string(args.size()), range);
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

shared_ptr<Value> Builtins::input(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() > 1)
    {
        error("input() expects at most 1 argument, but got " + std::to_string(args.size()), range);
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
    std::getline(cin, userInput);

    // check if getline was eof
    if (cin.eof())
    {
        // if so, return a NullValue
        return make_shared<NullValue>(range);
    }

    // if userInput is empty, the string will be empty. This allows the user to
    // distinguish between empty and eof.
    return make_shared<StringValue>(userInput, range);
}

shared_ptr<Value> Builtins::len(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 1)
    {
        error("len() expects exactly 1 argument, but got " + std::to_string(args.size()), range);
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
        default:
            error("len() expects a string or an array, but got " + arg->typeAsString(), arg->getRange());
            return nullptr;
    }
}

// attemtps to conver the first argument to a number if it fails it returns NullValue, but not nullptr
shared_ptr<Value> Builtins::toNumber(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 1)
    {
        error("expected exactly 1 argument, but got " + std::to_string(args.size()), range);
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
        double value = std::stod(arg->toString());
        return make_shared<NumberValue>(value, range);
    }
    catch (const std::invalid_argument &)
    {
        return make_shared<NullValue>(range); // conversion failed
    }
}

// this should function like random() in c
shared_ptr<Value> Builtins::randomNumber(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 0)
    {
        error("expected no arguments, but got " + std::to_string(args.size()), range);
        return nullptr;
    }

    return make_shared<NumberValue>(static_cast<double>(random()), range);
}

// read (fd, size) - returns a string with a max size of size
shared_ptr<Value> Builtins::readFd(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 2)
    {
        error("expected exactly 2 arguments, but got " + std::to_string(args.size()), range);
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
    ssize_t bytesRead = read(fd, &buffer[0], size);
    if (bytesRead < 0)
    {
        error("failed to read from file descriptor " + std::to_string(fd), range);
        return nullptr;
    }
    buffer.resize(bytesRead); // resize the buffer to the actual number of bytes read
    return make_shared<StringValue>(buffer, range);
}

// write (fd, data) - writes data to the file descriptor fd
shared_ptr<Value> Builtins::writeFd(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 2)
    {
        error("expected exactly 2 arguments, but got " + std::to_string(args.size()), range);
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

    ssize_t bytesWritten = write(fd, data.c_str(), data.size());
    if (bytesWritten < 0)
    {
        error("failed to write to file descriptor " + std::to_string(fd), range);
        return nullptr;
    }
    return make_shared<NumberValue>(static_cast<double>(bytesWritten), range);
}

// bool close(fd) - closes the file descriptor fd
shared_ptr<Value> Builtins::closeFd(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 1)
    {
        error("expected exactly 1 argument, but got " + std::to_string(args.size()), range);
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

    if (close(fd) < 0)
    {
        error("failed to close file descriptor " + std::to_string(fd), range);
        return nullptr;
    }
    return make_shared<BooleanValue>(true, range);
}

// number open(filename, mode) - opens a file and returns a file descriptor
shared_ptr<Value> Builtins::openFile(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 2)
    {
        error("expected exactly 2 arguments, but got " + std::to_string(args.size()), range);
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

    int fd = open(filename.c_str(), Utils::parseOpenMode(mode));
    if (fd < 0)
    {
        error("failed to open file '" + filename + "' with mode '" + mode + "'", range);
        return nullptr;
    }
    return make_shared<NumberValue>(static_cast<double>(fd), range);
}

// number open(type, address, port) - opens a socket connection to the given address and port
shared_ptr<Value> Builtins::openSocket(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 3)
    {
        error("expected exactly 3 arguments, but got " + std::to_string(args.size()), range);
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

    int sockfd = socket(AF_INET, (type == "tcp" ? SOCK_STREAM : SOCK_DGRAM), 0);
    if (sockfd < 0)
    {
        error("failed to create socket", range);
        return nullptr;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr) <= 0)
    {
        error("invalid address: '" + address + "'", range);
        close(sockfd);
        return nullptr;
    }

    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        error("failed to connect to " + address + ":" + std::to_string(port), range);
        close(sockfd);
        return nullptr;
    }

    return make_shared<NumberValue>(static_cast<double>(sockfd), range);
}

// bool listen(socket, backlog = SOMAXCONN)
shared_ptr<Value> Builtins::listenSocket(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() < 1 || args.size() > 2)
    {
        error("expected 1 or 2 arguments, but got " + std::to_string(args.size()), range);
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
    if (listen(sockfd, backlog) < 0)
    {
        error("failed to listen on socket " + std::to_string(sockfd), range);
        return nullptr;
    }
    return make_shared<BooleanValue>(true, range);
}

shared_ptr<Value> Builtins::runShellCommand(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 1)
    {
        error("expected exactly 1 argument, but got " + std::to_string(args.size()), range);
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
    if (pipe(pipefd) < 0)
    {
        error("failed to create pipe for command execution", range);
        return nullptr;
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        error("failed to fork process for command execution", range);
        close(pipefd[0]);
        close(pipefd[1]);
        return nullptr;
    }
    if (pid == 0) // child process
    {
        close(pipefd[0]); // close read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to the pipe
        dup2(pipefd[1], STDERR_FILENO); // redirect stderr to the pipe
        close(pipefd[1]); // close write end of the pipe after duplicating

        // split command into argv format
        char *const argv[] = {const_cast<char *>(command.c_str()), nullptr};

        execvp(argv[0], argv);
        perror("execvp failed"); // execvp only returns on error
        ::exit(EXIT_FAILURE);
    }

    close(pipefd[1]); // close write end of the pipe in the parent process
    string output;
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = '\0'; // null-terminate the buffer
        output += buffer; // append to output string
    }
    if (bytesRead < 0)
    {
        error("failed to read from pipe", range);
        close(pipefd[0]);
        return nullptr;
    }
    close(pipefd[0]); // close read end of the pipe
    int status;
    if (waitpid(pid, &status, 0) < 0)
    {
        error("failed to wait for child process", range);
        return nullptr;
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
        error("command execution failed with status " + std::to_string(WEXITSTATUS(status)), range);
        return nullptr;
    }
    return make_shared<StringValue>(output, range);
}
