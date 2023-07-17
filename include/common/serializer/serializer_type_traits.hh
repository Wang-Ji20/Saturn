//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/serializer/serializer_type_traits.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/shared_ptr.hh"
#include "common/unique_ptr.hh"

#include <type_traits>

namespace saturn {

class Deserializer;

template <typename T, typename = T> struct has_deserialize : std::false_type {};

// clang-format off
template <typename T>
struct has_deserialize<
    T,
    typename std::enable_if<std::is_same<decltype(T::SaturnReadValue),
                                         unique_ptr<T>(Deserializer &) // function unique_ptr<T> functionName(Deserializer&);
                                        >::value,
                          T>::type
                      > : std::true_type {};

template <typename T>
struct has_deserialize<
    T,
    typename std::enable_if<std::is_same<decltype(T::SaturnReadValue),
                                         shared_ptr<T>(Deserializer &)
                                        >::value,
                            T
                           >::type
                      > : std::true_type {};

template <typename T>
struct has_deserialize<
    T,
    typename std::enable_if<
        std::is_same<decltype(T::SaturnReadValue), T(Deserializer &)>::value,
                          T>::type
                      > : std::true_type {};

template <typename T> struct is_deserializable : has_deserialize<T> {};

template <typename T> struct is_unique_ptr : std::false_type {};


template <typename T>
struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {
  using ELEMENT_TYPE = T;
};

template <typename T> struct is_shared_ptr : std::false_type {};

template <typename T> struct is_shared_ptr<shared_ptr<T>> : std::true_type {
  using ELEMENT_TYPE = T;
};

} // namespace saturn
