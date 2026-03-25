#ifndef SHARESPHERE_EXCEPTION_H
#define SHARESPHERE_EXCEPTION_H

#include <exception>
#include <string>


// Base exception class for all ShareSphere exceptions (inherits from std::exception)
class ShareSphereException : public std::exception
{
protected:
    std::string message;

public:
    explicit ShareSphereException(const std::string &msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
    virtual ~ShareSphereException() = default;
};

// Thrown when file I/O operations fail (demonstrates inheritance in exception hierarchy)
class FileException : public ShareSphereException
{
public:
    explicit FileException(const std::string &msg)
        : ShareSphereException("[File Error] " + msg) {}
};

// Thrown when authentication fails
class AuthenticationException : public ShareSphereException
{
public:
    explicit AuthenticationException(const std::string &msg)
        : ShareSphereException("[Auth Error] " + msg) {}
};

// Thrown when input validation fails
class InvalidInputException : public ShareSphereException
{
public:
    explicit InvalidInputException(const std::string &msg)
        : ShareSphereException("[Input Error] " + msg) {}
};

// Thrown when a requested resource (item/user/request) is not found
class NotFoundException : public ShareSphereException
{
public:
    explicit NotFoundException(const std::string &msg)
        : ShareSphereException("[Not Found] " + msg) {}
};

#endif
