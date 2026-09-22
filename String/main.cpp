#include<iostream>
#include "String.h"

int main(){
    String s(3, 3, 'b');
    s.insert('a');
    s.print();
    return 0;
}