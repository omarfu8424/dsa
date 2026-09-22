#include <iostream>
#include <vector>
#include <limits>

using namespace std;
using Rank = unsigned int;
using ll = long long;

void print(vector<int>& arr){
    for(int i : arr)
        cout << i << " ";
    cout << endl;
}

void sort_bubble_ori(vector<int>& arr, Rank lo, Rank hi){   //[lo, hi)
    while(hi > lo){
        for(Rank i = lo; i+1 < hi; ++i){
            if(arr[i] > arr[i+1])
                swap(arr[i], arr[i+1]);                
        }
        hi--;  //每次循环后，最大值已经排到最后，长度每次仅缩小一个单位，循环到后期尾部有序长度增加，但仍被扫描，可优化
    }    
}

Rank bubble_scan_l2r(vector<int>& arr, Rank lo, Rank hi){
    Rank last = lo;  //记录最后一次交换的位置
    for(Rank i = lo; i+1 < hi; ++i){
        if(arr[i] > arr[i+1]){
            swap(arr[i], arr[i+1]);
            last = i + 1;  //更新最后一次交换的位置，即arr[i]此时的位置
        }
    }
    cout << "left to right: ";
    print(arr);
    return last;  //更新右边界，下一轮循环只需扫描到最后一次交换的位置
}
void sort_bubble_tail(vector<int>& arr, Rank lo, Rank hi){  //[lo, hi)
    while((hi = bubble_scan_l2r(arr, lo, hi)) > lo);
}

Rank bubble_scan_r2l(vector<int>& arr, Rank lo, Rank hi){
    Rank last = hi;
    for(Rank i = hi - 1; i > lo; --i){
        if(arr[i-1] > arr[i]){
            swap(arr[i-1], arr[i]);
            last = i - 1;   //更新最后一次交换的位置，即arr[i-1]此时的位置
        }
    }
    cout << "right to left: ";
    print(arr);
    return last;  //更新左边界，下一轮循环只需扫描到最后一次交换的位置
}
void sort_bubble_head(vector<int>& arr, Rank lo, Rank hi){  //[lo, hi)
    while(hi > (lo = bubble_scan_r2l(arr, lo, hi)));
}

void sort_bubble_opt(vector<int>& arr, Rank lo, Rank hi){   //cocktail

    while(hi > lo){
        bool sorted = true;
        Rank new_hi = lo;
        for(Rank i = lo; i+1 < hi; ++i){
            if(arr[i] > arr[i+1]){
                sorted = false;
                swap(arr[i], arr[i+1]);
                new_hi = i + 1;
            }
        }
        hi = new_hi;    //更新右边界

        if(sorted)  break;
        cout << "left to right: ";
        print(arr);

        sorted = true;
        Rank new_lo = hi;
        for(Rank i = hi - 1; i > lo; --i){
            if(arr[i-1] > arr[i]){
                sorted = false;
                swap(arr[i-1], arr[i]);
                new_lo = i - 1;
            }
        }
        lo = new_lo;  //更新左边界

        if(sorted)  break;
        cout << "right to left: ";
        print(arr);
    }
}

void merge_opt(vector<int>& arr, Rank lo, Rank mid, Rank hi){   //插入哨兵值，但需要所有元素小于numeric_limits<int>::max()
    vector<int> sub_left(arr.begin()+lo, arr.begin()+mid);    Rank index_left = 0;  //[lo, mid)
    vector<int> sub_right(arr.begin()+mid, arr.begin()+hi);   Rank index_right = 0; //[mid, hi)
    
    //插入哨兵值numeric_limits<int>::max()
    //确保在一个子串空后，总能从另一个字串中取元素，因为另一子串中所有元素小于numeric_limits<int>::max()
    sub_left.insert(sub_left.end(), numeric_limits<int>::max());
    sub_right.insert(sub_right.end(), numeric_limits<int>::max());

    for(Rank i = lo; i < hi; ++i){
        if(sub_left[index_left] < sub_right[index_right])
            arr[i] = sub_left[index_left++];
        else
            arr[i] = sub_right[index_right++];
    }
}
void merge_ori(vector<int>& arr, Rank lo, Rank mid, Rank hi){   //不插入哨兵值
    std::vector<int> sub_left(arr.begin()+lo, arr.begin()+mid);    Rank index_left = 0;  //[lo, mid);
    std::vector<int> sub_right(arr.begin()+mid, arr.begin()+hi);   Rank index_right = 0; //[mid, hi);

    Rank l_size = sub_left.size();
    Rank r_size = sub_right.size();

    for(Rank i = lo; i < hi && (index_left < l_size || index_right < r_size);){
        if((index_left < l_size) && (!(index_right < r_size) || sub_left[index_left] <= sub_right[index_right]))    //当前值的比较必须放在最后（短路），否则可能越界
            arr[i++] = sub_left[index_left++];

        if((index_right < r_size) && (!(index_left < l_size) || sub_left[index_left] > sub_right[index_right]))
            arr[i++] = sub_right[index_right++];
    }
}
void sort_merge(vector<int>& arr, Rank lo, Rank hi){    //[lo, hi)
    if(hi - lo < 2) return;
    Rank mid = (hi + lo) >> 1;
    sort_merge(arr, lo, mid);   sort_merge(arr, mid, hi);
    merge_ori(arr, lo, mid, hi);    
}

void inversePair_merge(vector<int>& arr, ll& res, Rank lo, Rank mid, Rank hi){
    vector<int> sub_left(arr.begin()+lo, arr.begin()+mid);    Rank index_left = 0;  //[lo, mid)
    vector<int> sub_right(arr.begin()+mid, arr.begin()+hi);   Rank index_right = 0; //[mid, hi)
    sub_left.insert(sub_left.end(), numeric_limits<int>::max());
    sub_right.insert(sub_right.end(), numeric_limits<int>::max());

    for(Rank i = lo; i < hi; ++i){
        if(sub_left[index_left] <= sub_right[index_right]){
            arr[i] = sub_left[index_left++];
        }else{
            res += mid - lo - index_left;  //统计逆序对数量
            arr[i] = sub_right[index_right++];
        }
    }
}
void count_inversePair(vector<int>& arr, ll& res, Rank lo, Rank hi){
    if(hi - lo < 2) return;
    Rank mid = (hi + lo) >> 1;
    count_inversePair(arr, res, lo, mid);   count_inversePair(arr, res, mid, hi);
    inversePair_merge(arr, res, lo, mid, hi); 
}

void sort_heap(vector<int>& arr, Rank lo, Rank hi){

}
void sort_quick(vector<int>& arr, Rank lo, Rank hi){

}


int main(){
    cout << "bubble sort with head optimization: " << endl;
    vector<int> arr_bubble_head = {1,2,3,4,5,0};
    sort_bubble_head(arr_bubble_head, 0, arr_bubble_head.size());

    cout << endl << "bubble sort with tail optimization: " << endl;
    vector<int> arr_bubble_tail = {1,2,3,4,5,0};
    sort_bubble_tail(arr_bubble_tail, 0, arr_bubble_tail.size());

    cout << endl << "bubble sort with optimized scan: " << endl;
    vector<int> arr_bubble_opt = {1,2,3,4,5,0};
    sort_bubble_opt(arr_bubble_opt, 0, arr_bubble_opt.size());

    cout << endl << "merge sort: " << endl;
    vector<int> arr_merge = {1,2,3,4,5,0};
    sort_merge(arr_merge, 0, arr_merge.size());
    print(arr_merge);

    // int n; cin >> n;
    // vector<int> arr;
    // for(int i = 0; i < n; ++i){
    //     int x; cin >> x;
    //     arr.push_back(x);
    // }
    // ll res = 0;
    // count_inversePair(arr, res, 0, arr.size());

    // cout << res << endl;
    return 0;
}