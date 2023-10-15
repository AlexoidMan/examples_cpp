#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>
#include <syncstream>

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include <boost/thread/future.hpp>

int main(int argc, char** argv)
{
     std::cout << "Future example:" << std::endl;

 //boost future continuations
 // Only f3.get() should be run first, other way {f1.get, f2.get}  - will be exception:
 // terminate called after throwing an instance of 'boost::wrapexcept<boost::future_uninitialized>'

   boost::future<int> f1 = boost::async([]() {
        std::cout << "future 1 running...  " << std::endl; 
        return 123; });

   boost::future<std::string> f2 = f1.then([](boost::future<int> f) {
      std::cout << "future 2 running...  " << std::endl; 
      std::cout << f.get() << std::endl; // here .get() won't block
      return std::string("GoodDay!");
   });

   boost::future<std::string> f3 = f2.then([](boost::future<std::string> f) {
      std::cout << "future 3 running...  " << std::endl; 
      std::cout << f.get() << std::endl; // here .get() won't block 
      return std::string("Gutan Aben!");
   });
    
    std::cout << "Run future continuation:  "<< std::endl << f3.get() << std::endl; 
    std::cout << std::endl;

    std::cout << "Shared future example:  "<< std::endl; 
    int data_shared = 555;


    return 0;
}