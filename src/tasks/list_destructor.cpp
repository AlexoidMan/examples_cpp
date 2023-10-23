#include <iostream>
#include <string>
// a->b->c->a

bool remove_mode = false;

class Node
{
  public:
    Node* next;
    std::string name;


    Node(std::string name): name(name)
    {}

    ~Node()
    {

      std::cout << "delete:" << this->name << std::endl;

      if(remove_mode)
         return;
      
      remove_mode = true;
      Node* new_next = next;
      while(new_next != this)
      {
          auto ptr_to_next = new_next->next;
          
          
          delete new_next;

          new_next = ptr_to_next;
      }
    }

};



int main(int argc, char** argv)
{
    std::cout << "List desctructor example:" << std::endl;
    std::cout << "Circular list: a->b->c->d->a" << std::endl;

    Node* a = new Node("a");
    Node* b = new Node("b");
    Node* c = new Node("c");
    Node* d = new Node("d");

    a->next = b;
    b->next = c;
    c->next = d;
    d->next = a;

    delete a;
    
    std::cout << std::endl;
    return 0;
}