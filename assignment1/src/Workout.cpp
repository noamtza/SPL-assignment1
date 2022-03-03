//
// Created by spl211 on 08/11/2021.
//
#include "Workout.h"
//class Workout{

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type) :
        id(w_id),
        name(w_name),
        price(w_price),
        type(w_type) {}

int Workout::getId() const {
    return this->id;
}

std::string Workout::getName() const {
    return name;
}

int Workout::getPrice() const {
    return price;
}

WorkoutType Workout::getType() const {
    return type;
}

std::string Workout::toString() {
    if (getType() == 0) {
        return name + ", ANAEROBIC, " + std::to_string(price);
    } else if (getType() == 1) {
        return name + ", MIXED, " + std::to_string(price);;
    } else {
        return name + ", CARDIO, " + std::to_string(price);
    };
}

Workout & Workout::operator=(const Workout&other){
    return *this;
}
