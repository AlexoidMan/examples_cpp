struct CoroTask::promise_type
{
    auto get_return_object()
    {
        return CoroTask{CoroHdl::from_promise(*this)};

    }

    auto initial_suspend()  //initial suspend point
    {
        return std::suspend_always{};  //suspend immediately at start. (std::suspend_never - crn start immediately after initialization)
    }

    void unhandled_exception()  //deal with exceptions
    {
        std::terminate();  //terminate the programm
    }

    void return_void()  //deal with the end or co_return
    { }

    auto final_suspend() noexcept
    {
        return std::suspend_always{}; //suspend immediately
    }

};