//**************************************************
// File: Utils.h
//
// Author: Bryce Schultz
//
// Purpose:  Declares utility functions for reading files
// and getting input lines, including handling indentation
// and multi-line input in a user-friendly way.
// This is useful for parsing and interpreting code in a
// more natural way, allowing users to write code that
// spans multiple lines without worrying about syntax errors
// due to line breaks or indentation issues.
//**************************************************
#pragma once

#include <string>
#include <vector>
#include <cstddef>  // for size_t
#include <sys/types.h>  // for pid_t, ssize_t, mode_t
#include <cstdint>  // for uint16_t
#include <ctime>    // for time_t

#ifdef _WIN32
    // Windows-specific types would go here
    typedef unsigned int socklen_t;
#else
    // Unix-like system types
    #include <sys/socket.h>  // for socklen_t
#endif

// macros for unused parameters to avoid compiler warnings
#define UNUSED(x) (void)(x)

namespace Utils
{
    using std::string;
    using std::vector;

    string readWholeFile(const string &filename);

    bool fileExists(const string &filename);

    string getInputLine();

    string join(const vector<string> &lines, const string &delimiter = "\n");

    string truncate(const string &str, size_t maxLength, const string &suffix = "...");

    string formatString(const string &format, const vector<string> &args);

    string getModulePath(const string &moduleName, const string &basePath = "");

    string getHomeDirectory();

    string getLithiumFileExtension();

    string getCurrentDirectory();

    vector<string> listDirectory(const string &path);

    mode_t parseOpenMode(const string &mode);

    // Platform-specific system call wrappers
    // These functions provide a cross-platform interface for system calls
    // that can be implemented differently on Windows vs Unix-like systems

    // File I/O operations
    ssize_t readFromFd(int fd, void *buffer, size_t size);
    ssize_t writeToFd(int fd, const void *data, size_t size);
    int closeFd(int fd);
    int openFile(const string &filename, int flags, mode_t mode = 0);

    // Socket operations
    int createSocket(int domain, int type, int protocol);
    int bindSocket(int sockfd, const void *addr, socklen_t addrlen);
    int listenSocket(int sockfd, int backlog);
    int acceptSocket(int sockfd, void *addr, socklen_t *addrlen);
    int connectSocket(int sockfd, const void *addr, socklen_t addrlen);
    ssize_t sendSocket(int sockfd, const void *buf, size_t len, int flags);
    ssize_t receiveSocket(int sockfd, void *buf, size_t len, int flags);
    int setSocketOption(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

    // Process operations
    int createPipe(int pipefd[2]);
    pid_t forkProcess();
    int waitForProcess(pid_t pid, int *status, int options);
    int executeProgram(const char *file, char *const argv[]);

    // Time operations
    void sleepSeconds(double seconds);
    time_t getCurrentTime();

    // Network utilities
    int convertAddress(int af, const char *src, void *dst);
    uint16_t hostToNetworkShort(uint16_t hostshort);

    void printArgs(int argc, char **argv);

    string ltrim(const string &str);

    void removePrefix(string &str, const string &prefix);
}