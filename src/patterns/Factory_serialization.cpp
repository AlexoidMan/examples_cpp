#include <vector>
#include <iostream>
#include <sstream>
#include <memory>

class SerializerBase {
  static size_t type_count;
  using Factory = SerializerBase* (*)(std::istream& s);
  static std::vector<Factory> registry;

  protected:
  virtual void Serialize(std::ostream& s) const = 0;

  public:
  virtual ~SerializerBase() {}
  static size_t RegisterType(Factory factory) {
    registry.push_back(factory);
    return type_count++;
  }

  static auto Deserialize(size_t type, std::istream& s) {
    Factory factory = registry[type];
    return std::unique_ptr<SerializerBase>(factory(s));
  }
};

std::vector<SerializerBase::Factory> SerializerBase::registry;
size_t SerializerBase::type_count = 0;

class DeserializerFactory {
  std::istream& s_;

  public:
  explicit DeserializerFactory(std::istream& s) : s_(s) {}
  template <typename It>
  void Deserialize(It iter) {
    while (true) {
      size_t type;
      s_ >> type;
      if (s_.eof()) return;
      iter = SerializerBase::Deserialize(type, s_);
    }
  }
};


class Resistor : public SerializerBase {
  int resistance;
  public:
  Resistor(int i) : resistance(i) { std::cout << "Resistor(" << i << ")" << std::endl; }
  void Serialize(std::ostream& s) const override { s << type_tag << " " << resistance << std::endl; }
  static const size_t type_tag;
};
const size_t Resistor::type_tag = RegisterType([](std::istream& s)->SerializerBase* { int i; s >> i; return new Resistor(i); });


class Coil : public SerializerBase {
  double l1_, l2_;
  public:
  Coil(double x, double y) : l1_(x), l2_(y) { std::cout << "Coil(" << x << ", " << y << ")" << std::endl; }
  void Serialize(std::ostream& s) const override { s << type_tag << " " << l1_ << " " << l2_ << std::endl; }
  static const size_t type_tag;
};
const size_t Coil::type_tag = RegisterType([](std::istream& s)->SerializerBase* { double x, y; s >> x >> y; return new Coil(x, y); });

class ShuntCompensator : public SerializerBase {
  bool isResistance_;
  int resistance_ {};
  double capacity_ {};
  public:
  ShuntCompensator(int i) : isResistance_(true), resistance_(i) { std::cout << "Derived3(" << i << ") - integer" << std::endl; }
  ShuntCompensator(double x) : isResistance_(false), capacity_(x) { std::cout << "Derived3(" << x << ") - double" << std::endl; }
  void Serialize(std::ostream& s) const override { s << type_tag << " " << isResistance_ << " "; if (isResistance_) s << resistance_; else s << capacity_; s << std::endl; }
  static const size_t type_tag;
};
const size_t ShuntCompensator::type_tag = RegisterType([](std::istream& s)->SerializerBase* {
  bool isResistance; s >> isResistance;
  if (isResistance) {
    int i; s >> i; return new ShuntCompensator(i);
  } else {
    double x; s >> x; return new ShuntCompensator(x);
  }
});


int main(int argc, char** argv)
{
    std::cout << "Factory serialization pattern(dynamic registry) example:" << std::endl;

    std::stringstream S;

    // Serialization.
    Resistor(42).Serialize(S);
    Coil(3.1415, 2.71828).Serialize(S);
    ShuntCompensator(0.5772156649).Serialize(S);
    ShuntCompensator(144).Serialize(S);

    std::cout << S.str() << std::endl;

    // Deserialization.
    std::vector<std::unique_ptr<SerializerBase>> v;
    DeserializerFactory F(S);
    F.Deserialize(std::back_inserter(v));

    return 0;
}
     
