#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Workout.h"
#include "Studio.h"
#include "Customer.h"

typedef std::pair<int, Workout> OrderPair;

class Studio;

class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();        //we added
    int getSalary();
    void setSalary(int sal);    //we added
    bool isOpen();
    int getId();      //we added

    virtual ~Trainer();                            //destructor, we added
    Trainer(int t_capacity,Studio * M_studio);     //constructor with studio argument, we added
    Trainer(const Trainer &other);                 //copy_constructor, we added
    Trainer(Trainer &&other);                      //move_constructor, we added
    Trainer& operator=(const Trainer &other);      //copy_assignment_operator, we added
    Trainer& operator=(Trainer &&other);           //move_assignment_operator, we added


private:
    int salary;                //we added
    int id;                    //we added
    int capacity;
    bool open;
    Studio * studio;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif