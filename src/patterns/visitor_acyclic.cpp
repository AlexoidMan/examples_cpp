
#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include <exception>

using std::cout;
using std::endl;

//declare the visitor abstract class - should not depends on objects types[Cat, Dog ...]
class PetVisitor
{
public:
 virtual ~PetVisitor() {}
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

///////////////////////////////////////////////////
class Cat;
//each object now has its own visitor
class CatVisitor
{
public:
    virtual void visit(Cat* c) = 0;
};

class Dog;
//each object now has its own visitor
class DogVisitor
{
public:
    virtual void visit(Dog* d) = 0;
};
///////////////////////////////////////////////////

// Objects itself: cat, dog, ...
class Cat : public Pet
{
public:

    Cat(std::string_view color): Pet(color)
    {}

    virtual void accept(PetVisitor& v) override
    {
        if(CatVisitor* cv = dynamic_cast<CatVisitor*>(&v) )
        {
            cv->visit(this); //visit cat by pointer 
        }else
        {
            cout << "Exception: Catvisitor"<< endl;
            throw std::runtime_error("CatVisitor cast failed!!");
        }

 
    }
};

class Dog : public Pet
{
public:

    Dog(std::string_view color): Pet(color)
    {}

    virtual void accept(PetVisitor& v) override
    {
        if(DogVisitor* cv = dynamic_cast<DogVisitor*>(&v) )
        {
            cv->visit(this); //visit Dog by pointer 
        }else
        {
            cout << "Exception: Dogvisitor"<< endl;
            throw std::runtime_error("DogVisitor cast failed!!");
        }

 
    }
};

//visitors inherits from base visitor and needed particular clases
class FeedingVisitor : public PetVisitor, public CatVisitor, public DogVisitor 
{
    public:
    void visit(Cat* c) override { std::cout << "Feed tuna to the " << c->Color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Feed steak to the " << d->Color() << " dog" << std::endl; }
};


class WashPetVisitor: public PetVisitor, public DogVisitor
{
  //we washing only the Dog
  virtual void visit(Dog* d) override
  {
    cout <<  "Wash with shampoo for the dog or simple HeadAndShoulders:  " << d->Color() << endl;
  }

};



//---------------------------------------
int main() {
    std::unique_ptr<Cat> cat(new Cat("black"));
    std::unique_ptr<Dog> dog(new Dog("yellow"));
    
    WashPetVisitor washv;

   // cat->accept(washv); //error
    dog->accept(washv);

    FeedingVisitor fv;
    cat->accept(fv);
    dog->accept(fv);

    cout << endl;
}