#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
class Trainer;
class Action;

class Studio{
public:
	Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    Studio( const Studio &other);//copy c
    Studio(Studio &&other);//move c
    const Studio& operator=(const Studio &other);//copy operator =
    const Studio& operator=(Studio &&other);//move assignment
    virtual ~Studio();//destructor
    BaseAction* curAction(std::string input);
    void  myDelete();

private:
    bool open;
    int counterCusId ;//we add
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;


};

#endif