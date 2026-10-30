cmake_minimum_required(VERSION 3.16)
project(MyDSAProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Header-only modules expose their public headers through the include tree.
add_library(List INTERFACE)
target_include_directories(List INTERFACE ${PROJECT_INCLUDE_DIR}/List)
add_executable(List_main src/List/main.cpp)
target_link_libraries(List_main PRIVATE List)
add_executable(List_polynominal src/List/polynominal.cpp)
target_link_libraries(List_polynominal PRIVATE List)

add_library(Matrix INTERFACE)
target_include_directories(Matrix INTERFACE ${PROJECT_INCLUDE_DIR}/Matrix)

add_library(Tree INTERFACE)
target_include_directories(Tree INTERFACE ${PROJECT_INCLUDE_DIR}/Tree)
add_executable(Tree_bintree src/Tree/BinTree.cpp)
target_link_libraries(Tree_bintree PRIVATE Tree)
add_executable(tree_Huffman src/Tree/Huffman.cpp)
target_link_libraries(tree_Huffman PRIVATE Tree)
target_compile_definitions(tree_Huffman PRIVATE
  DSA_INPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/resources/input.txt")

add_executable(BinTree_tests tests/BinTree_tests.cpp)
target_link_libraries(BinTree_tests PRIVATE Tree)
add_test(NAME BinTree_tests COMMAND BinTree_tests)

add_library(String STATIC src/String/String_implementation.cpp)
target_include_directories(String PUBLIC ${PROJECT_INCLUDE_DIR}/String)
add_executable(string_main src/String/main.cpp)
target_link_libraries(string_main PRIVATE String)

add_executable(hello src/hello.cpp)
add_executable(sum_basic src/sum/basic.cpp)
add_executable(sort_basic src/sort/basic.cpp)
add_executable(stack_RPN src/Stack/RPN.cpp)
add_executable(stack_expression src/Stack/expression.cpp)
add_executable(stack_convert src/Stack/convert.cpp)
add_executable(stack_matchBraces src/Stack/matchBraces.cpp)
add_executable(stack_queen src/Stack/queen.cpp)

add_executable(fib_main src/Fib/main.cpp)
target_include_directories(fib_main PRIVATE ${PROJECT_INCLUDE_DIR}/Fib)
add_executable(fib_basic src/Fib/basic.cpp)
target_include_directories(fib_basic PRIVATE ${PROJECT_INCLUDE_DIR}/Fib)

add_executable(search_basic src/search/basic.cpp)
target_include_directories(search_basic PRIVATE ${PROJECT_INCLUDE_DIR}/Fib)