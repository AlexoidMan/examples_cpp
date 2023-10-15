#include <iostream>


namespace {
  struct base_t { virtual ~base_t(){} };

  template<class T>
    struct Base : public base_t {
      virtual T t() = 0;
    };

  template<class T>
    struct A : public Base<T> {
      ~A(){}
      virtual T t() override { std::cout << "A" << '\n'; return T{}; }

      int foo(){std::cout << "A.foo() is called" << '\n';  return 1;}
    };
}

using foo_t = decltype(std::declval<A<int>>().foo());

int main(int argc, char** argv)
{
    /* int var;
    const int&& fx();
    struct C { double x; };
    const C* c = new C();

    decltype(fx());  //Type:  const int&&
    decltype(var);  //Type:  int
    decltype(c->x);  //Type:  double
    decltype((c->x)); //Type:  const double&   
    */

    std::cout << "Std::declval example:" << std::endl;

    decltype(std::declval<A<int>>().t()) a{}; // = int a;
    decltype(std::declval<Base<int>>().t()) b{}; // = int b;    //Base - is a Pure virtual class
    std::cout << a << ',' << b << '\n';

    //   foo_t type1;

    std::cout << std::endl;
    return 0;
}