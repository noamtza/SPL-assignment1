//
// Created by spl211 on 08/11/2021.

#include <algorithm>
#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id) :
        name(c_name),
        id(c_id) {}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}


bool Customer::compareTo(std::pair<int, int> &a, std::pair<int, int> &b) {//comparator
    if (a.first == b.first) {
        return a.second < b.second;
    } else
        return a.first<b.first;
}


SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> swtWorkouts;
    for (size_t i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == 2)
            swtWorkouts.push_back(workout_options[i].getId());
    }
    return swtWorkouts;
}

std::string SweatyCustomer::toString() const {
    return "swt " + getName() + ",";
}

SweatyCustomer* SweatyCustomer ::clone() {
    return new SweatyCustomer(*this);
}


CheapCustomer::CheapCustomer(std::string name, int id) : Customer{name, id} {}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<std::pair<int, int> > pairWorkout_options;
    std::vector<int> chpWorkouts;
    for (size_t i = 0; i < workout_options.size(); i++) {
            std::pair<int, int> curr(workout_options[i].getPrice(), workout_options[i].getId());
            pairWorkout_options.push_back(curr);
        }
    std::sort(pairWorkout_options.begin(), pairWorkout_options.end(), Customer::compareTo);
    chpWorkouts.push_back(pairWorkout_options[0].second);
    return chpWorkouts;
    }



std::string CheapCustomer::toString() const {
    return "chp " + getName() + ",";
}

CheapCustomer* CheapCustomer::clone() {
    return new CheapCustomer(*this);
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {}


std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<std::pair<int, int> > pairWorkout_options;
//copyWorkout_options = std::vector<Workout>();
    for (size_t i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == 0) {
            std::pair<int, int> curr(workout_options[i].getPrice(), workout_options[i].getId());
            pairWorkout_options.push_back(curr);
        }
    }
    std::sort(pairWorkout_options.begin(), pairWorkout_options.end(), Customer::compareTo);

    std::vector<int> mclWorkouts;
    for (int i = (int)pairWorkout_options.size()-1; i >= 0; i--) {
        mclWorkouts.push_back(pairWorkout_options[i].second);
    }

    return mclWorkouts;
}



std::string HeavyMuscleCustomer::toString() const {
    return "mcl " + getName() + ",";
}

HeavyMuscleCustomer *HeavyMuscleCustomer::clone() {
    return new HeavyMuscleCustomer(*this);
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> fbdWorkout;
    std::vector<std::pair<int, int> > pairWorkout_options;
    for (size_t i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == 2) {
            std::pair<int, int> curr(workout_options[i].getPrice(), workout_options[i].getId());
            pairWorkout_options.push_back(curr);
        }
    }
    std::sort(pairWorkout_options.begin(), pairWorkout_options.end(), Customer::compareTo);
    fbdWorkout.push_back(pairWorkout_options[0].second);
    std::vector<std::pair<int, int> > MixedPair;
    for (size_t i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == 1) {
            std::pair<int, int> curr(workout_options[i].getPrice(), workout_options[i].getId());
            MixedPair.push_back(curr);
        }
    }
    if(MixedPair.size()!=0) {
     std::sort(MixedPair.begin(), MixedPair.end(), Customer::compareTo);
        fbdWorkout.push_back(MixedPair[MixedPair.size() - 1].second);
    }
        std::vector<std::pair<int, int> > anaerobicPair;
        for (size_t i = 0; i < workout_options.size(); i++) {
            if (workout_options[i].getType() == 0) {
                std::pair<int, int> curr(workout_options[i].getPrice(), workout_options[i].getId());
                anaerobicPair.push_back(curr);
            }
        }
    std::sort(anaerobicPair.begin(), anaerobicPair.end(), Customer::compareTo);
    fbdWorkout.push_back(anaerobicPair[0].second);
    return fbdWorkout;

}

std::string FullBodyCustomer::toString() const {
    return "fbd " + getName() + ",";
};

FullBodyCustomer* FullBodyCustomer::clone() {
    return new FullBodyCustomer(*this);
}
