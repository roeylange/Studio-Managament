#include <fstream>
#include <string>
#include "../include/Studio.h"

Studio::Studio(){
    open=false;
    cus_count=0;
}

///copy constructor
Studio::Studio(const Studio &other) {
    open=other.open;
    cus_count=other.cus_count;
    for(Trainer * tr : other.trainers){
        trainers.push_back(new Trainer(*tr));
    }
    for(BaseAction * ba : other.actionsLog){
        actionsLog.push_back(ba->copy());
    }
    for(Workout wo : other.workout_options){
        workout_options.push_back(Workout(wo.getId(),wo.getName(),wo.getPrice(),wo.getType()));
    }
}

///move constructor
Studio::Studio(Studio &&other) {

    ///assign the new data
    for(Trainer * tr : other.trainers)
        trainers.push_back(tr);
    other.trainers.clear();

    for(BaseAction * ba : other.actionsLog)
        actionsLog.push_back(ba);
    other.actionsLog.clear();

    for(Workout wo : other.workout_options)
        workout_options.push_back(wo);
    other.workout_options.clear();

    //other=nullptr;
}

///copy assignment operator
Studio &Studio::operator=(const Studio &other) {
    if(this!=&other) {
        ///first we clear the previous data
        for (Trainer *tr: trainers)
            delete (tr);
        trainers.clear();

        for (BaseAction *ba: actionsLog)
            delete ba;
        actionsLog.clear();

        workout_options.clear();


        ///then we assign the new data
        for (Trainer *tr: other.trainers) {
            trainers.push_back(new Trainer(*tr));
        }
        for (BaseAction *ba: other.actionsLog) {
            actionsLog.push_back(ba->copy());
        }
        for (Workout wo: other.workout_options) {
            workout_options.push_back(Workout(wo.getId(), wo.getName(), wo.getPrice(), wo.getType()));
        }
    }
    return *this;
}

///move assignment operator
Studio &Studio::operator=(Studio &&other) {
    if(this!=&other) {
        ///first we clear the previous data
        for (Trainer *tr: trainers)
            delete (tr);
        trainers.clear();

        for (BaseAction *ba: actionsLog)
            delete ba;
        actionsLog.clear();

        workout_options.clear();

        ///then we assign the new data
        for (Trainer *tr: other.trainers)
            trainers.push_back(tr);
        other.trainers.clear();

        for (BaseAction *ba: other.actionsLog)
            actionsLog.push_back(ba);
        other.actionsLog.clear();

        for (Workout wo: other.workout_options)
            workout_options.push_back(wo);
        other.workout_options.clear();
    }
    return *this;
}

Studio::~Studio() {
    for(Trainer * tr : trainers)
        delete(tr);
    trainers.clear();

    for(BaseAction * ba : actionsLog)
        delete ba;
    actionsLog.clear();

    workout_options.clear();

}

Trainer *Studio::getTrainer(int tid) {
    for(Trainer * tr : trainers) {
        if (tr->getId() == tid)
            return tr;
    }
    return nullptr;
}

void Studio::setTrainer(Trainer *tr) {this->trainers.push_back(tr);}

int Studio::getNumOfTrainers() const {return trainers.size();}

std::vector<Workout> &Studio::getWorkoutOptions() {return workout_options;}

const std::vector<BaseAction *> &Studio::getActionsLog() const {return actionsLog;}



void Studio::start() {
    this->open=true;
    std::cout<<"Studio is now open!"<<std::endl;
    std::string str="";     ///here we will store the commands gotten from the user
    std::string command=""; ///this variable will store the actual command name
    int index=-1;
    while(str!="exit"){
        std::getline(std::cin,str);
        index=str.find(' ');
        command=str.substr(0,index);      ///we use the ' ' to crop the command's name
        str=str.substr(index+1,str.length()-index);       ///we now remove the command's name since we already saved it in another str
        if(command=="open"){
            std::string cst="";     ///here we store the customer name
            std::string cst_type="";        ///here we store it's type
            int trID=std::stoi(str.substr(0,str.find(' ')));        ///that's the trainer's ID
            str=str.substr(str.find(' ')+1);
            std::vector<Customer*>  vec =  std::vector<Customer*>();     ///this will be the customer vector
            while((getTrainer(trID)!=nullptr) && (!getTrainer(trID)->isOpen()) && str.find(',')!=std::string::npos && (int)this->getTrainer(trID)->getCapacity() != (int)vec.size()){
                cst=str.substr(0,str.find(','));    ///customer
                if(str.find(' ')==std::string::npos) {   ///trim
                    cst_type = str.substr(str.find(',') + 1);  ///type
                    str = "";     ///trim
                }
                else {                ///trim
                    cst_type = str.substr(str.find(',') + 1, str.find(' '));  ///type
                    str = str.substr(str.find(' ') + 1);         ///trim
                }
                if(cst_type.substr(0,1)=="s"){
                    SweatyCustomer * customer= new SweatyCustomer(cst,this->cus_count);
                    vec.push_back(customer);
                    this->cus_count=this->cus_count+1;
                }
                else if(cst_type.substr(0,1)=="m"){
                    HeavyMuscleCustomer * customer= new HeavyMuscleCustomer(cst,this->cus_count);
                    vec.push_back(customer);
                    this->cus_count=this->cus_count+1;
                }
                else if(cst_type.substr(0,1)=="c"){
                    CheapCustomer * customer= new CheapCustomer(cst,this->cus_count);
                    vec.push_back(customer);
                    this->cus_count=this->cus_count+1;
                }
                else {
                    FullBodyCustomer * customer= new FullBodyCustomer(cst,this->cus_count);
                    vec.push_back(customer);
                    this->cus_count=this->cus_count+1;
                }
            }
            OpenTrainer * open= new OpenTrainer(trID,vec);         //opens the trainer's session
            open->act(*this);
            this->actionsLog.push_back(open);
        }
        else if(command=="order"){
            int trID=std::stoi(str);
            if(this->getTrainer(trID)->getOrders().empty()) {
                Order * ord = new Order(trID);
                ord->act(*this);
                this->actionsLog.push_back(ord);
            }
            else                                               //prints the orders, if the order action has already been made
                for(OrderPair op : this->getTrainer(trID)->getOrders())
                    std::cout<<this->getTrainer(trID)->getCustomer(op.first)->getName() +" Is Doing " + op.second.getName()<<std::endl;
            str="";

        }
        else if(command=="move"){   ///src,dst,cus
            int srcID=std::stoi(str.substr(0,str.find(' ')));
            str=str.substr(str.find(' ')+1);
            int dstID=std::stoi(str.substr(0,str.find(' ')));
            str=str.substr(str.find(' ')+1);
            int cID=std::stoi(str);
            MoveCustomer * ord=new MoveCustomer(srcID,dstID,cID);
            ord->act(*this);
            str="";
            this->actionsLog.push_back(ord);
            if(ord->getClosed()) {                //closes the src trainer's session, if he has no customers left
                this->actionsLog.push_back(new Close(srcID));
                std::cout<<"Trainer " + std::to_string(srcID) +" closed. Salary " + std::to_string(this->getTrainer(srcID)->getSalary()) + "NIS"<<std::endl;
            }
        }
        else if(command=="close"){
            int trID=std::stoi(str);
            Close * ord=new Close(trID);
            ord->act(*this);
            str="";
            this->actionsLog.push_back(ord);
        }
        else if(command=="workout_options"){
            PrintWorkoutOptions * ord= new PrintWorkoutOptions();
            ord->act(*this);
            str="";
            this->actionsLog.push_back(ord);
        }
        else if(command=="status"){
            int trID=std::stoi(str);
            PrintTrainerStatus * ord= new PrintTrainerStatus(trID);
            ord->act(*this);
            str="";
            this->actionsLog.push_back(ord);
        }
        else if(command=="log"){
            PrintActionsLog * ord=new PrintActionsLog();
            ord->act(*this);
            str="";
            this->actionsLog.push_back(ord);
        }
        else if(command=="backup"){
            BackupStudio * ord=new BackupStudio();
            this->actionsLog.push_back(ord);
            ord->act(*this);
            str="";
        }
        else if(command=="restore"){
            RestoreStudio * ord= new RestoreStudio();
            ord->act(*this);
            str="";
            this->actionsLog.push_back(ord);
        }
        else if(command=="closeall"){
            CloseAll ord=CloseAll();
            ord.act(*this);
            str="";
            break;
        }
        index=-1;
    }
}


Studio::Studio(const std::string &configFilePath) {
    open=false;
    cus_count=0;
    actionsLog=std::vector<BaseAction*>();
    trainers=std::vector<Trainer*>();
    workout_options=std::vector<Workout>();
    std::ifstream my_file (configFilePath);
    std::string str="";
    //std::getline(my_file,str);
    bool firstline=false,secondline=false;
    std::string word="";
    std::string name="";       //name of workout
    std::string type="";       //type of workout
    int price=0;
    int cap=0;
    while(std::getline(my_file,str)) {
        if (str != "" && str.find("#") != 0) {
            if (!firstline) {            //line of NumOfTrainers
                firstline = true;
            }
            else if (!secondline)  {     //line of capacities
                for (auto let: str) {
                    if (let != ',')
                        word += let;
                    else {
                        cap = std::stoi(word);
                        Trainer *tr = new Trainer(cap, this);  //creates the trainers
                        tr->setSalary(0);
                        word = "";
                    }
                }
                cap = std::stoi(word);
                Trainer *tr = new Trainer(cap, this);          //creates the last trainer
                tr->setSalary(0);
                word = "";
                secondline = true;
            }
            else {                   //line of Workout
                for (auto let: str) {
                    if (let != ',')
                        word += let;
                    else if (name == "") {
                        name = word;
                        word = "";
                    } else if (type == "") {
                        type = word.substr(1,word.length()-1);
                        word = "";
                    }
                }
                price = std::stoi(word);
                word = "";
                //creates each workout based on it's type
                if (type == "Anaerobic") {
                    Workout wrk =  Workout(this->workout_options.size(), name, price, ANAEROBIC);
                    workout_options.push_back(wrk);
                } else if (type == "Mixed") {
                    Workout wrk =  Workout(this->workout_options.size(), name, price, MIXED);
                    workout_options.push_back(wrk);
                } else {
                    Workout wrk =  Workout(this->workout_options.size(), name, price, CARDIO);
                    workout_options.push_back(wrk);
                }
                type="";
                name="";
            }
        }
    }
};
