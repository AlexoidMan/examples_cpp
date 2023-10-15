#include <iostream>
#include <string>

class Abstract_productA
{
  public:
    virtual ~Abstract_productA(){}
    virtual std::string usefullFunc_A() = 0;
};

class concrete_productA1 : public Abstract_productA
{
  public:
    virtual std::string usefullFunc_A() override
    {
      return "concrete productA1: usefull func A called!";
    }
};

class concrete_productA2 : public Abstract_productA
{
  public:
    virtual std::string usefullFunc_A() override
    {
      return "concrete productA2: usefull func A called!";
    }
};


/// @brief Product B  // ====================================
class Abstract_productB
{
  public:
    virtual ~Abstract_productB(){}
    virtual std::string usefullFunc_B() = 0;
};

class concrete_productB1 : public Abstract_productB
{
  public:
    virtual std::string usefullFunc_B() override
    {
      return "concrete productB1: usefullFunc_B called!";
    }
};

class concrete_productB2 : public Abstract_productB
{
  public:
    virtual std::string usefullFunc_B() override
    {
      return "concrete productB1: usefullFunc_B called!";
    }
};


//  Factory itself
class Factory_abstract
{
  public:
    virtual Abstract_productA* create_product_A() = 0;
    virtual Abstract_productB* create_product_B() = 0;

};

// factory1
class factory_1 : public Factory_abstract
{
  public:

    virtual Abstract_productA* create_product_A() override
    {
      return new concrete_productA2();
    }

    virtual Abstract_productB* create_product_B() override
    {
      return new concrete_productB2();
    }

};

// factory2
class factory_2 : public Factory_abstract
{
  public:

    virtual Abstract_productA* create_product_A() override
    {
      return new concrete_productA1();
    }

    virtual Abstract_productB* create_product_B() override
    {
      return new concrete_productB1();
    }

};

void create_product(Factory_abstract & factory)
{

  Abstract_productA* prodA = factory.create_product_A();
  Abstract_productB* prodB = factory.create_product_B(); 
  
  std::cout << prodA->usefullFunc_A() << '\n';
  std::cout << prodB->usefullFunc_B() << '\n';

}


int main(int argc, char** argv)
{
    std::cout << "Abstracy factory example:" << std::endl;

    factory_1  factory1;
    factory_2  factory2;

    create_product(factory1);
    create_product(factory2);

    std::cout << std::endl;
    return 0;
}