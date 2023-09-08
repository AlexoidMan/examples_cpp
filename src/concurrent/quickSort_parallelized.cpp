#include <iostream>
#include <list>
#include <future>


template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if(input.empty())
    {
        return input;
    }

    std::list<T> result;
    result.splice(result.begin(), input, input.begin());

    T const& pivot = *result.begin();
    auto divide_point=std::partition(input.begin(), input.end(),  //Rearranges the elements from the range [first,last), in such a way that all the elements for which pred returns true precede all those for which it returns false.
                    [&](T const& t) { return t < pivot;});

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(),
                      divide_point);
    
    std::future<std::list<T> > new_lower( std::async(&parallel_quick_sort<T>, std::move(lower_part)));
    auto new_higher(parallel_quick_sort(std::move(input)));

    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower.get());

    return result;
};

int main(int argc, char** argv)
{
    std::cout << "Quick Sort parallel :" << std::endl;

    std::list<double> l1 {2.0, 1.1, 5.6, 3.34, 7.6, 0.23};
    
    auto result = parallel_quick_sort(l1);


    std::cout << "after sorting:" << std::endl;
    for(auto& v : result)
    {
        std::cout << v << " ";

    }

    std::cout << std::endl;
    return 0;
}