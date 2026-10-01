#include <cctype>
#include <iostream>
#include <stack>
#include <unordered_map>

using namespace std;
namespace  {
/*
isp: in-stack priority
icp: incoming priority

icp > isp  push
icp < isp  pop and caculate
*/
struct Prio {
    int isp; 
    int icp; 
};

const unordered_map<char, Prio> PRIO = {
    {'+', {3, 4}}, {'-', {3, 4}},
    {'*', {5, 6}}, {'/', {5, 6}},
    {'^', {7, 8}}, {'!', {8, 9}},
    {'(', {1, 9}}, {')', {9, 2}},
    {'#', {0, 0}}
};

// get valid number from a expression
void readNumber(char*& p, stack<double> &o){
    double num = 0.0;
    while(isdigit(static_cast<unsigned char>(*p)) == 0){
        p++;
    }
    while(isdigit(static_cast<unsigned char>(*p)) != 0){
        num = (num*10) + (*p-'0');
        p++;
    }
    if(*p == '.'){
        p++;
        double fraction = 1.0;
        while(isdigit(static_cast<unsigned char>(*p)) != 0){
            num += (*p-'0')*(fraction /= 10);
            p++;
        }
    }
    o.push(num);
}
float evaluate(char*& p, stack<char> &rpn);

}

int main(){
    // test readNumber
    char a[] = "(1.1+@!#2.2)*3.3";
    char* p = a;
    stack<double> o;
    readNumber(p, o);
    readNumber(p, o);
    readNumber(p, o);
    while(!o.empty()){
        cout << o.top() << '\n';
        o.pop();
    }
    return 0;
}