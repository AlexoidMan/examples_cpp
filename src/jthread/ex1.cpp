#include <iostream>
#include <string>
#include <stop_token>
#include <future>
#include <thread>
#include <syncstream>
#include <chrono>
using namespace::std::literals; //for duration literals

auto syncOut(std::ostream& strm= std::cout)
{
  return std::osyncstream{strm};
}

void task(std::stop_token st, int num)
{
  auto id = std::this_thread::get_id();
  syncOut() << "call task(" << num  << ")\n";

  //register a first callback
   std::stop_callback cb1 {st, [num, id]{ 
     syncOut() << "- STOP1 requested in task)" << num << (id == std::this_thread::get_id() ? ")\n" : ") in main thread \n") << std::flush;
    } };
    std::this_thread::sleep_for(9ms);

  //register a second callback
   std::stop_callback cb2 {st, [num, id]{ 
     syncOut() << "- STOP2 requested in task)" << num << (id == std::this_thread::get_id() ? ")\n" : ") in main thread \n") << std::flush;
    } };
    std::this_thread::sleep_for(20ms);

   //while(1); //in this case stopcallback doesn't work

  syncOut() << "   thread finished: " << num  << "\n";
}

int main(int argc, char** argv)
{
   //create stop_source and token
    std::stop_source ssrc;
    std::stop_token stok{ssrc.get_token()};

   //register callback
   std::stop_callback cb {stok, []{ 
     syncOut() << "- STOP requested in main()\n" << std::flush;
    } };

    //call task() a bunch of times in the background
    auto fut = std::async([stok] {
      for (int num = 1; num < 10; ++num){
        task(stok, num);
      }
    });

//after a while , request stop:
    std::this_thread::sleep_for(25ms);
    ssrc.request_stop();

    return 0;
}