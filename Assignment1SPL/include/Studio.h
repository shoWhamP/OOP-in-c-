#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include <iostream>
#include <fstream>
#include <algorithm>
class BaseAction;

class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    //Rule of 5*********
    virtual ~Studio();
    Studio(const Studio &other);
    const Studio& operator=(const Studio &other);
    Studio( Studio &&other);
    const Studio& operator=(Studio &&other);


private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    //additional fields
    int customersId=0;//tracking number of customers and numbers them
};

#endif
