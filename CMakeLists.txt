cmake_minimum_required(VERSION 3.16)
project(MyDSAProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
# 所有模块共享的头文件搜索路径，等价于 .vscode/build.sh 的行为。
# 同时暴露 include 根目录，使 "Graph/GraphList.h" 这类带前缀写法也能解析。
add_library(dsa_headers INTERFACE)
target_include_directories(dsa_headers INTERFACE
  ${PROJECT_INCLUDE_DIR}
  ${PROJECT_INCLUDE_DIR}/List
  ${PROJECT_INCLUDE_DIR}/Matrix
  ${PROJECT_INCLUDE_DIR}/Tree
  ${PROJECT_INCLUDE_DIR}/String
  ${PROJECT_INCLUDE_DIR}/Fib
  ${PROJECT_INCLUDE_DIR}/Graph
)


# Header-only modules expose their public headers through the include tree.
add_library(List INTERFACE)
target_link_libraries(List INTERFACE dsa_headers)
add_executable(List_main src/List/main.cpp)
target_link_libraries(List_main PRIVATE List)
add_executable(List_polynominal src/List/polynominal.cpp)
target_link_libraries(List_polynominal PRIVATE List)

add_executable(List_tests tests/List_tests.cpp)
target_link_libraries(List_tests PRIVATE List)
add_test(NAME List_tests COMMAND List_tests)

add_library(Matrix INTERFACE)
target_link_libraries(Matrix INTERFACE dsa_headers)

add_library(Tree INTERFACE)
target_link_libraries(Tree INTERFACE dsa_headers)
add_executable(Tree_bintree src/Tree/BinTree.cpp)
target_link_libraries(Tree_bintree PRIVATE Tree)
add_executable(tree_Huffman src/Tree/Huffman.cpp)
target_link_libraries(tree_Huffman PRIVATE Tree)
target_compile_definitions(tree_Huffman PRIVATE
  DSA_INPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/resources/input.txt")

add_executable(BinTree_tests tests/BinTree_tests.cpp)
target_link_libraries(BinTree_tests PRIVATE Tree)
add_test(NAME BinTree_tests COMMAND BinTree_tests)

add_executable(BST_tests tests/BST_tests.cpp)
target_link_libraries(BST_tests PRIVATE Tree)
add_test(NAME BST_tests COMMAND BST_tests)

add_executable(AVL_tests tests/AVL_tests.cpp)
target_link_libraries(AVL_tests PRIVATE Tree)
add_test(NAME AVL_tests COMMAND AVL_tests)

add_executable(BST_height_experiment tests/BST_height_experiment.cpp)
target_link_libraries(BST_height_experiment PRIVATE Tree)
add_test(NAME BST_height_experiment COMMAND BST_height_experiment)

add_executable(AVL_height_experiment tests/AVL_height_experiment.cpp)
target_link_libraries(AVL_height_experiment PRIVATE Tree)
add_test(NAME AVL_height_experiment COMMAND AVL_height_experiment)


add_library(Graph INTERFACE)
target_link_libraries(Graph INTERFACE dsa_headers)

add_executable(GraphMatrix_tests tests/GraphMatrix_tests.cpp)
target_link_libraries(GraphMatrix_tests PRIVATE Graph)
add_test(NAME GraphMatrix_tests COMMAND GraphMatrix_tests)

add_executable(GraphList_tests tests/GraphList_tests.cpp)
target_link_libraries(GraphList_tests PRIVATE Graph)
add_test(NAME GraphList_tests COMMAND GraphList_tests)




add_library(String STATIC src/String/String_implementation.cpp)
target_link_libraries(String PUBLIC dsa_headers)
add_executable(string_main src/String/main.cpp)
target_link_libraries(string_main PRIVATE String)

# 单文件程序同样链接 dsa_headers：clangd 给孤立头文件插值编译命令时会随机
# 挑一条，只要有任何一条命令缺少 -I，头文件就会报 file not found。
add_executable(hello src/hello.cpp)
add_executable(sum_basic src/sum/basic.cpp)
add_executable(sort_basic src/sort/basic.cpp)
add_executable(stack_RPN src/Stack/RPN.cpp)
add_executable(stack_expression src/Stack/expression.cpp)
add_executable(stack_convert src/Stack/convert.cpp)
add_executable(stack_matchBraces src/Stack/matchBraces.cpp)
add_executable(stack_queen src/Stack/queen.cpp)
target_link_libraries(hello PRIVATE dsa_headers)
target_link_libraries(sum_basic PRIVATE dsa_headers)
target_link_libraries(sort_basic PRIVATE dsa_headers)
target_link_libraries(stack_RPN PRIVATE dsa_headers)
target_link_libraries(stack_expression PRIVATE dsa_headers)
target_link_libraries(stack_convert PRIVATE dsa_headers)
target_link_libraries(stack_matchBraces PRIVATE dsa_headers)
target_link_libraries(stack_queen PRIVATE dsa_headers)

add_executable(fib_main src/Fib/main.cpp)
target_link_libraries(fib_main PRIVATE dsa_headers)
add_executable(fib_basic src/Fib/basic.cpp)
target_link_libraries(fib_basic PRIVATE dsa_headers)

add_executable(search_basic src/search/basic.cpp)
target_link_libraries(search_basic PRIVATE dsa_headers)