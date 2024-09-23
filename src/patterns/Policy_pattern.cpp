#if __cplusplus < 202002L
#error "This example needs C++20"
//#else
#endif // __cplusplus < 202002L  
///////////////////////////////////////////////////////////////

#include <vector>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <utility>
#include <cassert>

struct B;

struct A {
   A() {}
   ~A() {std::cout << "Destructor() ~A" << std::endl;}
  
  std::shared_ptr<B> ptr_B;
};

struct B {
   B() {}
   ~B() {std::cout << "Destructor() ~B" << std::endl;}
  
  //std::shared_ptr<A> ptr_A;  //strong reference to A
  std::weak_ptr<A> ptr_A;  //weak/strong reference to A

};
/////////////////////////////////////////////////////////////////
/// Policies definition: 

template <typename T>
struct DeleteByOperator {
    void operator()(T* p) const { 
        delete p;
    }
};

template <typename T>
struct DeleteByFree {
    void operator()(T* p) const {
        p->~T();
        free(p);
    }
};

template <typename T>
struct DeleteDestructorOnly {
    void operator()(T* p) const {
        p->~T();
    }
};

class SmallHeap {
    public:
    SmallHeap() {}
    ~SmallHeap() {}
    void* allocate(size_t s) {
        assert(s <= size_);
        return mem_;
    }
    void deallocate(void* p) {
        assert(p == mem_);
    }
    private:
    static constexpr size_t size_ = 1024;
    char mem_[size_];
    SmallHeap(const SmallHeap&) = delete;
    SmallHeap& operator=(const SmallHeap&) = delete;
};
void* operator new(size_t s, SmallHeap* h) { return h->allocate(s); }

template <typename T>
struct DeleteSmallHeap {
    explicit DeleteSmallHeap(SmallHeap& heap)
        : heap_(&heap) {}

    void operator()(T* p) const {
        p->~T();
        heap_->deallocate(p); //using own heap(SmallHeap)
    }

    DeleteSmallHeap(DeleteSmallHeap&& that)
        : heap_(that.heap_) 
    {
        that.heap_ = nullptr;
    }
    DeleteSmallHeap(const DeleteSmallHeap&) = delete;
    DeleteSmallHeap& operator=(const DeleteSmallHeap&) = delete;
    private:
    SmallHeap* heap_;
};

struct MoveForbidden {
    static constexpr bool enabled = false;
};

struct MoveAllowed {
    static constexpr bool enabled = true;
};


// SmartPtr with disabled by policy: [Copying, Moving]
////////////////////////////////////////////////////////////////////

template <typename T, typename DeletionPolicy = DeleteByOperator<T>,
                      typename MovePolicy = MoveForbidden>
class SmartPtr : private DeletionPolicy { // Empty base class optimization
    T* p_;
    public:
    explicit SmartPtr(T* p = nullptr,
                      DeletionPolicy&& deletion_policy = DeletionPolicy())
        : DeletionPolicy(std::move(deletion_policy)), p_(p)
    {}
    ~SmartPtr() { 
        DeletionPolicy::operator()(p_);
    }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
    SmartPtr(SmartPtr&& that) requires MovePolicy::enabled :
        DeletionPolicy(std::move(that)),
        p_(std::exchange(that.p_, nullptr))
    {}
    SmartPtr& operator=(SmartPtr&& that) requires MovePolicy::enabled {
        if (this == &that) return *this;
        DeletionPolicy::operator()(p_);
        p_ = std::exchange(that.p_, nullptr);
        DeletionPolicy::operator=(std::move(that));
        return *this;
    }
};

class C {
    int i_ {};
    public:
    explicit C(int i) : i_(i) { std::cout << "Constructing C(" << i << ") @" << this << std::endl; }
    ~C() { std::cout << "Destroying C @" << this << std::endl; }
    int get() const { return i_; }
};



int main(int argc, char** argv)
{
    std::cout << "Policy pattern example:" << std::endl;

    auto objectA = std::make_shared<A>();
    auto objectB = std::make_shared<B>();
    {
        //objects have strong references to each other
        objectA->ptr_B = objectB;
        objectB->ptr_A = objectA;
    }   
    if(objectB->ptr_A.expired())
    {
          std::cout << "pointer to A is Expired!" << std::endl;
  
    }else if(auto sharedP = objectB->ptr_A.lock() )
    {


    }

    //Policy itself
   {
        SmartPtr<C> c(new C(42));
        //SmartPtr<C> c1(std::move(c));  // Does not compile
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
        SmartPtr<C> c2;
        //c2 = std::move(c); // Does not compile
    }

    {
        SmallHeap h;
        SmartPtr<C, DeleteSmallHeap<C>> c{new(&h) C(42), DeleteSmallHeap<C>(h)};
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
    }

    {
        // C++17 does not help here since MovePolicy cannot be deduced from the constructor
        SmartPtr<C, DeleteByOperator<C>, MoveAllowed> c{new C(42)};
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
        auto c1(std::move(c));
        //std::cout << "C: " << c->get() << " @ " << &*c << std::endl; // This will crash
        std::cout << "C: " << c1->get() << " @ " << &*c1 << std::endl;
        SmartPtr<C, DeleteByOperator<C>, MoveAllowed> c2;
        c2 = std::move(c1);
        //std::cout << "C: " << c1->get() << " @ " << &*c1 << std::endl; // This will crash
        std::cout << "C: " << c2->get() << " @ " << &*c2 << std::endl;
    }

    std::cout << std::endl;
    return 0;
}
 
