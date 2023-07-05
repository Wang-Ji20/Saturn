//===------------------------------------------===
// Saturn 2023
//
// Identification: include\port\winapi.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#ifndef SATURN_API

#ifdef _WIN32
#define SATURN_API __declspec(dllexport)
#else
#define SATURN_API
#endif // _WIN32

#endif // SATURN_API
