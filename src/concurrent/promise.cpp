#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>
#include <syncstream>


// thread1.set_promise_0 => thread2:get_value(future_1), set_promise_2 => thread2:get_value(future_2), set_promise_3
void func (int num, std::promise<std::string> prom)
{
   std::osyncstream(std::cout) << "Thread running: " << std::this_thread::get_id() << " sleeping...\n";
  
//    std::this_thread::sleep_for(std::chrono::milliseconds(300));
   prom.set_value( "setvalue:" + std::to_string(num));

}


int main(int argc, char** argv)
{
    std::cout << "Promise example:" << std::endl;

    const size_t NUM_THREADS = 14;

    std::vector<std::thread> threads;
    std::vector<std::future<std::string>> futures_vector;

    for( auto i=0; i < NUM_THREADS; ++i)
    {
       
        std::promise<std::string> promise;
        futures_vector.emplace_back(promise.get_future());
        threads.emplace_back(std::thread(func, i, std::move(promise)));

    }

    std::string  main_result;

    // Create a full result
    for(auto& f : futures_vector) main_result += f.get() + "__";

    std::cout << "Main result:  " << main_result<< std::endl;

    for(auto& th : threads) th.join();

    std::cout << std::endl;
    return 0;
}