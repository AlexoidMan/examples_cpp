#include <string>
#include <exception>
#include <iostream>
#include <functional>


struct CommitFlag {
  mutable bool commit_ = false;
  CommitFlag() = default;
  CommitFlag(CommitFlag&& other) noexcept : commit_(std::exchange(other.commit_, true)) {}
  void commit() const noexcept { commit_ = true; }
};

namespace ScopeGuardTypeErased {
class ScopeGuard : public CommitFlag {
  public:
  template <typename Callable> ScopeGuard(Callable&& callable) : callable_(std::forward<Callable>(callable)) {}
  ~ScopeGuard() { if (!commit_) callable_(); }
  ScopeGuard(ScopeGuard&& other) = default;
  private:
  std::function<void()> callable_;
  ScopeGuard& operator=(const ScopeGuard&) = delete;
};
}

namespace ScopeGuardTypeErasedFast {
template <size_t S = 16>
class ScopeGuard : public CommitFlag {
  alignas(8) char space_[S];
  using guard_t = void(*)(void*);
  guard_t guard_ = nullptr;
  template<typename Callable> static void invoke(void* callable) {
    (*static_cast<Callable*>(callable))();
  }
  mutable bool commit_ = false;
  public:
  template <typename Callable, typename D = std::decay_t<Callable>> ScopeGuard(Callable&& callable) :
    guard_(invoke<Callable>)
  {
    static_assert(sizeof(Callable) <= sizeof(space_));
    static_assert(std::is_trivially_destructible_v<Callable>);
    static_assert(std::is_trivially_copyable_v<Callable>);
    ::new(static_cast<void*>(space_)) D(std::forward<Callable>(callable));
  }
  ScopeGuard(ScopeGuard&& other) = default;
  ~ScopeGuard() { if (!commit_) guard_(space_); }
};


template <typename Callable>
ScopeGuard<Callable> MakeGuard(Callable&& callable) {
  return ScopeGuard<Callable>(std::forward<Callable>(callable));
}
}


int main(int argc, char** argv)
{
    std::cout << "type-erased ScopeGuard example:" << std::endl;

    {
      int* p = nullptr;
       ScopeGuardTypeErasedFast::ScopeGuard<> SG([&] { delete p; });
    }

    return 0;
}
