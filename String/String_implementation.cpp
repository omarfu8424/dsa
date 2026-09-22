#include "String.h"
#include <iostream>
using namespace std;

//复制[lo, hi)区间的内容到当前对象
void String::copy(const char* o, Rank lo, Rank hi) {
    _str = new char[_capa = (hi - lo)<<1];
    _size = 0;
    while(lo < hi)
        _str[_size++] = o[lo++];
}

void String::expand(){
    if(_size < _capa)   return;
    if(_capa < DEFAULT_CAPA)
        _capa = DEFAULT_CAPA;
    const char* old = _str;
    copy(old, 0, _size);
    delete []old;
}

void String::shrink(){
    if(_capa < DEFAULT_CAPA >> 1)   return;
    if(_size > _capa >> 2)  return;
    const char* old = _str;
    _str = new char[_capa >>= 1];
    for(Rank i = 0; i < _size; ++i)
        _str[i] = old[i];
    delete []old;
}


//比较器
bool String::operator==(const String& o) const{
    if(_size != o._size)  return false;
    for(Rank i = 0; i < _size; ++i)
        if(_str[i] != o._str[i])  return false;
    return true;
}

bool String::operator<(const String& o) const{
    for(Rank i = 0; i < _size && i < o._size; ++i)
        if(_str[i] < o._str[i])  return true;
        else if(_str[i] > o._str[i])  return false;
    return _size < o._size;
}

//只读访问接口
void String::print() const{
    for(Rank i = 0; i < _size; ++i)
        cout << _str[i];
    cout << endl;
}

//只写访问接口
Rank String::remove(Rank lo, Rank hi){  //[lo, hi)
    if(lo == hi)    return 0;
    while(hi < _size)
        _str[lo++] = _str[hi++];
    _size = lo;
    shrink();
    return hi - lo;
}

Rank String::insert(Rank r, const char e){
    expand();
    Rank in = r; 
    for(Rank i = _size; i > r; i--)
        _str[i] = _str[i-1];
    _str[in] = e;
    _size++;

    return in;
}