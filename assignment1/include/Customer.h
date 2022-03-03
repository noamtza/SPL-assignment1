#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

class Customer {
public:
    Customer(std::string c_name, int c_id);

    virtual std::vector<int> order(const std::vector<Workout> &workout_options) = 0;

    virtual std::string toString() const = 0;

    virtual Customer* clone() =0;//we add


    std::string getName() const;

    int getId() const;

    static bool compareTo( std::pair<int, int> &a, std::pair<int, int> &b);

  //  std::vector<std::pair<int, int>  >helperSort(const std::vector<Workout> &workout_options);//we add
//    virtual ~Customer();
virtual ~Customer()=default;


private:
    const std::string name;
    const int id;
};



class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);

    std::vector<int> order(const std::vector<Workout> &workout_options);

    std::string toString() const;
    SweatyCustomer* clone() ;//we add

private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    CheapCustomer* clone() ;
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    HeavyMuscleCustomer* clone() ;
private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    FullBodyCustomer* clone() ;
private:
};

#endif