#include<iostream>
#include "corogenback.hpp" //for CoroGenBack

CoroGenBack coro(int max)
{
    std::cout << "      CORO" << max << " start\n";

    for(int val = 1; val <= max; ++val)
    {
        std::cout << "      CORO "<< val << '/' << max << std::endl;

        //yield next value:
        auto back = co_yield val;
        std::cout << "      CORO =>"<< back << std::endl;
    }

    std::cout << "      CORO "<< max <<  "  End. " << std::endl;

}