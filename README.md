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

## 随机 BST 高度实验

Devroye 的论文研究的是：把 `1..n` 的随机排列依次插入普通二叉搜索树。
在这个特定随机模型下，树高 `H_n` 满足

```text
H_n / ln(n) -> 4.31107...
```

这不是普通 BST 对任意输入的平衡保证：升序或降序插入会产生高度
`n - 1` 的链。可以运行仓库中的实验比较三种构造：

```bash
./build/BST_height_experiment
```

实验使用固定种子，输出随机插入的平均高度及其与 `ln(n)` 的比值，同时
输出升序插入和近似完全平衡插入的高度。有限规模的实验只能帮助建立直觉，
不能替代理论中的渐近结论；若工程需要严格的 `O(log n)` 最坏情况，应使用
AVL 树、红黑树等带平衡不变量的数据结构。