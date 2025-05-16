#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>

struct IfaceProduct {
    virtual std::string getDescription() = 0;
    virtual ~IfaceProduct() {}
};

class Car : public IfaceProduct {
public:
    Car() {}
    
    std::string getDescription() override final {
        return "This is car";
    }
};

class Truck : public IfaceProduct {
public:
    Truck() {}
    
    std::string getDescription() override final {
        return "This is truck";
    }
};

class Bus : public IfaceProduct {
public:
    Bus() {}
    
    std::string getDescription() override final {
        return "This is bus";
    }
};

//Factory Functions

using Product = std::unique_ptr<IfaceProduct>;

struct Factory {
    static Product createCar(){
        return std::make_unique<Car>();
    }

    static Product createBus(){
        return std::make_unique<Bus>();
    }

    static Product createTruck(){
        return std::make_unique<Truck>();
    }
};

int main(int argc, char** argv){
    std::vector<Product> product_list;
    
    product_list.push_back(Factory::createCar());
    product_list.push_back(Factory::createTruck());
    product_list.push_back(Factory::createBus());
    
    std::for_each(product_list.begin(), product_list.end(), [](Product & prod){
        std::cout << prod->getDescription() << '\n';
    });
}
