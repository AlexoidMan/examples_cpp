#include <vector>
#include <numeric>
#include <iostream>
#include <cassert>
#include <syncstream>
#include <stdexcept>
#include <string_view>
#include <boost/stacktrace.hpp>

#define BOOST_ENABLE_ASSERT_DEBUG_HANDLER


namespace boost {
    inline void assertion_failed_msg(char const* expr, char const* msg, char const* function, char const* /*file*/, long /*line*/) {
        std::cerr << "Expression '" << expr << "' is false in function '" << function << "': " << (msg ? msg : "<...>") << ".\n"
            << "Backtrace:\n" << boost::stacktrace::stacktrace() << '\n';

        std::abort();
    }

    inline void assertion_failed(char const* expr, char const* function, char const* file, long line) {
        ::boost::assertion_failed_msg(expr, 0 /*nullptr*/, function, file, line);
    }
} // namespace boost


void bar(int i)
{
    std::cout << boost::stacktrace::stacktrace();
    
}

int main(int argc, char** argv)
{
    std::cout << "StackTrace example:" << std::endl;

    bar(0);

    assert(false );

    return 0;
}