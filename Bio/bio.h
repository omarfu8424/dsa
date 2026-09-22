#pragma once
#include <iostream>
#include <vector>

using namespace std;

class bio{
    vector<int> arr;
    void normalize(){
        while(arr.size() > 1 && arr[0] == 0)
            arr.erase(arr.begin());
    }

public:
    bio(const string& str = ""){
        for(char c : str){
            if(c == '1')
                arr.push_back(1);
            else if(c == '0')
                arr.push_back(0);
        }
    };


    friend bio operator+(const bio& a, const bio& b){
        bio result;
        int i = a.arr.size() - 1;
        int j = b.arr.size() - 1;
        int carry = 0;
        for(;i >= 0 || j >= 0 || carry; i--, j--){
            int sum = carry;
            if(i >= 0) sum += a.arr[i];
            if(j >= 0) sum += b.arr[j];
            result.arr.insert(result.arr.begin(), sum % 2);
            carry = sum / 2;
        }
        return result;
    }

    friend bio operator-(const bio& a, const bio& b){
        bio result;
        int i = (int)a.arr.size() - 1;
        int j = (int)b.arr.size() - 1;
        int borrow = 0;
        for(;i >= 0 || j >= 0; i--, j--){
            int diff = (i >= 0 ? a.arr[i] : 0) - (j >= 0 ? b.arr[j] : 0) - borrow;
            if(diff < 0){
                diff += 2;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.arr.insert(result.arr.begin(), diff);
        }
        result.normalize();
        return result;
    }
    
    friend bio operator&(const bio& a, const bio& b){
        bio result;
        int i = a.arr.size() - 1;
        int j = b.arr.size() - 1;
        for(;i >= 0 && j >= 0; i--, j--){
            result.arr.insert(result.arr.begin(), a.arr[i] & b.arr[j]);
        }

        result.normalize();
        return result;
    }
    
    int count_1() const{
        bio num = *this;
        int count = 0;
        while(num.arr.size() > 0){
            if(num.arr.size() == 1 && num.arr[0] == 0)
                break;
            count++;
             num = (num - bio("1")) & num;
        }
        return count;
    }   
      
    
    void print() const{
        for(int bit : arr)
            cout << bit;
        cout << endl;
    }
};

int main(){
    bio a("11011");
    bio b("1011");
    bio c = a + b;
    c.print();
    cout << "Number of 1s in c: " << c.count_1() << endl;
    return 0;
}