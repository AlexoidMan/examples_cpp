#include "backawaiter.hpp"
#include <coroutine>
#include <exception>
#include <string>

class [[nodiscard]] CoroGenBack {
public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl;

public:
    struct promise_type
    {
        int coroValue = 0;  //value to caller on suspension
     
        std::string backValue;

        auto yield_value(int val)
        {
            coroValue = val;
            backValue.clear();
            return BackAwaiter<CoroHdl> {};
        }

        //usual members
        auto get_return_object() { return CoroHdl::from_promise(*this); }
        auto initial_suspend() { return std::suspend_always{}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
        auto final_suspend() noexcept{ return std::suspend_always{}; }
    };

    CoroGenBack(auto h) : hdl{h} {}
    ~CoroGenBack() {if (hdl)  hdl.destroy();}

    CoroGenBack(const CoroGenBack&) = delete;
    CoroGenBack& operator=(const CoroGenBack&) = delete;

    //resume the coroutine API
    bool resume() const{
        if(!hdl || hdl.done()){
            return false;
        }
        hdl.resume(); //RESUME
        return !hdl.done();
    }

    //yield value from co_yield:
    int getValue() const{
        return hdl.promise().coroValue;
    }

    //set value back to the coroutine after suspension
    void setBackValue(const auto& val){
        hdl.promise().backValue = val;
    }

};