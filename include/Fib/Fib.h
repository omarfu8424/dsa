#pragma once
using num = long long;

class Fib{
    num f, g;
public:
    Fib(int n){f = 1, g = 0; while(g < n) next();} //返回大于等于n的最小斐波那契数
    num get(){return g;};
    num next(){g += f; f = g - f; return g;};
    num prev(){f = g - f; g -= f; return g;};
};