#include <iostream>
#include <array>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

class Integers
{
public:
  struct Iterator 
  {
      /* iterator tags*/
      using iterator_category = std::forward_iterator_tag;
      using difference_type   = std::ptrdiff_t;
      using value_type        = int;
      using pointer           = int*;  // or also value_type*
      using reference         = int&;  // or also value_type&

      /* iterator constructor: add only one, other added by compiler*/
      Iterator(pointer ptr) : m_ptr(ptr) {}

      /* implement Operators: */
      reference operator*() const { return *m_ptr; }
      pointer operator->() { return m_ptr; }

      // Prefix increment
      Iterator& operator++() { m_ptr++; return *this; }  

      // Postfix increment
      Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

      friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
      friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };    

private:

    pointer m_ptr;
  };


    Iterator begin() { return Iterator(&m_data[0]); }
    Iterator end()   { return Iterator(&m_data[200]); } // 200 is out of bounds

private:
    int m_data[200];

};

//wrapper around std::array
class Integers2
{
  public:
    using IntegersType = std::array<int, 32>;

    IntegersType::iterator begin() { return m_data.begin(); }
    IntegersType::iterator end()   { return m_data.end(); }    

private:
    IntegersType m_data;
};

int main(int argc, char** argv)
{

    std::cout << "Iterator 1 example :" << std::endl;


    Integers integers;

    std::fill(integers.begin(), integers.end(), 3);   //for ex.:  std::reverse requires a Bidirectional Iterator.

    for (auto i : integers)
        std::cout << i << "\n";


    for (auto it = integers.begin(), end = integers.end(); it != end; ++it) { 
        const auto i = *it; 
        std::cout << i << "\n";
    }

    //wrapper iterator class
    Integers2 integers2;

    for (auto i : integers2)
        std::cout << i << "\n";

    return 0;
}