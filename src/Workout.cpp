#include <vector>
#include <string>
#include "../include/Workout.h"

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):id(w_id),name(w_name),price(w_price),type(w_type) {}

int Workout::getId() const {
    return this->id;
}

int Workout::getPrice() const {
    return this->price;
}

std::string Workout::getName() const {
    return this->name;
}

WorkoutType Workout::getType() const {
    return this->type;
}