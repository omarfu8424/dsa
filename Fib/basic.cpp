#include<iostream>
#include<vector>

using Rank = unsigned int ;
using namespace std;

long long fib_bio(long long n){
    if(n < 2)
        return n;
    else
        return fib_bio(n-1) + fib_bio(n-2);
}

long long pre;
long long fib_linear(int n, long long& pre){
    if(n == 0){
        pre = 1;
        return 0;
    }
    else{
        long long prepre;
        pre = fib_linear(n-1, prepre);
        return prepre + pre;
    }
}

vector<long long> memo(100, -1);
long long fib_memo(int n){
    if(memo[n] != -1)
        return memo[n];
    if(n < 2)
        return n;
    else{
        memo[n] = fib_memo(n-1) + fib_memo(n-2);
        return memo[n];
    }
    
}

long long fib_dp(int n){
    vector<long long> fib;
    fib.push_back(1);
    fib.push_back(1);
    for(int i = 2; i < n; i++){
        fib.push_back(fib[i-1] + fib[i-2]);
    }
    return fib[n-1];
}

int main(){
    int n;
    cin >> n;
    cout << fib_bio(n) << endl;
    cout << fib_linear(n, pre) << endl;
    cout << fib_memo(n) << endl;
    cout << fib_dp(n) << endl;
}