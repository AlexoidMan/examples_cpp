#include <iostream>
#include "cororange.hpp"  

int main(int argc, char* argv[])
{
    auto gen = coro(3); //initialize coroutine
    std::cout << "MAIN coro() started" <<std::endl;

    for(const auto& val : gen)
    {
        std::cout << "   val: " << val << '\n';
    }

    
    std::cout << "MAIN coro()  done \n";

    return 0;
}
