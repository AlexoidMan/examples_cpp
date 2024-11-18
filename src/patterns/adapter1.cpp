
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
using std::cout;
using std::endl;

//F - value parameter(time) of template
template <size_t F> class Date {
    public:
    explicit Date(std::time_t t) : t_(t) {}
    friend std::ostream& operator<<(std::ostream& out, const Date& d) {
        const tm local_tm = *localtime(&d.t_);
        const int t[3] = { local_tm.tm_mday, local_tm.tm_mon + 1, local_tm.tm_year + 1900 };
        constexpr size_t i1 = F/100;
        constexpr size_t i2 = (F - i1*100)/10;
        constexpr size_t i3 = F - i1*100 - i2*10;
        static_assert(i1 >= 0 && i1 <= 2 && i2 >= 0 && i2 <= 2 && i3 >= 0 && i3 <= 2 && i1 != i2 && i2 != i3, "Bad format");
        out << t[i1] << "/" << t[i2] << "/" << t[i3];
        return out;
    }

    private:
    const std::time_t t_;
};

using USA_Date = Date<102>; //positions: 0 - day, 1 - month, 2 - year 
using European_Date = Date<12>;
using ISO_Date = Date<210>;


// ------- Adapter Curry ------------------
struct much_less {
    template <typename T> bool operator()(T x, T y) { return x < y && std::abs(x - y) > tolerance; }
    static constexpr double tolerance = 0.2;
};

template<typename Container> void sort_much_less(Container& c) {
    std::sort(c.begin(), c.end(), much_less());
}


//---------------------------------------
int main() {
    const std::time_t now = std::time(NULL);
    cout << "Today in USA: " << USA_Date(now) << ", Europe: " << European_Date(now) << ", ISO: " << ISO_Date(now) << endl;

    // ------- Adapter Curry ------------------

    const size_t N = 20;
    std::vector<double> v(N);

    for (size_t i = 0; i < N; ++i) {
        v[i] = (N - i)*0.1;
    }
    sort_much_less(v);
    
    for (auto x : v) {
        cout << x << " ";
    }
    cout << endl;
}