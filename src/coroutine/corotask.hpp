#include <coroutine>


//coroutine interface 
// nodiscard - force compiler warnings when coroutine is created but not used
class [[nodiscard]] CoroTask
{
public:
    struct promise_type; //definition in corotaskpromise.hpp
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
   CoroHdl hdl;

public:
    CoroTask(auto h) : hdl{h} //store coroutine handle in interface
    {}

    ~CoroTask()
    {
        if(hdl)
        {
            hdl.destroy();
        }
    }

//don't copy
CoroTask(const CoroTask& ) = delete;
CoroTask&  operator=(const CoroTask& ) = delete;

//API to resume the coroutine
//returns whether there is still something to process
bool resume() const 
{
   if(!hdl || hdl.done())
       return false;


    hdl.resume();  //RESUME (blocks until suspended again or the end)
    return !hdl.done();   

}

};

#include "corotaskpromise.hpp"