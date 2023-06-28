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

static constexpr array<string_view,
                       size_t(ExceptionType::BuiltinExceptionNumberCount)>
    ExceptionTypeString = {"IllegalArgumentException", "InternalException",
                           "VerifyException",          "UnreachableException",
                           "NotImplementedException",  "OtherException"};

class Exception : public std::exception {
public:
  explicit Exception(const string &message);
  Exception(ExceptionType type, const string &message);

  template <typename... Args>
  explicit Exception(ExceptionType type, const string &message, Args &&...args)
      : message_(format(message, std::forward<Args>(args)...)) {}

  ExceptionType type;

  // TODO: add stack trace

private:
  string message_;
};

class IllegalArgumentException : public Exception {
public:
  explicit IllegalArgumentException(const string &message);

  template <typename... Args>
  explicit IllegalArgumentException(const string &message, Args &&...args)
      : Exception(ExceptionType::IllegalArgumentException, message,
                  std::forward<Args>(args)...) {}
};

class InternalException : public Exception {
public:
  explicit InternalException(const string &message);

  template <typename... Args>
  explicit InternalException(const string &message, Args &&...args)
      : Exception(ExceptionType::InternalException, message,
                  std::forward<Args>(args)...) {}
};

class VerifyException : public Exception {
public:
  explicit VerifyException(const string &message);

  template <typename... Args>
  explicit VerifyException(const string &message, Args &&...args)
      : Exception(ExceptionType::VerifyException, message,
                  std::forward<Args>(args)...) {}
};

class UnreachableException : public Exception {
public:
  explicit UnreachableException(const string &message);

  template <typename... Args>
  explicit UnreachableException(const string &message, Args &&...args)
      : Exception(ExceptionType::UnreachableException, message,
                  std::forward<Args>(args)...) {}
};

class NotImplementedException : public Exception {
public:
  explicit NotImplementedException(const string &message);

  template <typename... Args>
  explicit NotImplementedException(const string &message, Args &&...args)
      : Exception(ExceptionType::NotImplementedException, message,
                  std::forward<Args>(args)...) {}
};

class OtherException : public Exception {
public:
  explicit OtherException(const string &message);

  template <typename... Args>
  explicit OtherException(const string &message, Args &&...args)
      : Exception(ExceptionType::OtherException, message,
                  std::forward<Args>(args)...) {}
};

} // namespace saturn
