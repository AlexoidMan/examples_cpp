#include <iostream>
#include "coro.hpp"  //has function that describes coroutine

int main(int argc, char* argv[])
{
#ifdef false
    //start coroutine:
    auto coroTask = coro(3); //initialize coroutine
    auto coroTask2 = coro(5); //initialize coroutine

    std::cout << "MAIN coro(3) and coro(5) started" <<std::endl;

    coroTask2.resume();

    //loop to resume the coroutine until it is done
    while (coroTask.resume())
    {
        std::cout << "MAIN coro()  suspended\n";

    }

    std::cout << "MAIN coro()  done\n";


    coroTask2.resume();//resume again
#endif

    auto coroTask = callcoro(); //initialize coroutine
    std::cout << "MAIN callcoro() initialized" <<std::endl;

    while (coroTask.resume())
    {
        std::cout << "MAIN callCoro()  suspended\n";
    }

    std::cout << "MAIN callCoro()  done \n";

    return 0;
}
