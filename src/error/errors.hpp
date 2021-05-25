#ifndef ERRORS_HPP
#define ERRORS_HPP
#include <stdexcept>
#include <string>
// place your custom errors/exceptions here

class TypeCastException : std::runtime_error {
   public:
    TypeCastException(const std::string& msg) : std::runtime_error{std::move(msg)} {
    }
};

class InvalidIdException : std::runtime_error {
   public:
    InvalidIdException(const std::string& msg) : std::runtime_error{std::move(msg)} {
    }
};

class InvalidTypeException : std::runtime_error {
   public:
    InvalidTypeException(const std::string& msg) : std::runtime_error{std::move(msg)} {
    }
};

#endif