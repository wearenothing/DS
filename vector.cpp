#include <algorithm>
#include <iostream>
using namespace std;
typedef int Rank;
#define DEFAULT_CAPACITY 3
template <typename T>
class vector
{
private:
    Rank _size;
    int _capacity;
    T *_elem;

protected:
public:
    void copyFrom(const T *A, Rank lo, Rank hi); //[lo,hi)
    void expand();
    vector(int c = DEFAULT_CAPACITY)
    {
        _elem = new T[_capacity = c];
        _size = 0;
    }
    vector(const T *A, Rank lo, Rank hi)
    {
        copyFrom(A, lo, hi);
    }
    vector(const vector<T> &v, Rank lo, Rank hi)
    {
        copyFrom(v._elem, lo, hi);
    }
    vector(const vector<T> &v)
    {
        copyFrom(v._elem, 0, v._size);
    }
    ~vector() { delete[] _elem; };
    //guarantee:0<=r<_size
    T &operator[](Rank r) { return _elem[r]; }             //既可以做左值，也可以做右值
    const T &operator[](Rank r) const { return _elem[r]; } //只能做右值

    //insert
    Rank insert(Rank r, const T &e); //0<=r<=_size

    //remove [lo,hi)
    int remove(Rank lo, Rank hi);
    T remove(Rank r);
    Rank find(const T&e, Rank lo,Rank hi)const;
    int deduplicate();
    template<typename VST> void traverse(VST& visit);

    //有序向量
    int disordered()const;
    int uniquify();
    void bubbleSort(Rank lo,Rank hi);
};
template <typename T>
void vector<T>::copyFrom(const T *A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while (lo < hi)
        _elem[_size++] = A[lo++];
}
template <typename T>
void vector<T>::expand()
{
    if (_size < _capacity)
        return;
    _capacity = max(_capacity, DEFAULT_CAPACITY);
    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; i++)
    {
        _elem[i] = oldElem[i];
    }
    delete[] oldElem;
}
template <typename T>
Rank vector<T>::insert(Rank r, const T &e)
{
    expand();
    for (Rank i = _size; i > r; i--)
    {
        _elem[i] = _elem[i - 1];
    }
    _elem[r] = e;
    _size++;
    return r;
}
template <typename T>
int vector<T>::remove(Rank lo, Rank hi)
{
    if (lo == hi)
        return 0; //处理退化情况
    while (hi < _size)
        _elem[lo++] = _elem[hi++];
    _size = lo;
    return hi - lo;
}
template<typename T>
T vector<T>::remove(Rank r)
{
    T e=_elem[r];
    remove(r,r+1);
    return e;
}
template <typename T>
Rank vector<T>::find(const T&e,Rank lo ,Rank hi) const
{
    while ((lo<hi--&&e!=_elem[hi]));
    return hi;//if hi<lo, means that fail to find 
}
template <typename T>
int vector<T>::deduplicate()
{
    int oldsize=_size;
    Rank i=1;
    while (i<_size)
    {
        if(find(_elem[i],0,i)<0)
            i++;
        else
            remove(i);
    }
    return oldsize-_size;
}

template <typename T> template <typename VST>
void vector<T>::traverse(VST& visit)
{
    for (int i = 0; i < _size; i++)
    {
        visit(_elem[i]);
    }
}
template <typename T>
int vector<T>::disordered()const
{
    int count=0;
    for (int i = 1; i < _size; i++)
        count+=(_elem[i-1]>_elem[i]);//漂亮！！！
    return count;
}

template <typename T>
int vector<T>::uniquify()
{
    int i=0;int j=0;
    while (++j<_size)
        if(_elem[i]!=_elem[j])
            _elem[++i]=_elem[j];
    _size=++i;
    return j-i;
    
}

template <typename T>
void vector<T>::bubbleSort(Rank lo,Rank hi)
{
    while (lo<--hi)
        for(int i=lo;i<hi;i++)
            if(_elem[i]>_elem[i+1])
                swap(_elem[i],_elem[i+1]);
}
template <typename T>
struct Increase
{
    virtual void operator()(T& e){e++;};
};
//将traverse和Increase进行包装
template <typename T> void increase(vector<T> &v)
{
    Increase<T> t;
    v.traverse(t);
}

template <typename T>//在有序向量区间[lo,hi)中查找元素e
static Rank binSearch(T* S, const T& e, Rank lo, Rank hi )
{
    while (lo<hi)   
    {
       Rank mi=(lo+hi)>>1;
       if(e<S[mi]) hi=mi;
       else if(S[mi]<e) lo=mi+1;
       else 
       return mi; 
    }
    return -1;
}
int main()
{
    int a[]={1,3,4};
    vector<int> v(a,0,3);
    //v.traverse(Increase<int>());
    increase(v);
    return 0;
}