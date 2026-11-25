#pragma once
#include "List.h"
#include <iostream>
#include <utility>

// protected
template <typename T> void List<T>::init() {
  _header = new ListNode<T>;
  _trailer = new ListNode<T>;
  _header->succ = _trailer;
  _trailer->pred = _header;
  _size = 0;
}
template <typename T> Rank List<T>::clear() {
  if (_header == nullptr) {
    return 0;
  }
  Rank old = _size;
  for (ListNodePosit<T> i = _header->succ; i != _trailer;) {
    ListNodePosit<T> next = i->succ;
    delete i;
    _size--;
    i = next;
  }
  _header->succ = _trailer;
  _trailer->pred = _header;
  return old;
}
template <typename T> void List<T>::copy_nodes(ListNodePosit<T> p, Rank n) {
  while (n-- > 0) {
    insert_as_last(p->data);
    p = p->succ;
  }
}

// 构造函数
template <typename T>
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
List<T>::List(List<T> const &L, Rank r, Rank n) {
  init();
  ListNodePosit<T> p = L.first();
  while (r-- > 0) {
    p = p->succ;
  }
  copy_nodes(p, n);
}
template <typename T>
List<T>::List(List &&L) noexcept : List() {
  // 委托默认构造先建好空列表哨兵,再走移动赋值交换:this 取走 L 的全部资源,
  // L 得到 this 的空哨兵,成为可继续使用的空列表。
  // 注:noexcept 下 init() 的分配若失败会 terminate;若需强异常安全可移除 noexcept。
  *this = std::move(L);
}
template <typename T> List<T> &List<T>::operator=(List<T> const &L) {
  if (this != &L) {
    clear();
    copy_nodes(L.first(), L._size);
  }
  return *this;
}
template <typename T> List<T> &List<T>::operator=(List<T> &&L) noexcept {
  if (this != &L) {
    clear(); // 释放自身节点、保留哨兵,this 成为空列表
    // 与 L 整体交换:this 接管 L 的全部资源,L 获得 this 的空哨兵,仍可继续使用
    std::swap(_size, L._size);
    std::swap(_header, L._header);
    std::swap(_trailer, L._trailer);
  }
  return *this;
}

// 只读接口
template <typename T> void List<T>::show() const {
  for (ListNodePosit<T> i = first(); i != _trailer; i = i->succ) {
    std::cout << i->data << " ";
  }
  std::cout << '\n';
}
template <typename T> const T &List<T>::operator[](Rank r) const {
  ListNodePosit<T> p = first();
  while (r-- > 0) {
    p = p->succ;
  }
  return p->data;
}
template <typename T>
ListNodePosit<T> List<T>::find(T x, Rank n, ListNodePosit<T> end) const {
  while (n-- > 0) {
    end = end->pred;
    if (end->data == x) {
      return end;
    }
  }
  return nullptr;
}
template <typename T>
ListNodePosit<T> List<T>::search(T x, Rank n, ListNodePosit<T> end)
    const { // finding x in sorted range [end-n, end), return the largest node
            // <= x, if not found, return the first node < x
  end = end->pred;
  while (n-- > 0 && x < end->data) {
    end = end->pred;
  }
  return end;
}
template <typename T>
ListNodePosit<T> List<T>::max(ListNodePosit<T> p, Rank n) const {
  ListNodePosit<T> max = p;
  while (n-- > 0) {
    if (p->data > max->data) {
      max = p;
    }
    p = p->succ;
  }
  return max;
}

// 可写接口
template <typename T> ListNodePosit<T> List<T>::insert_as_first(T const x) {
  _size++;
  return _header->insert_as_succ(x);
}
template <typename T> ListNodePosit<T> List<T>::insert_as_last(T const x) {
  _size++;
  return _trailer->insert_as_pred(x);
}
template <typename T>
ListNodePosit<T> List<T>::insert_before(T const x, ListNodePosit<T> p) {
  _size++;
  return p->insert_as_pred(x);
}
template <typename T>
ListNodePosit<T> List<T>::insert_after(T const x, ListNodePosit<T> p) {
  _size++;
  return p->insert_as_succ(x);
}
template <typename T> T List<T>::remove(ListNodePosit<T> p) {
  _size--;
  T d = p->data;
  p->pred->succ = p->succ;
  p->succ->pred = p->pred;
  delete p;
  return d;
}
template <typename T> Rank List<T>::dedumlicate() {
  if (_size < 2) {
    return 0;
  }
  Rank oldsize = _size;
  Rank n = 0;
  for (ListNodePosit<T> i = first(); i != _trailer; i = i->succ) {
    // std::cout << n << '\n';
    ListNodePosit<T> same = find(i->data, n, i);
    if (same != nullptr) {
      remove(same);
    } else {
      n++;
    }
  }
  return oldsize - _size;
}
template <typename T> Rank List<T>::uniquify() {
  if (_size < 2) {
    return 0;
  }
  Rank oldsize = _size;
  for (ListNodePosit<T> i = first()->succ; i != _trailer; i = i->succ) {
    if (i->data == i->pred->data) {
      remove(i->pred);
    }
  }
  return oldsize - _size;
}

// 排序
template <typename T> void List<T>::sort_insert(ListNodePosit<T> p, Rank n) {
  if (n < 2) {
    return;
  }
  for (Rank sorted = 0; sorted < n; sorted++) {
    insert_after(p->data, search(p->data, sorted, p));
    p = p->succ;
    remove(p->pred);
  }
}
template <typename T> void List<T>::sort_select(ListNodePosit<T> p, Rank n) {
  if (n < 2) {
    return;
  }
  ListNodePosit<T> head = p;
  ListNodePosit<T> tail = p;
  for (Rank i = 0; i <= n; ++i) {
    tail = tail->succ;
  }
  // unsorted range: (head, tail), tail-head = n+1

  while (n > 1) {
    ListNodePosit<T> maxp = max(head->succ, n);
    insert_before(maxp->data, tail);
    remove(maxp);

    // update
    tail = tail->pred;
    n--;
  }
}

template <typename T> void List<T>::sort_merge(ListNodePosit<T> &p, Rank n) {
  if (n < 2) {
    return;
  }
  Rank mid = n >> 1;
  ListNodePosit<T> q = p;
  for (int i = 0; i < mid; ++i) {
    q = q->succ;
  }
  sort_merge(p, mid);
  sort_merge(q, n - mid);
  p = merge(p, n, *this, q, n - mid);
}
template <typename T>
ListNodePosit<T> List<T>::merge(ListNodePosit<T> p, Rank n, List<T> &l,
                                ListNodePosit<T> q, Rank m) {
  ListNodePosit<T> pp = p->pred; // 借助前驱,便于返回归并后区间的起点
  while (0 < m) {                // 在 q 尚未移出区间之前
    if ((0 < n) && (p->data <= q->data)) { // 若 p 仍在区间内且 v(p) <= v(q)
      if (q == (p = p->succ)) {           // p 归入合并列表,并替换为其直接后继
        break; // 若 p 已越过 q(自归并场景),退出
      }
      n--;
    } else { // 若 p 已超出右界或 v(q) < v(p)
      ListNodePosit<T> old = q;
      q = q->succ;
      insert_before(l.remove(old), p); // 将 q 的原节点整体转移至 p 之前
      m--;
    }
  }
  return pp->succ; // 确定归并后区间的(新)起点
}