//
// Created by spl211 on 23/11/2021.
//

#include <vector>
#include <string>
#include "../include/Customer.h"
#include <bits/stdc++.h>

///constructor
Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id) {}

///destructor
Customer::~Customer() {}



std::string Customer::getName() const {
    return this->name;
}

int Customer::getId() const {
    return this->id;
}



///SweatyCustomer
SweatyCustomer::SweatyCustomer(std::string name, int id):Customer(name,id) {}

std::string SweatyCustomer::toString() const {
    return "swt";
}

SweatyCustomer *SweatyCustomer::copyCustomer() {
    SweatyCustomer* customer = new SweatyCustomer(this->getName(),this->getId());
    return customer;
}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> vec = std::vector<int>();
    for(int i=0;i<(int)workout_options.size();i++){         //adds all the Cardio workouts
        if(workout_options[i].getType()==CARDIO)
            vec.push_back(workout_options[i].getId());
    }
    return vec;
}

///CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id) {}

std::string CheapCustomer::toString() const {
    return "chp";
}

CheapCustomer *CheapCustomer::copyCustomer() {
    CheapCustomer* customer = new CheapCustomer(this->getName(),this->getId());
    return customer;
}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> vec = std::vector<int>();
    int cheapest_id = workout_options[0].getId();            //id of cheapest workout
    int cheapest_price = workout_options[0].getPrice();     //price of cheapest workout
    for(int i=1;i<(int)workout_options.size();i++){          //checks all the other orders and compare their price
        if(workout_options[i].getPrice()<cheapest_price){
            cheapest_price = workout_options[i].getPrice();
            cheapest_id = workout_options[i].getId();
        }
    }
    vec.push_back(cheapest_id);
    return vec;
}

///HeavyMuscleCustomer
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name,id) {}

std::string HeavyMuscleCustomer::toString() const {
    return "mcl";
}

HeavyMuscleCustomer *HeavyMuscleCustomer::copyCustomer() {
    HeavyMuscleCustomer* customer = new HeavyMuscleCustomer(this->getName(),this->getId());
    return customer;
}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> vec = std::vector<int>();
    std::vector<std::pair<int,int>> temp_vec = std::vector<std::pair<int,int>>();   //price, id
    for(int i=0;i<(int)workout_options.size();i++){            //first, finding all the ANAEROBIC workouts
        if(workout_options[i].getType()==ANAEROBIC)
            //we have inserted the value in a way that will allow us to sort from the most expensive to the cheapest,and keep the order of the ID's
            temp_vec.push_back(std::pair<int,int>(1000000-workout_options[i].getPrice(),workout_options[i].getId()));
    }
    std::sort(temp_vec.begin(),temp_vec.end());       //sorts from the most expensive to the cheapest
    for(int i=0;i<(int)temp_vec.size();i++)
        vec.push_back(temp_vec[i].second);
    return vec;
}


///FullBodyCustomer
FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name,id) {}

std::string FullBodyCustomer::toString() const {
    return "fbd";
}

FullBodyCustomer *FullBodyCustomer::copyCustomer() {
    FullBodyCustomer* customer = new FullBodyCustomer(this->getName(),this->getId());
    return customer;
}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> vec = std::vector<int>();
    int cheap_cardio_price=2147483647, cheap_cardio_id=-1;
    int expensive_mix_price=-1, expensive_mix_id=-1;
    int cheap_anaerobic_price=2147483647, cheap_anaerobic_id=-1;
    for(int i=0;i<(int)workout_options.size();i++){    //searches for cheapest cardio, expensive mixed, cheapest anaerobic
        if(workout_options[i].getType()==CARDIO && workout_options[i].getPrice()<cheap_cardio_price){
            cheap_cardio_price=workout_options[i].getPrice();
            cheap_cardio_id=workout_options[i].getId();
        }
        else if(workout_options[i].getType()==MIXED && workout_options[i].getPrice()>expensive_mix_price){
            expensive_mix_price = workout_options[i].getPrice();
            expensive_mix_id = workout_options[i].getId();
        }
        else if(workout_options[i].getType()==ANAEROBIC && workout_options[i].getPrice()<cheap_anaerobic_price){
            cheap_anaerobic_price = workout_options[i].getPrice();
            cheap_anaerobic_id = workout_options[i].getId();
        }
    }
    if(cheap_cardio_id==-1 || expensive_mix_id==-1 || cheap_anaerobic_id==-1)
        return vec;
    else {
        vec.push_back(cheap_cardio_id);
        vec.push_back(expensive_mix_id);
        vec.push_back(cheap_anaerobic_id);
        return vec;
    }
}