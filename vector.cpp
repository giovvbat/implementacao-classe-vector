#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cassert>
#include <math.h>
using namespace std;

namespace sc {    
    template<typename T>
    class vector;
    template<typename T>
    class MyFowardIterator;
    template<typename T>
    string to_string(const vector<T>& v);
    template<typename T>
    ostream& operator<<(ostream& os, const vector<T>& v);
}

template<typename T>
class sc::MyFowardIterator {
    public:
    /*aliases*/
    typedef MyFowardIterator iterator;
    typedef ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef bidirectional_iterator_tag iterator_category;

    private:
    pointer v_ptr;

    public:
    /*construtores*/
    MyForwardIterator(pointer pt=nullptr) { v_ptr=pt; }
    MyForwardIterator(const iterator& other) { v_ptr=other.v_ptr; }
    iterator& operator=(const iterator &other) { v_ptr=other.v_ptr; return *this; }

    /*métodos*/
    reference operator*() const { return *v_ptr; }
    pointer operator->() const { return v_ptr; }
    iterator operator++() { ++v_ptr; return *this; }
    iterator operator++(int) { iterator dummy{*this}; ++v_ptr; return dummy; }
    iterator operator--() { --v_ptr; return *this; }
    iterator operator--(int) { iterator dummy{*this}; --v_ptr; return dummy; }
    iterator& operator+=(difference_type offset) { iterator& it{*this}; it.v_ptr=it.v_ptr+offset; return it; }
    iterator& operator-=(difference_type offset) { iterator& it{*this}; it.v_ptr=it.v_ptr-offset; return it; }
    bool operator==(const iterator& rhs_) const { return rhs_.v_ptr==v_ptr }
    bool operator!=(const iterator& rhs_) const { return rhs_.v_ptr!=v_ptr; }
    difference_type operator-(const iterator& rhs_) const { return v_ptr-rhs_.v_ptr; }

    /*friends*/
    friend bool operator<(const iterator& ita, const iterator& itb) { return ita.v_ptr<itb.v_ptr; }
    friend bool operator>(const iterator& ita, const iterator& itb) { return ita.v_ptr>itb.v_ptr; }
    friend bool operator<=(const iterator& ita, const iterator& itb) { return ita.v_ptr<=itb.v_ptr; }
    friend bool operator>=(const iterator& ita, const iterator& itb) { return ita.v_ptr>=itb.v_ptr; }
    friend iterator operator+(difference_type offset, iterator it) { iterator& dummy; dummy.v_ptr=it.v_ptr+offset; return dummy; }
    friend iterator operator+(iterator it, difference_type offset) { iterator& dummy; dummy.v_ptr=it.v_ptr+offset; return dummy; }
    friend iterator operator-(difference_type offset, iterator it) { iterator& dummy; dummy.v_ptr=it.v_ptr-offset; return dummy; }
    friend std::ostream& operator<<(std::ostream& os_, const MyForwardIterator& p_) { os_<<"[@ "<<p_.v_ptr<<": "<<*p_.v_ptr<<" ]"; return os_; }
};

template<typename T>
class sc::vector {
    /*aliases*/
    using size_type=unsigned long;
    using value_type=T;
    using pointer=value_type*;
    using reference=value_type&;
    using const_reference=const value_type&;

    private:
    size_type v_end;
    size_type v_capacity;
    T *v_data;
    bool full() const { return v_end==v_capacity; }

    public:
    /*construtores*/
    /*vector() { v_capacity=v_end=0; }*/
    explicit vector(size_type count=0);
    template<typename InputItr>
    vector(InputItr first, InputItr last);
    vector(const vector& other);
    vector(initializer_list<T> ilist);
    virtual ~vector() { if(v_data) delete[] v_data; }
    vector& operator=(const vector& other);
    vector& operator=(initializer_list<T> ilist); 

    /*acessos*/
    size_type size() const { return v_end; }
    size_type capacity() const { return v_capacity; }
    bool empty() const { return v_end==0; }
    const_reference back() const { return v_data[v_end-1]; }
    const_reference front() const { return v_data[0]; }
    reference back() { if(v_data) return v_data[v_end-1]; }
    reference front() { if(v_data) return v_data[0]; }
    const_reference operator[](size_type pos) const { return v_data[pos]; }
    reference operator[](size_type pos) { return v_data[pos]; }
    const_reference at(size_type pos) const;
    reference at(size_type pos);
    pointer data() { return v_data; }
    const_reference data() const { return v_data; }

    /*modificadores*/
    void clear() { v_end=0; }
    void push_front(const_reference value);
    void push_back(const_reference value);
    void pop_back();
    void pop_front();
    void assign(size_type count, const_reference value);
    void assign(const std::initializer_list<T>& ilist);
    template <typename InputItr>
    void assign(InputItr first, InputItr last);
    void reserve(size_type new_cap);
    void shrink_to_fit();

    /*friends*/
    friend string to_string<T>(const sc::vector<T>& v);
    friend ostream& operator<<<T>(ostream& os, const vector<T>& v);
};

template<typename T>
sc::vector<T>::vector(size_type count) {
    v_capacity=v_end=count;
    v_data=new T[count];
    for(auto i{0};i<v_end;++i)
        v_data[i]=T();
}

template<typename T>
template<typename InputItr>
sc::vector<T>::vector(InputItr first, InputItr last) {
    size_type count=last-first;
    v_end=v_capacity=count;
    v_data=new T[count];
    for(auto i{0};i<count;++i) {
        v_data[i]=*first;
        first++;
    }
}

template<typename T>
sc::vector<T>::vector(const vector& other) {
    v_capacity=other.v_capacity;
    v_end=other.v_end;
    v_data=new T[other.v_capacity];
    for(size_type i{0};i<other.v_end;++i) 
        v_data[i]=other.v_data[i];
}

template<typename T>
sc::vector<T>::vector(initializer_list<T> ilist) {
    v_capacity=ilist.size();
    v_end=0;
    v_data=new T[v_capacity];
    auto runner=ilist.begin();
    while(runner!=ilist.end()) {
        v_data[v_end]=*runner;
        ++runner;
        ++v_end;
    }
}

template<typename T>
sc::vector<T>& sc::vector<T>::operator=(const vector<T>& other) {
    if(this==&other)
        return *this;
    /*verificando se a capacidade é igual 
    e alocando uma nova caso seja diferente*/
    if(this->v_capacity!=other.v_capacity) {
        delete[] v_data;
        this->v_data=new T[other.v_capacity];
    }
    this->v_capacity=other.v_capacity;
    this->v_end=other.v_end;
    for(auto i{0};i<other.v_end;++i)
        this->v_data[i]=other.v_data[i];
    return *this;
}

template<typename T>
sc::vector<T>& sc::vector<T>::operator=(const initializer_list<T> ilist) {
    v_capacity=ilist.size();
    v_end=0;
    v_data=new T[v_capacity];
    auto runner=ilist.begin();
    while(runner!=ilist.end()) {
        v_data[v_end]=*runner;
        ++runner;
        ++v_end;
    }
    return *this;
}

template<typename T>
const T& sc::vector<T>::at(size_type pos) const {
    if(pos>=v_end)
        throw out_of_range("Posição fora dos limites do vector!");
    return v_data[pos];
}

template<typename T>
T& sc::vector<T>::at(size_type pos) {
    if(pos>=v_end)
        throw out_of_range("Posição fora dos limites do vector!");
    return v_data[pos];
}

template<typename T>
void sc::vector<T>::push_front(const T& value) {
    if(full()) {
        size_type new_capacity=(v_capacity==0)?1:(v_capacity*2);
        T *new_data=new T[new_capacity];
        for(auto i{0};i<v_end;++i)
            new_data[i]=v_data[i];
        delete[] v_data;
        v_data=new_data;
        v_capacity=new_capacity;
    }
    for(auto i{v_end};i>0;--i)
        v_data[i]=v_data[i-1];
    v_data[0]=value;
    ++v_end;
}

template<typename T>
void sc::vector<T>::push_back(const T& value) {
    if(full()) {
        size_type new_capacity=(v_capacity==0)?1:(v_capacity*2);        
        T *new_data=new T[new_capacity];
        for(auto i{0};i<v_end;++i)
            new_data[i]=v_data[i];
        delete[] v_data;
        v_data=new_data;
        v_capacity=new_capacity;
    }
    v_data[v_end]=value;
    ++v_end;
}

template<typename T>
void sc::vector<T>::pop_front() {
    if(empty())
        throw length_error("Vector está vazio!");
    for(auto i{0};i<v_end-1;++i)
        v_data[i]=v_data[i+1];
    v_end--;
}

template<typename T>
void sc::vector<T>::pop_back() {
    if(empty())
        throw length_error("Vector está vazio!");
    v_end--;
}

template<typename T>
void sc::vector<T>::assign(size_type count, const T& value) {
    for(auto i{0};i<count;++i) 
        v_data[i]=value;
    v_end=count;
}

template <typename T>
template<typename InputItr>
void sc::vector<T>::assign(InputItr first, InputItr last) {
    size_type count=last-first;
    v_end=v_capacity=count;
    v_data=new T[count];
    for(auto i{0};i<count;++i) {
        v_data[i]=*first;
        first++;
    }
}

template<typename T>
void sc::vector<T>::reserve(size_type new_cap) {
    if(new_cap<=v_capacity)
        return;
    T *new_data=new T[new_cap];
    for(auto i{0};i<v_end;++i)
        new_data[i]=v_data[i];
    delete[] v_data;
    v_data=new_data;
    v_capacity=new_cap;
}

template<typename T>
void sc::vector<T>::shrink_to_fit() {
    if(v_capacity==v_end)
        return;
    T *new_data=new T[v_end];
    for(auto i{0};i<v_end;++i)
        new_data[i]=v_data[i];
    delete[] v_data;
    v_data=new_data;
    v_capacity=v_end;
}

template<typename T>
bool operator==(const sc::vector<T>& lhs, const sc::vector<T>& rhs) {
    if(lhs.size()!=rhs.size()) 
        return false;
    for(auto i{0};i<lhs.size();++i) {
        if(lhs.at(i)!=rhs.at(i))
            return false;
    }
    return true;
}

template<typename T>
bool operator!=(const sc::vector<T>& lhs, const sc::vector<T>& rhs) {
    if(lhs.size()!=rhs.size()) 
        return true;
    for(auto i{0};i<lhs.size();++i) {
        if(lhs.at(i)!=rhs.at(i))
            return true;
    }
    return false;
}

template<typename T>
ostream& sc::operator<<(ostream& os, const sc::vector<T>& v) {
    os<<"[ ";
    for(auto i{0};i<v.v_capacity;++i) {
      if(i==v.v_end) 
        os<<"| ";
      os<<v.v_data[i]<<" ";
    }
    os<<"]";
    return os;
}

template<typename T>
string sc::to_string(const sc::vector<T>& v) {
    ostringstream oss;
    oss<<"[ ";
    for(size_t ct{0}; ct<v.size();++ct) {
        oss<<v.v_data[ct]<<" ";
    }
    oss<<"| ";
    for(size_t ct{v.size()};ct<v.capacity();++ct) {
        oss<<v.v_data[ct]<<" ";
    }
    oss<<"]";
    return oss.str();
}

int main() {
    sc::vector<int> vetor(10);
    for(auto i{0};i<6;++i)
        vetor[i]=i;

    
    cout<<vetor<<endl;

    return 0;
}
