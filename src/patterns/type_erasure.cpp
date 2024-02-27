#include <iostream>
#include <string>
#include <type_traits>
#include <new>
#include <memory>

#include "benchmark/benchmark.h"
#include "benchmark/export.h"

// benchmarking
struct deleter {
  template <typename T> void operator()(T* p) { delete p; }
};
template <typename T>
struct deleter1 {
  void operator()(T* p) { delete p; }
};

static deleter d;

int* get_raw_ptr() {
    return new int(0);
}


//---------------------------------------------


//Type Erasure  using inheritance
template <typename T> 
class smartptr_te
{
    struct destroy_base {
        virtual void operator()(void*) = 0;
        virtual ~destroy_base() {}
    };

    template <typename Deleter>
    struct destroy : public destroy_base {
        destroy (Deleter d) : d_(d) {}
        void operator()(void* p) override 
        {
            d_(static_cast<T*>(p));
        }

        Deleter d_;
    };
public:
    template <typename Deleter> smartptr_te(T* p, Deleter d):
        p_(p), d_(new destroy<Deleter>(d)) { }
    
    ~smartptr_te() { (*d_)(p_); delete d_; }

    T* operator->() { return p_; }
    const T* operator->() const { return p_; }

private:
    T* p_;
    destroy_base* d_;

};

smartptr_te<int> get_smartptr_te() {
    return smartptr_te<int>(new int(0), d);
}

// Type Erasure - static implementation
template <typename T>
class smartptr_te_static {
  T* p_;
  using destroy_t = void(*)(T*, void*);
  destroy_t destroy_;
  alignas(8) char buf_[8]; // holds Deleter object
  template<typename Deleter> static void invoke_destroy(T* p, void* d) {
    (*static_cast<Deleter*>(d))(p);
  }
  public:
  template <typename Deleter> smartptr_te_static(T* p, Deleter d)
    : p_(p), destroy_(invoke_destroy<Deleter>)
  {
    static_assert(sizeof(Deleter) <= sizeof(buf_));
    static_assert(std::is_trivially_destructible_v<Deleter>);
    ::new (static_cast<void*>(buf_)) Deleter(d);
  }
  ~smartptr_te_static() {
    this->destroy_(p_, buf_);
  }
  T* operator->() { return p_; }
  const T* operator->() const { return p_; }
};

smartptr_te_static<int> get_smartptr_te_static() {
    return smartptr_te_static<int>(new int(0), d);
}
//---------------------------------------------------------------------

//benchmarki function implementation:
void BM_rawptr(benchmark::State& state) {
    for (auto _ : state) {
        int* p(get_raw_ptr());
        d(p);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_smartptr_te());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te_static(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_smartptr_te_static());
    }
    state.SetItemsProcessed(state.iterations());
}

// ---- Benchmarking ---------

//register function
BENCHMARK(BM_rawptr);
BENCHMARK(BM_smartptr_te);
BENCHMARK(BM_smartptr_te_static);

BENCHMARK_MAIN();


// int main(int argc, char** argv)
// {
//     std::cout << "Type Erasure example:" << std::endl;

//     smartptr<int> my_ptr1(10);
     
//     std::cout << std::endl;
//     return 0;
// }