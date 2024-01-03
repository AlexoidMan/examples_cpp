#include <iostream>
#include <vector>

template <typename T, template<typename> typename Container>
class Builder
{
   Container<T> data_;
public:
    void add(const T& t) {data_.push_back(t);}
    void print() const
    {
        for(const auto& x : data_) std::cout << x << " ";
        std::cout << std::endl;
    }

};

template <typename T>  class my_vector
{
public:
    my_vector() = default;
    ~my_vector(){} 

    //using standart container with iterator
    using container=std::vector<T>;
    using iterator=typename container::iterator;
    using const_iterator=typename container::const_iterator;

    iterator begin() { return vec.begin(); }
    iterator end() { return vec.end(); }
    const_iterator begin() const { return vec.begin(); }
    const_iterator end() const { return vec.end(); }

    void push_back(const T &t)
    {
        vec.push_back(t);
    }

private:
  container vec;

};


int main(int argc, char** argv)
{
    std::cout << "Template template parameters :" << std::endl;

   Builder<int, my_vector> b;
   // Builder<int, std::vector> b;
    b.add(1);
    b.add(2);

    b.print();    

    return 0;
}