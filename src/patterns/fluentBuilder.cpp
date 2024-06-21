#include <iostream>
#include <string>
#include <cassert>
#include <string_view>
#include <vector>


class Employee{
    std::string prefix_;
    std::string first_name_;
    std::string middle_name_;
    std::string last_name_;
    std::string suffix_;
    friend class EmployeeBuilder;
    Employee() = default;

    public:
    friend std::ostream& operator<<(std::ostream& out, const Employee& e);


};

class EmployeeBuilder
{
    Employee e_;

public:
    
    EmployeeBuilder() = default;
    
    EmployeeBuilder& SetPrefix(std::string_view s) {
        e_.prefix_ = s; return *this;
    }

    EmployeeBuilder& SetFirstName(std::string_view s) {
        e_.first_name_ = s; return *this;
    }

    EmployeeBuilder& SetMiddleName(std::string_view s) {
        e_.middle_name_ = s; return *this;
    }

    EmployeeBuilder& SetLastName(std::string_view s) {
        e_.last_name_ = s; return *this;
    }

    EmployeeBuilder& SetSuffix(std::string_view s) {
        e_.suffix_ = s; return *this;
    }

    operator Employee() {
        assert(!e_.first_name_.empty() &&
                !e_.last_name_.empty());

       // std::cout << "operator Employee() called." << std::endl;
        return std::move(e_);
    }


};

int main(int argc, char** argv)
{
    std::cout << "Fluent Builder example:" << std::endl;
    // Simple Builder:
    Employee Homer = EmployeeBuilder().SetFirstName("HouseHolder").SetMiddleName("JJ").SetLastName("Watson");
     
    std::cout << std::endl;
    return 0;
}