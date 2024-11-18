// 08a with composable decorators
#include <iostream>
#include <string>
#include <mutex>
using std::cout;
using std::endl;

template <typename Callable> class DebugDecorator {
    public:
    template <typename F>
    DebugDecorator(F&& f, const char* s) : c_(std::forward<F>(f)), s_(s) {}
    template <typename... Args> auto operator()(Args&&... args) const {
        cout << "Invoking " << s_ << endl;
        using res_t = decltype(c_(std::forward<Args>(args)...));
        return this->call_impl(std::is_same<res_t, void>{}, std::forward<Args>(args)...);
    }

    private:
    Callable c_;
    const std::string s_;
    template <typename... Args> auto call_impl(std::false_type, Args&&... args) const {
            auto res = c_(std::forward<Args>(args)...);
            cout << "Result: " << res << endl;
            return res;
    }
    template <typename... Args> void call_impl(std::true_type, Args&&... args) const {
        c_(std::forward<Args>(args)...);
    }
};

template <typename Callable>
auto decorate_debug(Callable&& c, const char* s) {
    return DebugDecorator<Callable>(std::forward<Callable>(c), s);
}

template <typename Callable> class LockDecorator {
    public:
    template <typename F>
    LockDecorator(F&& f, std::mutex& m) : c_(std::forward<F>(f)), m_(m) {}
    template <typename... Args> auto operator()(Args&&... args) const {
        std::lock_guard<std::mutex> l(m_);
        return c_(std::forward<Args>(args)...);
    }

    private:
    Callable c_;
    std::mutex& m_;
};

template <typename Callable>
auto decorate_lock(Callable&& c, std::mutex& m) {
    return LockDecorator<Callable>(std::forward<Callable>(c), m);
}

struct R {
    int value() const { return rand(); }
};

template <typename Callable>
class DebugDecoratorNew
{
public:
    template <typename F>
    DebugDecoratorNew(F&& f, const char* s) : c_(std::forward<F>(f)), s_(s) {}

    template<typename... Args>
    auto operator() (Args... args) const
    {
        std::cout << "Invoking ...  "<< s_ << std::endl;
        using res_t = decltype(c_(std::forward<Args>(args)...));
        return this->call_impl(std::is_same<res_t, void>{}, std::forward<Args>(args)...);
    }


private:
    Callable c_;
    const std::string s_;

    template <typename... Args> auto call_impl(std::false_type, Args&&... args) const {
        auto res = c_(std::forward<Args>(args)...);
        cout << "Result[call_impl]: " << res << endl;
        return res;
    }
};

int main() {
    std::mutex m;
    auto f1 = decorate_lock(decorate_debug([](int i) { return i; }, "i->i"), m);
    f1(5);
    auto f2 = decorate_debug(decorate_lock([](int i, int j) { return i + j; }, m), "i+j");
    f2(5, 3);
    R r;
    auto f0 = decorate_lock([&]() { return r.value(); }, m);
    f0(); f0();
    auto incr = decorate_debug([](int& x) { ++x; }, "++x");
    int i = 0;
    incr(i);

    //decorator debug new
    auto f3 = [](int x, int y){ return x + y;};
    auto dcnew = DebugDecoratorNew<decltype(f3)>(f3, "f3() - new decorator");
    dcnew(12, 3);
    auto dcnew_lock = decorate_lock(dcnew, m); //with mutex
    dcnew_lock(100, 23);
    //std::cout << "f3 : " << f3(3, 5) << std::endl;
    return 0;
}