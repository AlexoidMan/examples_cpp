#include <iostream>
#include "corotask.hpp"  //for CoroTask
#include <coroutine>

//pass variables only by value
CoroTask coro(int max)
{
    std::cout << "       CORO " << max << "  start\n";

    for (int val=1; val <= max; ++val)
    {
        //print next value:
        std::cout << "     CORO " << val << '/'  << max << std::endl;

        co_await std::suspend_always{}; //SUSPEND

    }

    std::cout << "     CORO " << max << std::endl;

}

CoroTask coro_1()
{
      std::cout << "       coro_1: PART1\n";
      co_await std::suspend_always{}; 

      std::cout << "       coro_1: PART2\n";
}

CoroTask callcoro()
{
    std::cout << "       callcoro(): CALL coro_1()\n";

    auto sub = coro_1(); //init subroutine
    sub.resume();  //RESUME sub-coroutine

    std::cout << "       callcoro(): call sub.resume done\n";

    co_await std::suspend_always{}; //SUSPEND

    std::cout << "       callcoro(): CALL END! \n";
}
/*
CoroTask callcoro()
{
    std::cout << "       callcoro(): CALL coro_1()\n";

    co_await coro_1(); //init subroutine
    
    std::cout << "       callcoro(): call sub.resume done\n";

    co_await std::suspend_always{}; //SUSPEND

    std::cout << "       callcoro(): CALL END! \n";
}*/


