cmake_minimum_required(VERSION 3.16)
project(MyDSAProject)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_COMPILE_FEATURES "/usr/bin/g++")
# set(CMAKE_C_COMPILE_FEATURES "/usr/bin/gcc")


# 添加 String 静态库
add_library(String STATIC String/String_implementation.cpp)
target_include_directories(String PUBLIC String)
# 可执行文件：String/main（链接 String 库）
add_executable(string_main String/main.cpp)
target_link_libraries(string_main PRIVATE String)



# 添加 List 库
add_library(List INTERFACE)
target_include_directories(List INTERFACE List)
# 可执行文件：List/main（链接 List 库）
add_executable(List_main List/main.cpp)
target_link_libraries(List_main PRIVATE List)
# 可执行文件：List/polynominal
add_executable(List_polynominal List/polynominal.cpp)


# 添加 Matrix 库
add_library(Matrix INTERFACE)
target_include_directories(Matrix INTERFACE Matrix)








# 可执行文件：hello
add_executable(hello hello.cpp)

# 可执行文件：sum/basic
add_executable(sum_basic sum/basic.cpp)

# 可执行文件：sort/basic
add_executable(sort_basic sort/basic.cpp)

# 可执行文件：Stack
add_executable(stack_expression Stack/expression.cpp)
add_executable(stack_convert Stack/convert.cpp)
add_executable(stack_matchBraces Stack/matchBraces.cpp)

# 可执行文件：Fib/main
add_executable(fib_main Fib/main.cpp)
target_include_directories(fib_main PRIVATE Fib)

# 可执行文件：Fib/basic
add_executable(fib_basic Fib/basic.cpp)
target_include_directories(fib_basic PRIVATE Fib)



# 可执行文件：search/basic（包含 Fib 头文件）
add_executable(search_basic search/basic.cpp)
target_include_directories(search_basic PRIVATE Fib)