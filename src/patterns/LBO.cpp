#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <vector>

#include "benchmark/benchmark.h"

class simple_string {
    public:
    simple_string() = default;
    explicit simple_string(const char* s) : s_(strdup(s)) {}
    simple_string(const simple_string& s) : s_(strdup(s.s_)) {}
    simple_string& operator=(const char* s) {
        free(s_);
        s_ = strdup(s);
        return *this;
    }
    simple_string& operator=(const simple_string& s) {
        if (this == &s) return *this;
        free(s_);
        s_ = strdup(s.s_);
        return *this;
    }
    bool operator==(const simple_string& rhs) const { return strcmp(s_, rhs.s_) == 0; }
    ~simple_string() { free(s_); }
    private:
    char* s_ = nullptr;
};

class small_string {
    public:
    small_string() { memset(&b_, 0, sizeof(b_)); }
    explicit small_string(const char* s) {
        if (strlen(s) + 1 < sizeof(b_.buf)) {
            strncpy(b_.buf, s, sizeof(b_.buf));
            b_.tag = 0;
        } else {
            s_ = strdup(s);
            b_.tag = 1;
        }
    }
    small_string(const small_string& s) {
        if (s.b_.tag == 0) {
            b_ = s.b_;
        } else {
            s_ = strdup(s.s_);
            b_.tag = 1;
        }
    }
    small_string& operator=(const char* s) {
        if (b_.tag == 1) free(s_);
        if (strlen(s) + 1 < sizeof(b_.buf)) {
            strncpy(b_.buf, s, sizeof(b_.buf));
            b_.tag = 0;
        } else {
            s_ = strdup(s);
            b_.tag = 1;
        }
        return *this;
    }
    small_string& operator=(const small_string& s) {
        if (this == &s) return *this;
        if (b_.tag == 1) free(s_);
        if (s.b_.tag == 0) {
            b_ = s.b_;
        } else {
            s_ = strdup(s.s_);
            b_.tag = 1;
        }
        return *this;
    }
    bool operator==(const small_string& rhs) const {
        const char* s1 = (b_.tag == 0) ? b_.buf : s_;
        const char* s2 = (rhs.b_.tag == 0) ? rhs.b_.buf : rhs.s_;
        return strcmp(s1, s2) == 0;
    }
    ~small_string() { if (b_.tag == 1) free(s_); }
    private:
    union {
    char* s_ = nullptr;
    struct {
        char buf[15];
        char tag;
    } b_;
    };
};

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

template <typename T>
void BM_string_create_short(benchmark::State& state) {
    const char* s = "Simple string";
    for (auto _ : state) {
        REPEAT({ T S(s); benchmark::DoNotOptimize(S); })
    }
    state.SetItemsProcessed(32*state.iterations());
}

template <typename T>
void BM_string_copy_short(benchmark::State& state) {
    const T s("Simple string");
    for (auto _ : state) {
        REPEAT({ T S(s); benchmark::DoNotOptimize(S); })
    }
    state.SetItemsProcessed(32*state.iterations());
}

template <typename T>
void BM_string_assign_short(benchmark::State& state) {
    const T s("Simple string");
    T S;
    for (auto _ : state) {
        REPEAT({ benchmark::DoNotOptimize(S = s); })
    }
    state.SetItemsProcessed(32*state.iterations());
}

template <typename T>
void BM_string_compare_short(benchmark::State& state) {
    const size_t N = state.range(0);
    const T s("Simple string");
    std::vector<T> v1, v2;
    v1.reserve(N);
    v2.reserve(N);
    {
        std::vector<void*> v0(2*N);
        for (size_t i = 0; i < N; ++i) {
            v1.push_back(s);
            v0[i] = malloc(rand() % 32);
            v2.push_back(s);
            v0[N + i] = malloc(rand() % 32);
        }
        for (size_t i = 0; i < N; ++i) {
            free(v0[i]);
            free(v0[N + i]);
        }
    }
    for (auto _ : state) {
        for (size_t i = 0; i < N; ++i) benchmark::DoNotOptimize(v1[i] == v2[i]);
    }
    state.SetItemsProcessed(N*state.iterations());
}

template <typename T>
void BM_string_create_long(benchmark::State& state) {
    const char* s = "Slightly longer string";
    for (auto _ : state) {
        REPEAT({ T S(s); benchmark::DoNotOptimize(S); })
    }
    state.SetItemsProcessed(32*state.iterations());
}

BENCHMARK_TEMPLATE1(BM_string_create_short, simple_string);
BENCHMARK_TEMPLATE1(BM_string_create_short, small_string);
BENCHMARK_TEMPLATE1(BM_string_copy_short, simple_string);
BENCHMARK_TEMPLATE1(BM_string_copy_short, small_string);
BENCHMARK_TEMPLATE1(BM_string_assign_short, simple_string);
BENCHMARK_TEMPLATE1(BM_string_assign_short, small_string);
#define ARG Arg(1<<22)
BENCHMARK_TEMPLATE1(BM_string_compare_short, simple_string)->ARG;
BENCHMARK_TEMPLATE1(BM_string_compare_short, small_string)->ARG;
#undef ARG
BENCHMARK_TEMPLATE1(BM_string_create_long, simple_string);
BENCHMARK_TEMPLATE1(BM_string_create_long, small_string);

static const long max_threads = sysconf(_SC_NPROCESSORS_CONF);
BENCHMARK_TEMPLATE1(BM_string_create_short, simple_string)->ThreadRange(1, max_threads);
BENCHMARK_TEMPLATE1(BM_string_create_short, small_string)->ThreadRange(1, max_threads);

BENCHMARK_MAIN();