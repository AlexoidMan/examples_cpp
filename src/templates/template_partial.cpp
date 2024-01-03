#include <iostream>

template <typename N, typename D>
class Ratio
{
public:
   Ratio(): num_(), denom_() {}
   Ratio(const N& num, const D& denom):
      num_(num), denom_(denom)
   { }
   explicit operator double() const
   { return double(num_)/double(denom_);  }

private:
 N num_;
 D denom_;

};

//c++17 style
template <typename N, typename D>
Ratio<N,D> make_ratio(const N& num, const D& denom)
{ 
    return {num, denom};
}

//in the header ----------------------
template <typename T> class A
{
public:
    static T n;
};

template <typename T> T A<T>::n {};
//-----------------------------------------

// in the header C++17 ----
template <typename T> class B
{
public:
    static inline T n {};
};
//--------------------
//-------- Explicit Specialization ------------
template < >
class Ratio<double, double>
{
public:
   Ratio(): value_(0.0) {}

   template <typename N, typename D>
     Ratio(const N& num, const D& denom):
      value_( double(num)/ double(denom)) { std::cout << "Ratio<double, double> Explicit Specialization constructor" << std::endl; }

   explicit operator double() const { return value_; }

private:
 double value_;
 
};

//-------- Partial Specialization ------------
template < typename N, typename D >
class Ratio< N*, D*>
{
public:

   Ratio(N* num, D* denom): num_(num), denum_(denom) 
       { std::cout << "Ratio< N*, D*> Partial Specialization constructor" << std::endl; }

   explicit operator double() const
    { return double(*num_)/ double(*denum_); }

private:
  N* const num_;
  D* const denum_;
 
};


int main(int argc, char** argv)
{
    std::cout << "Common templates :" << std::endl;

    Ratio<int, double> r{5, 2.3f};

    auto r17(make_ratio(4, 2.2));
    A<int> a;
    B<double> b;

    std::cout << "A<int>::n   " << a.n << std::endl;
    std::cout << "B<double>::n   " << b.n << std::endl;

    std::cout << "Partial templates :" << std::endl;
    Ratio<double, double> r_explicit{5.5f, 2.3f};
    
    int i =5; double x = 10;
    auto r_partial(make_ratio(&i, &x));     //Ratio<int*, double*>
    //double(r_partial);
    std::cout << "double(r_partial) :"<< double(r_partial) << std::endl;
    x = 2.5;
    // double(r_partial);
    // std::cout << "double(r_partial) :"<< double(r_partial) << std::endl;

    return 0;
}