
#include <iostream>
#include <string>
#include <string_view>
#include <memory>

using std::cout;
using std::endl;

class Cat;
class Dog;

//declare the visitor abstract class
class PetVisitor
{
public:
 virtual void visit(Cat& c)=0;
 virtual void visit(Dog& d)=0;

};

class Pet
{
public:
    Pet(std::string_view color): color_(color)
    { }

    virtual void accept(PetVisitor& v) = 0;

    const std::string& Color() const {return color_;}
private:
    std::string color_;
};

class Cat : public Pet
{
public:

    Cat(std::string_view color): Pet(color)
    {}

    virtual void accept(PetVisitor& v) override
    {
        v.visit(*this); //visit Cat object
    }
};

class Dog : public Pet
{
public:

    Dog(std::string_view color): Pet(color)
    {}

    virtual void accept(PetVisitor& v) override
    {
        v.visit(*this); //visit Cat object
    }
};



class WashPetVisitor: public PetVisitor
{
  virtual void visit(Cat& c) override
  {
    cout <<  "Wash with shampoo for the cat. Cat color is " << c.Color() << endl;
  }

  virtual void visit(Dog& d) override
  {
    cout <<  "Wash with shampoo for the dog or simple HeadAndShoulders:  " << d.Color() << endl;
  }

};



//---------------------------------------
int main() {
    std::unique_ptr<Cat> cat(new Cat("black"));
    std::unique_ptr<Dog> dog(new Dog("yellow"));
    
    WashPetVisitor washv;

    cat->accept(washv);
    dog->accept(washv);


    cout << endl;
}