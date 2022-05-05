//
// Created by spl211 on 23/11/2021.
//

#include <vector>
#include "../include/Trainer.h"



Trainer::Trainer(int t_capacity): capacity(t_capacity){}

Trainer::Trainer(int t_capacity, Studio *M_studio): id(M_studio->getNumOfTrainers()),capacity(t_capacity),studio(M_studio)  {
    M_studio->setTrainer(this);
    open=false;
}

//copy constructor
Trainer::Trainer(const Trainer &other){
    id=other.id;
    open=other.open;
    capacity=other.capacity;
    salary=other.salary;

    this->studio = other.studio;
    for(int i=0;i<(int)other.customersList.size();i++){          //creates new and add the other's customers
        Customer* tmp = other.customersList[i]->copyCustomer();
        this->customersList.emplace_back(tmp);
    }
    for(int i=0;i<(int)other.orderList.size();i++){              //creates new and add the other's orders
        orderList.push_back(OrderPair (other.orderList[i].first,other.orderList[i].second));
    }
}

//move constructor
Trainer::Trainer(Trainer &&other) {
    id=other.id;
    open=other.open;
    capacity=other.capacity;
    salary=other.salary;
    studio=other.studio;


    for(int i=0;i<(int)other.customersList.size();i++){
        this->customersList.push_back(other.customersList[i]);
    }

    for(int i=0;i<(int)other.orderList.size();i++){
        this->orderList.push_back((other.orderList[i]));
    }
    other.customersList.clear();
    other.orderList.clear();
}

//copy assignment operator
Trainer &Trainer::operator=(const Trainer &other) {
    if(this!=&other){
        this->id=other.id;
        this->open = other.open;
        this->capacity = other.capacity;
        this->salary = other.salary;
        this->studio = new Studio(*other.studio);    //using copy-constructor of studio

        for(int i=0;i<(int)customersList.size();i++){     //deep-deleting the customers
            delete(customersList[i]);
            customersList[i] = nullptr;
        }
        if(!orderList.empty())                      //clearing the order list
            orderList.clear();

        for(int i=0;i<(int)other.customersList.size();i++){      //adding the other's customers
            Customer* customer = other.customersList[i]->copyCustomer();
            customersList.emplace_back(customer);
        }

        for(int i=0;i<(int)other.orderList.size();i++){          //adding the other's orders
            OrderPair op = OrderPair(other.orderList[i].first,other.orderList[i].second);
            orderList.emplace_back(op);
        }
    }
    return *this;
}

//move assignment operator
Trainer &Trainer::operator=(Trainer &&other) {
    if(this!=&other){
        this->id = other.id;
        this->open = other.open;
        this->capacity = other.capacity;
        this->salary = other.salary;
        this->studio = new Studio(*(other.studio));
        for(int i=0;i<(int)customersList.size();i++){     //deep-deleting the customers
            delete(customersList[i]);
            customersList[i] = nullptr;
        }
        if(!orderList.empty())                      //clearing the order list
            orderList.clear();
        customersList = std::move(other.customersList);
        orderList = std::move(other.orderList);
        other.customersList.clear();
        other.orderList.clear();
    }
    return *this;
}

//destructor
Trainer::~Trainer() {
    for(int i=0;i<(int)customersList.size();i++){
        delete(customersList[i]);
        customersList[i]=nullptr;
    }
    customersList.clear();
    orderList.clear();
}

int Trainer::getCapacity() const {
    return this->capacity;
}

void Trainer::addCustomer(Customer *customer) {
    this->customersList.push_back(customer);
}

void Trainer::removeCustomer(int id) {
    int cus_index = -1;
    for(int i=0;i<(int)customersList.size();i++)            //searching for the customer in customersList
        if(customersList[i]->getId()==id)
            cus_index = i;
    this->customersList.erase(customersList.cbegin()+cus_index);    //removing the customer
    std::vector<OrderPair> temp = std::vector<OrderPair>();     //keeps only orders not belong to the customer
    for(int i=0;i<(int)orderList.size();i++){
        if(orderList[i].first!=id)
            temp.push_back(orderList[i]);
    }
    orderList.clear();           //clears the original orders list
    for(int i=0;i<(int)temp.size();i++)
        orderList.push_back(temp[i]);
}

Customer *Trainer::getCustomer(int id) {
    for(int i=0;i<(int)customersList.size();i++){
        if(customersList[i]->getId()==id)
            return customersList[i];
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers() {
    return this->customersList;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return this->orderList;
}

void Trainer::openTrainer() {
    this->open = true;
}

void Trainer::closeTrainer() {
    this->open = false;
    for(Customer * cst : customersList)
        delete(cst);
    orderList.clear();
    customersList.clear();

}

bool Trainer::isOpen() {
    return this->open;
}

int Trainer::getId() {
    return this->id;
}

void Trainer::setSalary(int sal) {
    this->salary=sal;
}

int Trainer::getSalary() {
    int sum = this->salary;
    for(int i=0;i<(int)orderList.size();i++)
        sum += orderList[i].second.getPrice();
    return sum;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    for(int i=0;i<(int)workout_ids.size();i++){         //searches for the wanted workouts, based on the customers demands
        for(int j=0;j<(int)workout_options.size();j++){
            if(workout_ids[i]==workout_options[j].getId())
                orderList.push_back(OrderPair(customer_id,workout_options[j]));
        }
    }
}