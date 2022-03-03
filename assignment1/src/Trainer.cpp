//
//Created by spl211 on 08/11/2021.
#include "Customer.h"
#include "Trainer.h"
#include <iostream>



Trainer::Trainer(int t_capacity) : capacity(t_capacity), open(false),customersList(),orderList(), salary(0), firstOpen(false){};//constructor
Trainer* Trainer::clone(){
    return new Trainer(*this);
}

Trainer::Trainer(const Trainer &other): //copy constructor
    capacity(other.capacity),open(other.open),customersList(other.customersList),orderList(other.orderList),salary(other.salary),firstOpen(other.firstOpen){
    for (size_t i = 0; i < other.customersList.size(); i++) {
        customersList[i] = other.customersList[i]->clone();
    }
}


const Trainer& Trainer::operator=(const Trainer &other) {//copy assignment
    if (&other != this) {
        capacity = other.capacity;
        open = other.open;
        salary = other.salary;
        firstOpen = other.firstOpen;
        orderList = other.orderList;
        myDelete();
        for (size_t i = 0; i < other.customersList.size(); i++) {
            customersList.push_back(other.customersList[i]->clone());
        }
    }
    return *this;
}
Trainer::Trainer(Trainer &&other) : capacity(other.capacity), open(other.open),customersList(other.customersList),orderList(other.orderList), salary(other.salary),
                                    firstOpen(other.firstOpen) {//move constructor
    for (size_t i = 0; i < other.customersList.size(); i++) {
        customersList[i] = other.customersList[i];
        other.customersList[i] = nullptr;
    }
}

const Trainer &Trainer::operator=(Trainer &&other) {
    if (this != &other) {
        myDelete();
        for (size_t i = 0; i < other.customersList.size(); i++) {
            customersList.push_back(other.customersList[i]);
            other.customersList[i]=nullptr;
        }
        capacity = other.capacity;
        open = other.open;
        salary=other.salary;
        firstOpen=other.firstOpen;
        orderList=other.orderList;
    }
    return *this;
}

Trainer::~Trainer() { myDelete(); }//destractor


void Trainer::myDelete() {//for the constructor
    for (size_t i = 0; i < customersList.size(); i++) {
        delete customersList[i];
       // customersList[i] = nullptr;
    }
    customersList.clear();
    orderList.clear();
}
int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::Trainer::setCapacity(int newCapacity) { //we add
    capacity=newCapacity;
}
bool Trainer::isFirstOpen(){ //we add
    return firstOpen;
}
void Trainer::setFirstOpen(){
    firstOpen=true;
}

void Trainer::addCustomer(Customer* customer) {
    if (capacity == 0) {
        std::cout << "cannot add customer, Trainer is full" << std::endl;
    }
    else {
        customersList.push_back(customer);
        capacity--;
    }
}

void Trainer::removeCustomer(int id) {//FIX IT MEMORY LEAK
    for (auto it = customersList.begin(); it != customersList.end(); it++) {
        if ((*it)!=nullptr&&(*it)->getId() == id) {
            capacity--;;
            customersList.erase(it);//check it
            break;
        }
    }
}

Customer *Trainer::getCustomer(int id) {
    for (Customer *cus: customersList) {
        if (cus->getId() == id) {
            return cus;
        }
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers(){
        return customersList;
};

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;

}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    Customer *currCustumer = getCustomer(customer_id);
    std::vector<int> customerWorkouts = currCustumer->order(workout_options);
    for (size_t j = 0; j < customerWorkouts.size(); j++){//workouts_id[j]==workout id.
        Workout currWorkout=workout_options[customerWorkouts[j]];
        OrderPair pair =std::make_pair(customer_id,currWorkout);
        orderList.push_back(pair);
        salary=salary+currWorkout.getPrice();
        std::cout << currCustumer->getName() + " Is Doing " + currWorkout.getName() << std::endl;
    }
}

void Trainer::openTrainer() {
    open = true;
}

void Trainer::closeTrainer() {
    open = false;
    capacity = capacity + customersList.size();;
    myDelete();
}

int Trainer::getSalary() {
    return salary;
}

bool Trainer::isOpen() {
    return open;
};
void Trainer::setSalary(int addSalary){
    salary=addSalary;

}

