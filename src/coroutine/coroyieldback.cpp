#include "coroyieldback.hpp"
#include<iostream>
#include<vector>


int main()
{
    //start coroutine
    auto coroGen = coro(3);
    std::cout << "*** coro() started\n";

    std::cout << "\n**** resume coro()\n";
    while(coroGen.resume())
    {
        //process value from co_yield
        auto val = coroGen.getValue();
        std::cout << "*** coro() suspended with " << val << std::endl;

        //set response (the value co_yield :)
        std::string back = (val % 2 != 0 ? "OK" : "ERR");
        std::cout << "\n*** resume coro() with back value: " << back << std::endl;
        coroGen.setBackValue(back);
    }

    std::cout << "*** coro() done\n" << std::endl;

}