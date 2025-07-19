//**************************************************
// File: Utils.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements utility functions for reading files
// and getting input lines, including handling indentation
// and multi-line input in a user-friendly way.
//**************************************************

#include "Utils.h"

#include <fstream>
#include <string>
#include <stdexcept>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <vector>
#include <cstdlib>

#include <fcntl.h> // platform specific for file open modes

using std::ifstream;
using std::string;
using std::istreambuf_iterator;
using std::ios;
using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::max;
using std::runtime_error;
using std::vector;
using std::invalid_argument;
using std::make_shared;

namespace fs = std::filesystem;

string Utils::readWholeFile(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        cout << "Error: Could not open file " << filename << endl;
        throw runtime_error("Could not open file: " + filename);
    }
    return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}

bool Utils::fileExists(const string &filename)
{
    ifstream file(filename);
    bool good = file.good();
    file.close();
    return good;
}

string Utils::getInputLine()
{
    string line, part;
    int indent = 0;

    while (true)
    {
        cout << (line.empty() ? "> " : "| ");
        cout.flush();
        if (!getline(cin, part))
        {
            cout << "\r\n";
            return "exit";
        }
        if (part == "exit")
            return "exit";
        if (part.empty())
            continue;

        // count opening and closing braces to track indentation
        for (char c : part)
        {
            if (c == '{') indent++;
            if (c == '}') indent = max(0, indent - 1);
        }

        line += part + "\n";

        // if not able to continue the line, break
        if (indent == 0 && part.back() != '{' && part.back() != '(' && part.back() != '[' && part.back() != ',')
            break;
    }

    // remove trailing newline if it exists
    /*if (!line.empty() && line.back() == '\n')
    {
        line.pop_back();
    }*/

    return line;
}

string Utils::join(const vector<string> &lines, const string &delimiter)
{
    if (lines.empty())
        return "";

    string result;
    for (const auto &line : lines)
    {
        if (!result.empty())
            result += delimiter;
        result += line;
    }
    return result;
}

string Utils::truncate(const string &str, size_t maxLength, const string &suffix)
{
    if (str.length() <= maxLength)
    {
        return str;
    }

    size_t suffixLength = suffix.length();
    if (maxLength <= suffixLength)
    {
        return suffix.substr(0, maxLength);
    }

    return str.substr(0, maxLength - suffixLength) + suffix;
}

string Utils::formatString(const string &format, const vector<string> &args)
{
    // loop through the format string and replace placeholders with arguments, placeholder will be %
    string result;
    size_t argIndex = 0;
    for (size_t i = 0; i < format.length(); ++i)
    {
        if (format[i] == '%' && argIndex < args.size())
        {
            result += args[argIndex++];
        }
        else
        {
            result += format[i];
        }
    }

    return result;
}

string Utils::getModulePath(const string &moduleName, const string &basePath)
{
    //for debugging purposes, print the module name and base path and the current working directory
    // cout << "getModulePath called with moduleName: " << moduleName << ", basePath: " << basePath << "current working directory: " << getCurrentDirectory() << endl;

    string home = getHomeDirectory();
    if (moduleName.empty())
    {
        return string();
    }

    vector<string> searchPaths;

    searchPaths.push_back(basePath);

    // Add default path
    searchPaths.push_back("./modules/");

    // Add home directory path if available
    if (!home.empty())
    {
        searchPaths.push_back(home + "/modules/");
    }

    for (const auto &path : searchPaths)
    {
        string fullPath = path + moduleName + getLithiumFileExtension();
        if (fileExists(fullPath))
        {
            return fullPath;
        }
    }

    return "";
}

string Utils::getHomeDirectory()
{
    const char *home = getenv("HOME");
    if (home)
    {
        return string(home);
    }
    else
    {
        return "";
    }
}

string Utils::getLithiumFileExtension()
{
    return ".li";
}

string Utils::getCurrentDirectory()
{
    return fs::current_path().string();
}

bool Utils::changeDirectory(const string &path)
{
    try
    {
        fs::current_path(path);
        return true;
    }
    catch (const fs::filesystem_error &e)
    {
        return false;
    }
}

vector<string> Utils::listDirectory(const string &path)
{
    vector<string> files;
    for (const auto &entry : fs::directory_iterator(path))
    {
        files.push_back(entry.path().filename().string());
    }
    return files;
}

mode_t Utils::parseOpenMode(const string &mode)
{
    if (mode == "r")
        return O_RDONLY;
    else if (mode == "w")
        return O_WRONLY | O_CREAT | O_TRUNC;
    else if (mode == "a")
        return O_WRONLY | O_CREAT | O_APPEND;
    else if (mode == "r+")
        return O_RDWR;
    else if (mode == "w+")
        return O_RDWR | O_CREAT | O_TRUNC;
    else if (mode == "a+")
        return O_RDWR | O_CREAT | O_APPEND;

    throw invalid_argument("Invalid file open mode: " + mode);
}

void Utils::printArgs(int argc, char **argv)
{
    cout << "Command line arguments: ";
    for (int i = 0; i < argc; ++i)
    {
        cout << argv[i];
        if (i < argc - 1)
        {
            cout << ", ";
        }
    }
    cout << endl;
}

string Utils::ltrim(const string &str)
{
    size_t start = str.find_first_not_of(" \t");
    if (start == string::npos) return ""; // string is all whitespace
    return str.substr(start);
}

void Utils::removePrefix(string &str, const string &prefix)
{
    if (str.find(prefix) == 0)
    {
        str.erase(0, prefix.length());
    }
}

int Utils::getpid()
{
#ifdef _WIN32
    return _getpid(); // Windows-specific implementation
#else
    return ::getpid(); // POSIX implementation
#endif
}

string Utils::getCurrentUser()
{
#ifdef _WIN32
    char username[256];
    DWORD size = sizeof(username);
    if (GetUserNameA(username, &size))
    {
        return string(username);
    }
    else
    {
        return "unknown";
    }
#else
    const char *user = getenv("USER");
    if (user)
    {
        return string(user);
    }
    else
    {
        return "unknown";
    }
#endif
}

// Platform-specific system call wrappers
// These functions provide a cross-platform interface for system calls
// On Unix-like systems, they directly call the system functions
// On Windows, they can be implemented using Windows API equivalents

#ifdef _WIN32
    // Windows-specific includes would go here
#else
    // Unix-like system includes
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/wait.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
#endif

// File I/O operations
ssize_t Utils::readFromFd(int fd, void *buffer, size_t size)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return read(fd, buffer, size);
#endif
}

ssize_t Utils::writeToFd(int fd, const void *data, size_t size)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return write(fd, data, size);
#endif
}

int Utils::closeFd(int fd)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return close(fd);
#endif
}

int Utils::openFile(const string &filename, int flags, mode_t mode)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    if (flags & O_CREAT) {
        return open(filename.c_str(), flags, mode);
    } else {
        return open(filename.c_str(), flags);
    }
#endif
}

// Socket operations
int Utils::createSocket(int domain, int type, int protocol)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return socket(domain, type, protocol);
#endif
}

int Utils::bindSocket(int sockfd, const void *addr, socklen_t addrlen)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return bind(sockfd, (const struct sockaddr *)addr, addrlen);
#endif
}

int Utils::listenSocket(int sockfd, int backlog)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return listen(sockfd, backlog);
#endif
}

int Utils::acceptSocket(int sockfd, void *addr, socklen_t *addrlen)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return accept(sockfd, (struct sockaddr *)addr, addrlen);
#endif
}

int Utils::connectSocket(int sockfd, const void *addr, socklen_t addrlen)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return connect(sockfd, (const struct sockaddr *)addr, addrlen);
#endif
}

ssize_t Utils::sendSocket(int sockfd, const void *buf, size_t len, int flags)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return send(sockfd, buf, len, flags);
#endif
}

ssize_t Utils::receiveSocket(int sockfd, void *buf, size_t len, int flags)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return recv(sockfd, buf, len, flags);
#endif
}

int Utils::setSocketOption(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return setsockopt(sockfd, level, optname, optval, optlen);
#endif
}

// Process operations
int Utils::createPipe(int pipefd[2])
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return pipe(pipefd);
#endif
}

pid_t Utils::forkProcess()
{
#ifdef _WIN32
    // Windows implementation would go here (CreateProcess)
    return -1;
#else
    return fork();
#endif
}

int Utils::waitForProcess(pid_t pid, int *status, int options)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return waitpid(pid, status, options);
#endif
}

int Utils::executeProgram(const char *file, char *const argv[])
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return execvp(file, argv);
#endif
}

// Time operations
void Utils::sleepSeconds(double seconds)
{
#ifdef _WIN32
    // Windows implementation would go here (Sleep)
#else
    usleep(static_cast<useconds_t>(seconds * 1000000));
#endif
}

time_t Utils::getCurrentTime()
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return time(nullptr);
#endif
}

// Network utilities
int Utils::convertAddress(int af, const char *src, void *dst)
{
#ifdef _WIN32
    // Windows implementation would go here
    return -1;
#else
    return inet_pton(af, src, dst);
#endif
}

uint16_t Utils::hostToNetworkShort(uint16_t hostshort)
{
#ifdef _WIN32
    // Windows implementation would go here
    return 0;
#else
    return htons(hostshort);
#endif
}

/*
 * Windows Implementation Notes:
 * ============================
 *
 * To implement Windows support, replace the #ifdef _WIN32 sections above with:
 *
 * 1. File I/O operations:
 *    - Use CreateFile, ReadFile, WriteFile, CloseHandle instead of open/read/write/close
 *    - Map Unix file mode flags to Windows equivalents
 *
 * 2. Socket operations:
 *    - Use WSAStartup/WSACleanup for socket initialization
 *    - Use Winsock2 functions: socket, bind, listen, accept, connect, send, recv
 *    - Handle SOCKET type vs int file descriptors
 *
 * 3. Process operations:
 *    - Use CreateProcess instead of fork/exec
 *    - Use CreatePipe for pipe creation
 *    - Use WaitForSingleObject instead of waitpid
 *
 * 4. Time operations:
 *    - Use Sleep(milliseconds) instead of usleep(microseconds)
 *    - Use GetSystemTime or time_t equivalents
 *
 * 5. Network utilities:
 *    - Use InetPton instead of inet_pton
 *    - Use htons (available on Windows)
 *
 * Example Windows implementation for readFromFd:
 *
 * #ifdef _WIN32
 * ssize_t Utils::readFromFd(int fd, void *buffer, size_t size)
 * {
 *     HANDLE handle = (HANDLE)_get_osfhandle(fd);
 *     if (handle == INVALID_HANDLE_VALUE) return -1;
 *
 *     DWORD bytesRead = 0;
 *     if (!ReadFile(handle, buffer, (DWORD)size, &bytesRead, NULL)) {
 *         return -1;
 *     }
 *     return (ssize_t)bytesRead;
 * }
 * #endif
 */
