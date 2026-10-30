#ifndef STRING_H
#define STRING_H

#define DEFAULT_CAPA 3
#include <cstring>
using Rank = unsigned int;

class String{
    char* _str;
    Rank _size; //规模
    Rank _capa; //容量
protected:
    void copy(const char* o, Rank lo, Rank hi); //并未释放原_str，需调用者释放
    void expand();
    void shrink();
public:
    //构造函数
    String(Rank c = DEFAULT_CAPA){
        _str = new char[_capa = c > DEFAULT_CAPA ? c : DEFAULT_CAPA];
        _size = 0;
    }
    String(Rank c, Rank s, char e){
        _str = new char[_capa = c > DEFAULT_CAPA ? c : DEFAULT_CAPA];
        for(_size = 0; _size < s; _str[_size++] = e);
    }
    String(const String& o){copy(o._str, 0, o._size);}
    String(const String& o, Rank lo, Rank hi){copy(o._str, lo, hi);}
    String(const char* s){copy(s, 0, strlen(s));}
    String& operator=(const String& o){
        if(this != &o){
            delete []_str;
            copy(o._str, 0, o._size);
        }
        return *this;
    }

    //析构函数
    ~String(){delete []_str;}

    //元素访问接口
    char& operator[](Rank r){return _str[r];} 
    const char& operator[](Rank r) const{return _str[r];}

    //比较器
    bool operator==(const String& o) const;
    bool operator!=(const String& o) const{return !(*this == o);}
    bool operator<(const String& o) const;
    bool operator>(const String& o) const{return o < *this;}

    //只读访问接口
    Rank size() const{return _size;}
    bool empty() const{return !_size;}
    void print() const;

    //只写访问接口
    Rank remove(Rank lo, Rank hi);  //返回删除元素数量
    Rank remove(Rank r){return remove(r, r+1);};
    Rank insert(Rank r, const char e);
    Rank insert(const char e){return insert(_size, e);}
    int deduplicate();  //无序去重
    int uniquify(); //有序去重

};

#endif // STRING_H
