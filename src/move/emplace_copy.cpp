#include <iostream>
#include <vector>
#include <memory>

class Client
{
public:
    Client(){}
    ~Client(){
        std::cout << "Deleting Client ... \n";

    }

    Client(const Client& c )
    {
         std::cout << "Client Copy constructor \n";
    }

    Client& operator=(const Client& c )
    {
        std::cout << "Client Copy constructor(operator=) \n";
        return *this;
    }

    Client(const Client&& c )
    {
         std::cout << "Client Move constructor \n";
    }

    Client& operator=(const Client&& c )
    {
        std::cout << "Client Move constructor(operator=) \n";
        return *this;
    }

  
  int m_var;

};

#ifdef false
int main(int argc, char* argv[])
{

    std::cout << "MAIN  \n";

    std::vector<Client> vClients;

    // Client c;
    // vClients.push_back(c);   //copy
    //vClients.push_back(Client()); //move

    // vClients.push_back(std::move(Client()));  //move + Copy constructor inside vector
    // vClients.push_back(std::move(Client()));

    std::cout << "Emplace_back:  \n";

    vClients.emplace_back(Client()); //move
    vClients.emplace_back(Client()); //move
    vClients.emplace_back(Client()); //move
    vClients.emplace_back(Client()); //move


    return 0;
}

#endif
// ./emplace_copy 
// MAIN  
// Emplace_back:  
// Client Move constructor 
// Deleting Client ... 
// Client Move constructor 
// Client Copy constructor 
// Deleting Client ... 
// Deleting Client ... 
// Client Move constructor 
// Client Copy constructor 
// Client Copy constructor 
// Deleting Client ... 
// Deleting Client ... 
// Deleting Client ... 
// Client Move constructor 
// Deleting Client ... 
// Deleting Client ... 
// Deleting Client ... 
// Deleting Client ... 
// Deleting Client ... 


int main(int argc, char* argv[])
{

    std::cout << "MAIN  \n";

    std::vector<std::shared_ptr<Client>> vClients;

    // Client c;
    vClients.push_back(std::shared_ptr<Client>(new Client));  //copy
    //vClients.push_back(Client()); //move

    // vClients.push_back(std::move(Client()));  //move + Copy constructor inside vector
    // vClients.push_back(std::move(Client()));

    std::cout << "Emplace_back:  \n";

    vClients.emplace_back(new Client); //move
    vClients.emplace_back(new Client); //move
    vClients.emplace_back(new Client);  //move
    
    // for(auto p : vClients)
    //     delete p;

    return 0;
}