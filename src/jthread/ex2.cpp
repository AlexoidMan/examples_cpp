#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <stop_token>
#include <thread>
#include <condition_variable>
#include <chrono>
using namespace::std::literals; //for duration literals

int main(int argc, char** argv)
{
    std::queue<std::string> messages;
    std::mutex messagesMx;
    std::condition_variable_any messagesCV;

    //start messages which prints messages from the queue:
    std::jthread t1{ [&](std::stop_token st){
        while (!st.stop_requested()) {
            std::string msg;
            {
                //wait for the next message;
                std::unique_lock lock(messagesMx);
                if(!messagesCV.wait(lock, st,
                [&]{ return !messages.empty();}))
                {
                    return; //stop requested
                }
                //retrieve the next message out of the queue:
                msg = messages.front();
                messages.pop();
            }

            //print the next message:
            std::cout << "msg: " << msg << std::endl;
        }
    }};


    //store 3 messages and notify one waiting thread each time:
    for(std::string s : {"Tic", "Tac", "Toe"})
    {
        std::scoped_lock lg{messagesMx};
        messages.push(s);
        messagesCV.notify_one();
    }

    //after some time
    // stor 1 message and notify all waiting threads:
    std::this_thread::sleep_for(1s);
    {
        std::scoped_lock lg{messagesMx};
        messages.push("done");
        messagesCV.notify_all();
    }

    //after some time
    // end programm - request stop, which interrups wait() 
    std::this_thread::sleep_for(1s);

    return 0;
}

