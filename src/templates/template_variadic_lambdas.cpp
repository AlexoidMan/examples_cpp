#include <iostream>
#include <tuple>
#include <memory>

//---------------------------------------------
template <typename T1>
auto sum(const T1& x1)
{
    return x1;
};

template <typename T1, typename ... T>
auto sum(const T1& x1, const T& ... x)
{
    //return x1 + sum(x ...);
    return sum(x ...);

}
//---------------------------------------------

template <typename ... T> struct Group;
template <typename T1> struct Group<T1>
{
    T1 t1_;
    Group() = default;
    explicit Group(const T1& t1) : t1_(t1) {}
    explicit Group(T1&& t1) : t1_(std::move(t1)) {}
    explicit operator const T1&() const {return t1_;}
    explicit operator T1&() { return t1_; }
};

template <typename T1, typename ... T>
struct Group<T1, T ...> : Group<T ...>
{
    T1 t1_;
    Group() = default;
    explicit Group(const T1& t1, T&& ... t) : Group<T ...> (std::forward<T>(t) ...), t1_(t1) {}
    explicit Group(T1&& t1, T&& ... t) :  Group<T ...> (std::forward<T>(t) ...), t1_(std::move(t1)) {}
    explicit operator const T1&() const {return t1_;}
    explicit operator T1&() { return t1_; }
};

template <typename ... T>  Group(T&&... t) -> Group<T...>; // for automatic deducing types without defition of them

//-------- Lambdas -------------------------------------
//1) Old-school solution with recursion
/* 
template <typename ... F> struct overload_set;
template <typename F1>
struct overload_set<F1> : public F1
{
    overload_set(const F1& f1) : F1(f1) {}
    overload_set(F1&& f1) : F1(std::move(f1)) {}
    using F1::operator();
};

template <typename F1, typename ... F>
struct overload_set<F1, F ...> : public F1, public overload_set<F ...>
{
    overload_set(F1&& f1, F&& ... f) : F1(std::move(f1)),  overload_set<F ...> (std::forward<F>(f) ...) {}
    overload_set(const F1& f1, F&& ... f) : F1(f1), overload_set<F ...> (std::forward<F>(f) ...)  {}

    using F1::operator();
    using overload_set<F ...>::operator();
};

//assistant function
template <typename ... F> auto overload(F&& ... f)
{
    return overload_set<F ...>(std::forward<F>(f) ...);
}
*/

// 2) C++17 solution using parameter pack
template <typename ... F>
struct overload_set : public F ...
{
    overload_set(F&& ... f) : F(std::forward<F>(f)) ... {}
    using F::operator() ...; 
};

template <typename ... F> auto overload(F&& ... f)
{
    return overload_set<F ...>(std::forward<F>(f) ...);
}
//----------------------------------------------------------
// Concept example
template <typename T> concept HasXOR =
  requires(T a, T b) { a ^ b; };

template <HasXOR T> void MySwap(T& x, T& y)
{
    std::cout << "MySwap with XOR" << std::endl;
    x = x ^ y;
    y = x ^ y;
    x = x ^ y;
}

//another concept declaration
template <typename T> concept Assignable =
  requires(T a, T b) {
    T(std::move(b));
    b = std::move(a);
 };

template <Assignable T>
void MySwap(T& x, T& y)
  requires(!HasXOR<T>)
{
    std::cout << "MySwap on MOVE" << std::endl;
    T tmp(std::move(x));
    x = std::move(y);
    y = std::move(tmp);
}

int main(int argc, char** argv)
{
    std::cout << "Variadic template :" << std::endl;
  
    std::cout << " sum(2, 4, 5, 6, 8, 8) :" << sum(2, 4, 5, 6, 8, 8)  << std::endl;

    Group<int, long> g(3, 5);
    std::cout << "  int(g) :" <<  int(g)  << std::endl;  // called operator for int 
    std::cout << "  long(g) :" <<  long(g)  << std::endl;

    //C++17:

    Group g_17(3, 2.2, std::string("xyz"));
    std::cout << "int(g)= " <<  int(g_17) << "  double(g)= " <<  double(g_17) << "  string(g)= " <<  std::string(g_17) << std::endl;

    //lambdas
    int i = 5;
    double d = 7.3;
    auto l = overload (
        [](int* i) { std::cout << "i=" << *i << std::endl;},
        [](double* d) { std::cout << "d=" << *d << std::endl;}
    );

    l(&i);
    l(&d);

    // concept
    int X1 = 10, Y1 = 20;
    MySwap(X1, Y1);
    std::cout << " MySwap(X1, Y1)" <<  " X1= " <<  X1 << "  Y1= " <<  Y1 << std::endl;

    return 0;
}