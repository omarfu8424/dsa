#include <cstdlib>
#include <queue>
#include <stack>
template <typename T> struct BinNode;
template <typename T> using BinNodePosi = BinNode<T> *;
using Rank = unsigned int;

template <typename T> struct BinNode {
private:
  T _data;
  BinNodePosi<T> _parent;
  BinNodePosi<T> _lc;
  BinNodePosi<T> _rc;
  Rank _height{};

protected:
  // 先序遍历 root-l-r
  // 迭代
  template <typename VST>
  void travPre_iteration_1(BinNodePosi<T> root, VST &visit) {
    std::stack<BinNodePosi<T>> s;
    if (root) {
      s.push(root);
    }
    while (!s.empty()) {
      root = s.top();
      visit(root);
      s.pop();
      if (root->_rc) {
        s.push(root->_rc);
      }
      if (root->_lc) { // 这样左孩子先被visit
        s.push(root->_lc);
      }
    }
  }
  template <typename VST>
  void travPre_iteration_2(BinNodePosi<T> root, VST &visit);
  // 递归
  template <typename VST>
  void travPre_recursion(BinNodePosi<T> root, VST &visit) {
    if (!root) {
      return;
    }
    visit(root);
    travPre_recursion(root->_lc, visit);
    travPre_recursion(root->_rc, visit);
  }
  // 中序遍历 l-root-r
  // 迭代
  template <typename VST>
  void travIn_iteration(BinNodePosi<T> root, VST &visit) {
    std::stack<BinNodePosi<T>> s;
    BinNodePosi<T> curr = root;

    while (!s.empty() || curr) {
      while(curr){
        s.push(curr);
        curr = curr->_lc;
      }
      // s.top()为当前未访问的最左侧结点
      curr = s.top();
      s.pop(); visit(curr);
      curr = curr->_rc;
    }
  }
  // 递归
  template <typename VST>
  void travIn_recursion(BinNodePosi<T> root, VST &visit) {
    if (!root) {
      return;
    }
    travIn_recursion(root->_lc, visit);
    visit(root);
    travIn_recursion(root->_rc, visit);
  }

public:
  // 构造
  BinNode() : _parent(nullptr), _lc(nullptr), _rc(nullptr) {};
  BinNode(T d, BinNodePosi<T> p = nullptr, BinNodePosi<T> l = nullptr,
          BinNodePosi<T> r = nullptr, Rank h = 0)
      : _data(d), _parent(p), _lc(l), _rc(r), _height(h) {};
  // 插入孩子
  BinNodePosi<T> insertLC(const T &d) { return _lc = new BinNode<T>(d, this); }
  BinNodePosi<T> insertRC(const T &d) { return _rc = new BinNode<T>(d, this); }
  // 只读
  [[nodiscard]] Rank size() const {
    Rank s = 1;
    if (_lc) {
      s += _lc->size();
    }
    if (_rc) {
      s += _rc->size();
    }
    return s;
  }
  // 遍历
  template <typename VST> void travLevel(VST &visit); // 层次遍历
  template <typename VST> void travPre(VST &visit);   // 先序遍历
  template <typename VST> void travIn(VST &visit);    // 中序遍历
  template <typename VST> void travPost(VST &visit);  // 后序遍历
};
// 层次遍历 BFS
template <typename T>
template <typename VST>
void BinNode<T>::travLevel(VST &visit) {
  std::queue<BinNodePosi<T>> q;
  if (this) {
    q.push(this);
  }
  while (!q.empty()) {
    BinNodePosi<T> root = q.front();
    visit(root);
    q.pop();
    if (root->_lc) {
      q.push(root->_lc);
    }
    if (root->_rc) {
      q.push(root->_rc);
    }
  }
}
// 先序遍历 PLR
template <typename T>
template <typename VST>
void BinNode<T>::travPre(VST &visit) {
  switch (rand() % 3) {
  case 0:
    travPre_iteration_1(this, visit);
    break;
  case 1:
    travPre_iteration_2(this, visit);
    break;
  case 2:
    travPre_recursion(this, visit);
    break;
  default:
    return;
  }
}