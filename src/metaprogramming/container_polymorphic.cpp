#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <variant>
#include <memory>

using namespace std;

//ordinary abstract class as the base class
//vector<variant<int, string, float>> MultipleTypeArray; 
 
// Simplified: 
using MultiType = variant<int, string, float>; 
vector<MultiType> MultipleTypeArray; 
 

// pure virtual class in the container
 namespace {
  struct base {};

  template<class T>
    struct base_t : public base {
      virtual ~base_t(){}
      virtual T t() = 0;
    };

  template<class T>
    struct A : public base_t<T> {
      A(){}
      A(T const& t_): _t(t_) {}
      ~A(){}
      T _t{};
      virtual T t() override { std::cout << _t << '\n'; return _t; }
    };
}


int main(int argc, char** argv)
{
    std::cout << "container of polymorphic objects example:" << std::endl;

    std::cout << "std::variant example:" << std::endl;
    MultipleTypeArray.push_back(120); // int 
    MultipleTypeArray.push_back("Example"); // string 
    MultipleTypeArray.push_back(3.14f); // float 

    std::cout << "size of MultipleTypeArray: "<< MultipleTypeArray.size() << std::endl;
    std::cout << "index 0 MultipleTypeArray: "<< std::get<int>(MultipleTypeArray.at(0)) << std::endl;
    //std::cout << "index 0 MultipleTypeArray: "<< std::get<int>(MultipleTypeArray.at(1)) << std::endl;  Exception: MultipleTypeArray.at(1)  is a string
    std::cout << "index 0 MultipleTypeArray: "<< std::get<string>(MultipleTypeArray.at(1)) << std::endl;
    std::cout << "index 0 MultipleTypeArray: "<< std::get<float>(MultipleTypeArray.at(2)) << std::endl;
    

    std::cout << "shared ptr example:" << std::endl;
    std::vector<std::shared_ptr<base_t<int>>> vec;

    vec.push_back(std::make_shared<A<int>>(1));
    // vec.push_back(std::make_shared<A<double>>(2.22f));
    //vec.push_back(std::make_shared<A<std::string>>("Hello!"));


    std::cout << std::endl;
    return 0;
}