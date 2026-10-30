#ifndef LIST_H
#define LIST_H

#include "ListNode.h"
using Rank = unsigned int;

template <typename T> class List {
  Rank _size{};
  ListNodePosit<T> _header;  // head pointer
  ListNodePosit<T> _trailer; // tail pointer

protected:
  void init();
  Rank clear();
  void copy_nodes(ListNodePosit<T> p, Rank n); // 复制p起n项，[p, p+n-1]

public:
  // 构造函数
  List() { init(); }
  List(List<T> const &L) {
    init();
    copy_nodes(L.first(), L._size);
  } // 是L.first()，而不是first()
  List(List<T> const &L, Rank r, Rank n);
  List(ListNodePosit<T> r, Rank n) {
    init();
    copy_nodes(r, n);
  }
  List(List &&L) noexcept;
  List &operator=(List<T> const &L);
  List &operator=(List &&L) noexcept;

  // 析构函数
  ~List() {
    clear();
    delete _header;
    delete _trailer;
  }

  // 只读接口
  void show() const;
  [[nodiscard]] Rank size() const { return _size; }
  [[nodiscard]] bool empty() const { return _size <= 0; }
  const T &operator[](Rank r) const;
  ListNodePosit<T> first() const { return _header->succ; }
  ListNodePosit<T> last() const { return _trailer->pred; }
  bool valid(ListNodePosit<T> p) const {
    return p && (p != _header) && (p != _trailer);
  }
  ListNodePosit<T>
  find(T x, Rank n,
       ListNodePosit<T> end) const; // finding x in unsorted range [end-n, end)
  ListNodePosit<T> find(T const x) const { return find(x, _size, _trailer); }
  ListNodePosit<T>
  search(T x, Rank n,
         ListNodePosit<T> end) const; // finding x in sorted range [end-n, end)
  ListNodePosit<T> search(T const x) const {
    return search(x, _size, _trailer);
  }
  ListNodePosit<T> max(ListNodePosit<T> p, Rank n) const;

  // 可写接口
  ListNodePosit<T> insert_as_first(T x);
  ListNodePosit<T> insert_as_last(T x);
  ListNodePosit<T> insert_before(T x, ListNodePosit<T> p);
  ListNodePosit<T> insert_after(T x, ListNodePosit<T> p);
  T remove(ListNodePosit<T> p);
  Rank dedumlicate();
  Rank uniquify();

  // 排序
  void sort_insert(ListNodePosit<T> p,
                   Rank n); // 对p起n项进行插入排序，[p, p+n-1]
  void sort_insert() { sort_insert(first(), _size); }
  void sort_select(ListNodePosit<T> p,
                   Rank n); // 对p起n项进行插入排序，(p, p+n]
  void sort_select() { sort_select(_header, _size); }
  ListNodePosit<T>  merge(ListNodePosit<T> p, Rank n, List<T> &l, ListNodePosit<T> q,
             Rank m);                           // 将l中q起m项归并到[p, p+n)中
  void sort_merge(ListNodePosit<T> &p, Rank n); //[p, p+n)
};

#include "List_implementation.tpp"

#endif // LIST_H