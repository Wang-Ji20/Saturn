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
  explicit Exception(string message) : message_(std::move(message)) {}
  Exception(ExceptionType type, const char *message);

  template <typename... Args>
  explicit constexpr Exception(ExceptionType type,
                               string_view message,
                               Args &&...args)
      : message_(absl::StrFormat(message, std::forward<Args>(args)...)) {}

  ExceptionType type;

private:
  string message_;
};

class IllegalArgumentException : public Exception {
public:
  explicit IllegalArgumentException(const char *message) : Exception(message){};

  template <typename... Args>
  explicit constexpr IllegalArgumentException(string_view message,
                                              Args &&...args)
      : Exception(ExceptionType::IllegalArgumentException,
                  message,
                  std::forward<Args>(args)...) {}
};

class InternalException : public Exception {
public:
  explicit InternalException(const char *message = "Internal")
      : Exception(message) {}

  template <typename... Args>
  explicit constexpr InternalException(string_view message, Args &&...args)
      : Exception(ExceptionType::InternalException,
                  message,
                  std::forward<Args>(args)...) {}
};

class VerifyException : public Exception {
public:
  explicit VerifyException(const char *message);

  template <typename... Args>
  explicit constexpr VerifyException(string_view message, Args &&...args)
      : Exception(ExceptionType::VerifyException,
                  message,
                  std::forward<Args>(args)...) {}
};

class UnreachableException : public Exception {
public:
  explicit UnreachableException(const char *message);

  template <typename... Args>
  explicit constexpr UnreachableException(string_view message, Args &&...args)
      : Exception(ExceptionType::UnreachableException,
                  message,
                  std::forward<Args>(args)...) {}
};

class NotImplementedException : public Exception {
public:
  explicit NotImplementedException(const char *message);

  template <typename... Args>
  explicit constexpr NotImplementedException(string_view message,
                                             Args &&...args)
      : Exception(ExceptionType::NotImplementedException,
                  message,
                  std::forward<Args>(args)...) {}
};

class OtherException : public Exception {
public:
  explicit OtherException(const char *message);

  template <typename... Args>
  explicit constexpr OtherException(string_view message, Args &&...args)
      : Exception(ExceptionType::OtherException,
                  message,
                  std::forward<Args>(args)...) {}
};

} // namespace saturn
