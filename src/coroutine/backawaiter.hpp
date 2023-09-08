template <typename Hdl>
class BackAwaiter {
    Hdl hdl = nullptr;
public:
    BackAwaiter() = default;

    bool await_ready() const noexcept
    {
        return false;  //do suspend
    }

    void await_suspend(Hdl h) noexcept
    {
        hdl = h;  //save the handle to get access to its promise
    }

    auto await_resume() const noexcept
    {
        return hdl.promise().backValue; // return back value stored in the promise
    }

};