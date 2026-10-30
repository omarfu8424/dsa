# dsa

## 目录结构

- `include/`：各数据结构模块的公共头文件和模板实现。
- `src/`：示例程序、库实现和按模块组织的源文件。
- `tests/`：CTest 测试。
- `resources/input.txt`：Huffman 示例输入。

## 构建与测试

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
