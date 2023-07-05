# Saturn

A database system for learning database implementation.

what i cannot invent, i cannot understand.

the project will be in C++ 17.

# File Structure

The organization of this repo is as follows:

.github/
    github action scripts. now it runs all test cases upon push.

build_support/
    I put helper tools here, like build scripts, formatting utilities, lint scripts, cmake modules...

docs/
    some development diaries, hints, tutorial, examples about this project.

include/
    C++ headers for this project. The header and source code is separated into two folders. `include/` has all headers while `lib/` has all source files.
    The headers are scattered to different modules according to their roles.

lib/
    C++ source code for this project.

test/
    I use Google Test to test my codes, and this folder contains all test cases.

third_party/
    I put third party C++ source libraries here using git submodules so that we can access to the newest main commit of dependencies.

tools/
    execuable targets for this project. Nothing yet.

.clang-format
    This projects roughly follows the LLVM code style, but also have some additional rules.

.clangd
    Configuration file for C++ language server clangd. It provides code intelisense, auto complete and dignosis advices.

.gitmodules
    git submodule configuration file.

CMakeLists.txt
    for build generator CMake. describes the project structure in compiler way.

README.md
    This file.
