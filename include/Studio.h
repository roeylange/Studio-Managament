#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


//Forward declaration
class BaseAction;


//Forward declaration
class Trainer;

class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void setTrainer(Trainer * tr);                //we added

    virtual ~Studio();                           //destructor
    Studio(const Studio &other);                 //copy constructor
    Studio(Studio &&other);                      //move constructor
    Studio& operator=(const Studio &other);      //copy assignment operator
    Studio& operator=(Studio &&other);           //move assignment operator
private:
    bool open;
    int cus_count;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};


#endif