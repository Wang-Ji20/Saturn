//===------------------------------------------===
// Saturn 2023
//
// Identification: include\common\exception.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/array.hh"
#include "common/format.hh"
#include "common/string.hh"

#include <stdexcept>
#include <string_view>
#include <vector>

namespace saturn {

// clang-format off
// Exceptions:
//===------------------------------------------------------------------------------------------------------===
// Kind of check	    The throwing method is saying	    Commonly indicated with
// Precondition	        "You messed up (caller)."	        IllegalArgumentException
// Assertion	        "I messed up."	                    InternalException
// Verification	        "Someone I depend on messed up."	VerifyException
// Impossible condition	"What the? the world is messed up!"	UnreachableException
// Not Implemented	    "I didn't finish this yet."	        NotImplementedException
// Exceptional result	"No one messed up"	                other checked or unchecked exceptions
//===------------------------------------------------------------------------------------------------------===
// clang-format on

enum class ExceptionType {
  IllegalArgumentException = 0,
  InternalException,
  VerifyException,
  UnreachableException,
  NotImplementedException,
  OtherException,
  BuiltinExceptionNumberCount
};

static constexpr array<const char *,
                       size_t(ExceptionType::BuiltinExceptionNumberCount)>
    ExceptionTypeString = {"IllegalArgumentException ",
                           "InternalException ",
                           "VerifyException ",
                           "UnreachableException ",
                           "NotImplementedException ",
                           "OtherException "};

class Exception : public std::exception {
public:
  Exception(ExceptionType type, const string &message)
      : type(type), message_(ExceptionTypeString[size_t(type)] + message){};

  ExceptionType type;

  [[nodiscard]] auto what() const noexcept -> const char * override {
    return message_.c_str();
  }

private:
  string message_;
};

class IllegalArgumentException : public Exception {
public:
  explicit IllegalArgumentException(const string &message)
      : Exception(ExceptionType(ExceptionType::IllegalArgumentException),
                  message) {}
};

class InternalException : public Exception {
public:
  explicit InternalException(const string &message)
      : Exception(ExceptionType(ExceptionType::InternalException), message) {}
};

class VerifyException : public Exception {
public:
  explicit VerifyException(const string &message)
      : Exception(ExceptionType(ExceptionType::VerifyException), message) {}
};

class UnreachableException : public Exception {
public:
  explicit UnreachableException(const string &message)
      : Exception(ExceptionType(ExceptionType::UnreachableException), message) {
  }
};

class NotImplementedException : public Exception {
public:
  explicit NotImplementedException(const string &message)
      : Exception(ExceptionType::NotImplementedException, message) {}
};

class OtherException : public Exception {
public:
  explicit OtherException(const string &message)
      : Exception(ExceptionType::OtherException, message) {}
};

} // namespace saturn
