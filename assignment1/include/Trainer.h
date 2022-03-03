#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);//constructor
    Trainer(const Trainer& other);//copy constructor
    const Trainer& operator=(const Trainer &other);//copy assignment
    Trainer(Trainer &&other); //move constructor
    const Trainer& operator=(Trainer &&other);//move assignment
    virtual ~Trainer();//destractor
    int getCapacity() const;
    void myDelete();//we add
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    void setCapacity( int capacity);
    bool isFirstOpen();//we add
    void setFirstOpen();//we add
    void setSalary(int addSalary);//we add
    Trainer* clone();
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    int salary;//we add
    bool firstOpen; //we add the trainer open at least once.(openTrainer)
};


#endif