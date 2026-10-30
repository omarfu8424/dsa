template<typename T> struct ListNode;
template<typename T> using ListNodePosit = ListNode<T>*;

template<typename T>
struct ListNode{
    T data;
    ListNodePosit<T> pred;
    ListNodePosit<T> succ;

    ListNode():pred(nullptr), succ(nullptr){}
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    ListNode(T e, ListNodePosit<T> p = nullptr, ListNodePosit<T> s = nullptr)
            : data(e), pred(p), succ(s){}
    
    ListNodePosit<T> insert_as_pred(const T& e){
        auto insert = new ListNode(e, pred, this);
        if(pred) {pred->succ = insert;}
        pred = insert;
        return insert;
    }
    ListNodePosit<T> insert_as_succ(const T& e){
        auto insert = new ListNode(e, this, succ);
        succ->pred = insert;
        succ = insert;
        return insert;
    }
};
