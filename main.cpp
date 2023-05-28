#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace std;

class Array {
    public:
    using value_type=int;
    private:
    value_type *m_data;
    size_t m_capacity;
    size_t m_end;
    public:
    /*métodos especiais*/
    Array(size_t capacity=1){
        m_capacity=capacity;
        m_data=new value_type[m_capacity];
        m_end=0;
        for(size_t i{0};i<m_capacity;++i)
            m_data[i]=value_type();
    }
    ~Array(){
        delete[] m_data;
    }
    Array& operator=(const Array& other) {
        /*apagar a memória antiga apenas se os tamanhos divergirem*/
        if(this->m_capacity!=other.m_capacity) {
            /*liberar memória original*/
            delete[] m_data;
            /*assumir o tamanho da nova memória*/
            m_capacity=other.m_capacity;
            m_end=other.m_end;
            /*alocar a nova memória*/
            m_data=new value_type[m_capacity];
        }
        /*copiar os elementos de other para this*/
        copy(other.m_data, other.m_data+other.m_end, m_data);
        return *this;
    }
    /*acessando*/
    size_t size() const { return m_end; }
    size_t capacity() const { return m_capacity; }
    bool empty() const { return m_end==0; }
    bool full() const { return m_end==m_capacity; }
    const value_type& at(size_t pos) const;
    /*modificando*/
    void push_back(const value_type& value);
    void pop_back();
    void clear() { m_end=0; }
    void remove(const size_t& pos);
    void insert(const size_t& pos, const value_type& value);
    value_type& at(size_t pos);
    /*friends*/
    friend string to_string(const Array& v);
};

const Array::value_type& Array::at(size_t pos) const {
    if(pos>=m_end) 
        throw out_of_range("Acesso fora dos limites do vetor!");
    return m_data[pos];
}

void Array::push_back(const value_type& value) {
    if(full()) 
        throw length_error("Array já está cheio!");
    m_data[m_end++]=value;
}

void Array::pop_back() {
    if(empty()) 
        throw length_error("Array está vazio!");
    m_end--;
}

void Array::remove(const size_t& pos) {
    if(pos>=m_end) 
        throw out_of_range("Posição fora dos limites do vetor!");
    for(auto i{pos};i<(m_end-1);++i) 
        m_data[i]=m_data[i+1];
    m_end--;
}

void Array::insert(const size_t& pos, const value_type& value) {
    if(full()) 
        throw length_error("Array já está cheio!");
    else if(pos>=m_capacity) 
        throw out_of_range("Posição fora dos limites do vetor!");
    else if(pos<m_end) {
        for(auto i{m_end};i>(pos);--i) 
            m_data[i]=m_data[i-1];
        m_data[pos]=value;
        m_end++;
    }
    else {
        for(size_t i{m_end};i<pos;++i)
            m_data[i]=value_type();
        m_end=pos+1;
    }
    m_data[pos]=value;
}

Array::value_type& Array::at(size_t pos) {
    if(pos>=m_end)
        throw out_of_range("Acesso fora dos limites do vetor!");
    return m_data[pos];
}

string to_string(const Array& v) {
    ostringstream oss;
    oss<<"[ ";
    for(size_t ct{0}; ct<v.size();++ct) {
        oss<<v.m_data[ct]<<" ";
    }
    oss<<"| ";
    for(size_t ct{v.size()};ct<v.capacity();++ct) {
        oss<<v.m_data[ct]<<" ";
    }
    oss<<"]";
    return oss.str();
}

int main() {
    Array vec(8);
    cout<<"vec: "<<to_string(vec)<<'\n';
    cout<<"size: "<<vec.size()<<'\n';
    cout<<"capacity: "<<vec.capacity()<<'\n';

    for(auto i{0};i<(vec.capacity()-3);++i)
        vec.push_back(i+1);
    cout<<"vec depois: "<<to_string(vec)<<'\n';

    vec.insert(0, 0);
    cout<<"vec depois: "<<to_string(vec)<<'\n';
    vec.remove(0);
    cout<<"vec depois: "<<to_string(vec)<<'\n';
    return 0;
}