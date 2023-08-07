#include <iostream>
#include "generator.hpp"  // for Generator<>

Generator<int> coro(int max)
{
  std::cout << "CORO " << max << " start\n";

  for (int val = 1; val <= max; ++val) {
    // print next value:
    std::cout << "        CORO " << val << '/' << max << '\n';

    // yield next value:
    co_yield val;         // SUSPEND with value

   // this->getNext();
    //std::cout << "        CORO after co_yield " <<  val << '/' << max << '\n';
  }

  std::cout << "CORO " << max << " end\n";
}