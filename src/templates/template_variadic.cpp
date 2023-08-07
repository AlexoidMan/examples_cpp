#include <iostream>
#include <tuple>
#include <memory>


template <typename ... Params>
struct Dummy
{
    std::tuple<Params...>  data;

};

template <typename ... Params>
struct Dummy2
{
    std::tuple<std::string, Params...>  data;

};

// tuple of smart pointers to variadic types
template <typename ... Params>
struct Dummy3
{
    std::tuple<Params* ...>  pointers;
    std::tuple< std::unique_ptr<Params> ...>  unique_pointers;

};

template <typename ... Types>
struct Dummy4
{
    std::pair<Types...> data;
};

//Function template
// args - package of parameters
template<typename ...Args>
size_t foo(Args ... args)
{
    auto argsNum = sizeof... (Args);
    if(argsNum > 2)
        std::cout <<  " Too Many Arguments! " << std::endl;


    return sizeof... (Args);  //returns the number of input arguments
}

int main(int argc, char** argv)
{
    std::cout << "Variadic template :" << std::endl;

    Dummy<int, double,char> dummy ;
    dummy.data={10, 3.8f, 'A'};

    std::cout << "Dummy<int, double,char>:" << std::get<int>(dummy.data) << ", " 
    << std::get<double>(dummy.data) << ", " 
    << std::get<char>(dummy.data) << ", " 
    << std::endl;

    Dummy2<int, double,char> dummy2 ;
    dummy2.data={"Hello Dude!",10, 3.8f, 'A'};

    std::cout << "Dummy2<std::string, int, double,char>:"<< std::get<std::string>(dummy2.data) << ", " 
    << std::get<int>(dummy2.data) << ", " 
    << std::get<double>(dummy2.data) << ", " 
    << std::get<char>(dummy2.data) << ", " 
    << std::endl;

    // should create pointers to int, double, char
    int *p1;
    double *p2;
    char *p3;

    Dummy3<int, double,char> dummy3 ;
    dummy3.pointers = {p1, p2, p3};
    
    std::tie(p1, p2, p3) = dummy3.pointers;

    std::cout << "Dummy3<int, double,char>  types:"
                  << typeid(p1).name() << ", " 
                  << typeid(p2).name() << ", " 
                  << typeid(p3).name() << ", " 
                  << std::endl;

    Dummy4<int, std::string*> d1;
    Dummy4<int, double> d2;
    // Dummy4<int, std::string*, int> d3;//Error- to many types for pair

    std::cout << "Function template:" << std::endl;

   
    std::cout << " foo<int, double>(1, 2.2)|  Arguments :" <<  foo<int, double>(1, 2.2) << std::endl;
    std::cout << " foo<int, double, double>(1, 2.2, 3.0)|  Arguments :" <<  foo<int, double, double>(1, 2.2, 3.0) << std::endl;
    std::cout << " foo<char>('C')|  Arguments :" <<  foo<char>('A') << std::endl;


    return 0;
}