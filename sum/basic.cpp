#include<iostream>
#include<vector>

using Rank = unsigned int ;
using namespace std;

template<typename T>
T sum_bio(vector<T>& arr, Rank lo, Rank hi, int depth = 0){
    //cout << depth << " ( " << lo << " " << hi << " )" << endl;
    if(lo == hi){
        return arr[lo];
    }
    else{
        int mid = (lo + hi) >> 1;
        return sum_bio(arr, lo, mid, depth+1) + sum_bio(arr, mid+1, hi, depth+1);
    }
}

template<typename T>
T sum_linear(vector<T>& arr, Rank n, int depth = 0){
    //cout << depth << " ( " << n << " )" << endl;
    if(n < 1)
        return 0;
    else{
        return sum_linear(arr, n-1, depth+1) + arr[n-1]; 
    }
}

int main(){
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << sum_bio(arr, 0, arr.size()-1) << endl;
    cout << sum_linear(arr, arr.size()) << endl;
}