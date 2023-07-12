# Saturn 开发日志 01，实用工具

## 编译与构建

### 编译器

编译程序是一个非常复杂的过程。最普通的 Hello World 程序，在打出 `c++ -v hello.cc` 之后，显示出的实际编译参数也非常多（见下面的代码块）。编译系统，`GCC`, `LLVM`, `MSVC`, 的用户手册往往长达数千页。开发者在其中事无巨细描述每一个编译选项。查阅 `clang` 的用户手册，才能明白下面这一长串东西在说什么：

```bash
"/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
-cc1 -triple x86_64-apple-macosx13.0.0 -Wundef-prefix=TARGET_OS_ -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -Werror=implicit-function-declaration 
-emit-obj
-mrelax-all --mrelax-relocations
-disable-free
-clear-ast-before-backend
-disable-llvm-verifier 
-discard-value-names 
-main-file-name hello.cc 
-mrelocation-model pic -pic-level 2
-mframe-pointer=all
-fno-strict-return -ffp-contract=on -fno-rounding-math -funwind-tables=2 -target-sdk-version=13.3 -fvisibility-inlines-hidden-static-local-var
-target-cpu penryn -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning -debugger-tuning=lldb -target-linker-version 857.1
-v -fcoverage-compilation-dir=/tmp
-clang-vendor-feature=+disableNonDependentMemberExprInCurrentInstantiation -fno-odr-hash-protocols -clang-vendor-feature=+enableAggressiveVLAFolding -clang-vendor-feature=+revert09abecef7bbf -clang-vendor-feature=+thisNoAlignAttr -clang-vendor-feature=+thisNoNullAttr -mllvm -disable-aligned-alloc-awareness=1 -D__GCC_HAVE_DWARF2_CFI_ASM=1 
-fdeprecated-macro -fdebug-compilation-dir=/tmp -ferror-limit 19 -stack-protector 1 -fstack-check -mdarwin-stkchk-strong-link -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fno-cxx-modules -no-opaque-pointers -fcxx-exceptions -fexceptions -fmax-type-align=16 -fcommon -fcolor-diagnostics

# 以上一长段都太细节，通常来说，用户不需要手动调节

-resource-dir /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/14.0.3
# sysroot 路径下有 include/ 和 lib/ 编译器会在这个目录下寻找头文件和库。
-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk 

# 编译器会在 -I<路径> 选项指定的路径下寻找头文件
-I/usr/local/include -stdlib=libc++ # stdlib 指的是 C++ 标准库。LLVM 的实现为 libc++，GNU 的实现为 libstdc++。

# 这些指定系统头文件的搜索路径，也就是 #include <> 的头文件路径
-internal-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1 
-internal-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/local/include 
-internal-isystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/14.0.3/include

# C 和 C++ 是两门不同的语言！虽然 C++ 兼容大部分的 C 语言语法，但是链接阶段会出问题。所以加入 C 语言的头文件是特殊的。
-internal-externc-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include 
-internal-externc-isystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include 

# 编译器警告选项
-Wno-reorder-init-list -Wno-implicit-int-float-conversion -Wno-c99-designator -Wno-final-dtor-non-final-class -Wno-extra-semi-stmt -Wno-misleading-indentation -Wno-quoted-include-in-framework-header -Wno-implicit-fallthrough -Wno-enum-enum-conversion -Wno-enum-float-conversion -Wno-elaborated-enum-base -Wno-reserved-identifier -Wno-gnu-folding-constant

# 输出的目标文件位置
-o /var/folders/1f/fg6d0gxx1_3_5rpmmvk592s80000gn/T/hello-e5a929.o

# 编程语言为 C++
-x c++

# 源文件
hello.cc
```

在我们只有一个文件的时候，只要使用默认的编译选项就好了。但是，我们的项目文件越来越多以后，默认的编译选项 `c++ *.cpp` 也浮现出越来越多的问题：
  * 默认生成一个可执行文件。那么，如果项目中含有多个可执行文件，或者根本没有可执行文件的时候，怎么办呢？
> 为什么会没有可执行文件？
> 通常我们写一些可执行的程序。但是有些时候，我们需要写一些代码，向外暴露接口，供其他可执行程序使用。这些可重用的代码片段称为库（library）。例如，`linenoise` 是一个命令行终端库，提供一个头文件，其他应用可以调用这个头文件里的函数，来读取用户的输入，并且提供自动补全、历史命令的功能。事实上，许多的项目都是一些巨大的代码库，可执行文件反而被看作一种 “驱动”，一种 “药引”，用来向库中的类/函数提供输入输出的能力，而不是最主要的东西。
  * 找不到头文件！我需要用 -I 指定头文件的位置。
  * 链接不到外部库！我需要用 -l 指定需要我链接的库。

> TL;DR: 我们需要构建系统

### 构建系统

我们当然不能手动在命令行敲这些命令。当我们想自动做一些事情的时候，脚本出现了。我们当然可以用 `.sh` `.bat` 来编译程序，但用一些特殊的构建工具更加方便。[`Make`](https://makefiletutorial.com) 提供了很多便利的功能，让它成为了一个非常流行的构建系统。其他常见的构建系统还有 `Ninja`, `Gradle`, `Maven` 等。

当用户在命令行输入 `make` 之后，它会从当前目录寻找一个名为 `makefile` 的文件。在里面用户编写构建的逻辑。其中的每一条都有这样的结构：

```make
target: dep1 dep2
  command
```

其中 target 是用户打 `make <target>` 时候指定的目标(什么都不指定的时候，为第一个)。而 dep1 dep2 为构建此目标之前的依赖。command 为构建此目标时执行的命令。

target 既可以是实际的文件名，也可以是一个自定义的命令。当它不是实存文件的时候，需要使用 `.PHONY` 来区别。关于变量、通配符等内容，详见[这篇教程](https://makefiletutorial.com)。


> TL;DR: `make` 提供了什么？一键构建。`make` 有什么问题？太难写。

### Build Generator

> 命令式编程（C）：详细描述做一件事情的过程。声明式编程（SQL，React）：描述要什么样的东西。

在很多时候，命令式的东西都会被声明式的取代。构建工具也是如此。与其写 `g++ -l$(libs) -I$(include_path)`，直接写 `include(...)` 或许是更便利的做法。这样做在构建系统的上方出现了一个新的中间层，称作 `build generator`，也就是构建生成器。在编译程序的时候，构建生成器生成构建系统，然后构建系统再调用编译器，编译器再调用代码生成器生成代码...

这样做还有许多好处。正如一切中间层一样，它增强了构建系统的可移植性。不同的编译器，不同的操作系统，可能会有不同的命令行指令，文件的位置也不一样。尤其是 `Windows` 往往和其他操作系统差别很大，甚至使用反斜杠。所以，我们需要使用 `CMake` 这个构建生成器。

`CMake` 的配置文件为 `CMakeLists.txt`。在每个含有程序源文件（头文件不需要）的目录下，都应该有一个 `CMakeLists.txt`。

```CMake
# 常用变量
# CMAKE_CURRENT_SOURCE_DIR
# CMAKE_CXX_FLAGS
# ALL_OBJECT_FILES

# 下面介绍一些常用的指令

cmake_minimum_required

# 
project(...)


set()

message()

add_subdirectory()

include_directories()

add_library()

target_link_libraries()

target_include_directories()



```


## 测试工具

### ctest

### googletest

### LCOV

### sqllogictest

## 诊断工具

### Sanitizer

### Clang-tools

### Language Server

### Compiler explorer

### Flame Graph

## 外部实用库

### abseil
