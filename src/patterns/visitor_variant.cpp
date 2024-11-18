#include <iostream>
#include <memory>
#include <variant>
#include <string_view>

using Fish = std::variant<class Carp, class Karas, class Salmon>;

class FishBase {
    public:
    FishBase(std::string_view color) : color_(color) {}
    const std::string& color() const { return color_; }
    private:
    const std::string color_;
};

class Carp : private FishBase {
    public:
    using FishBase::FishBase;
    using FishBase::color;
};

class Karas : private FishBase {
    public:
    using FishBase::FishBase;
    using FishBase::color;
};

class Salmon {
    public:
    Salmon(std::string_view body, std::string_view head) : body_(body), head_(head) {}
    std::string color() const { return body_ + " and " + head_; }
    private:
    const std::string body_;
    const std::string head_;
};

class FeedingVisitor {
    public:
    void operator()(const Carp& c) { std::cout << "Feed corn to the " << c.color() << " carp" << std::endl; }
    void operator()(const Karas& d) { std::cout << "Feed water worm to the " << d.color() << " karas" << std::endl; }
    void operator()(const Salmon& l) { std::cout << "Feed grain and worm to the " << l.color() << " salmon" << std::endl; }
};

class PlayingVisitor {
    public:
    void operator()(const Carp& c) { std::cout << "Play with feather with the " << c.color() << " carp" << std::endl; }
    void operator()(const Karas& d) { std::cout << "Play fetch with the " << d.color() << " karas" << std::endl; }
    void operator()(const Salmon& l) { std::cout << "Teach words to the " << l.color() << " salmon" << std::endl; }
};

//accept the variat as argument: Carp, Karas, Salmon
void fishing(const Fish& p) {
    class WalkingVisitor {
        public:
        void operator()(const Carp& c) { std::cout << "Let the " << c.color() << " carp swallow the bait. don't hurry to pull out!" << std::endl; }
        void operator()(const Karas& d) { std::cout << "Take the fishing rod quickly to get the" << d.color() << " karas" << std::endl; }
        void operator()(const Salmon& l) { std::cout << "Release the " << l.color() << " salmon into the water. Forbidden to catch the salmon!" << std::endl; }
    } v;
    std::visit(v, p);
}

int main() {
    Fish c = Carp("silver");
    Fish d = Karas("brown");
    Fish l = Salmon("grey", "black");

    FeedingVisitor fv;
    std::visit(fv, c);
    std::visit(fv, d);
    std::visit(fv, l);

    PlayingVisitor pv;
    std::visit(pv, c);
    std::visit(pv, d);
    std::visit(pv, l);

    fishing(c);
    fishing(d);
    fishing(l);
}
