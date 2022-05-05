#include "../include/Action.h"
#include "../include/Customer.h"
#include "../include/Trainer.h"
#include "../include/Studio.h"

BaseAction::BaseAction():errorMsg(""),status() {}

ActionStatus BaseAction::getStatus() const {return status;}

void BaseAction::complete() {status=COMPLETED;}

std::string BaseAction::getErrorMsg() const {return errorMsg;}

void BaseAction::error(std::string errorMsg) {
    this->errorMsg=errorMsg;
    this->status=ERROR;
    std::cout<<errorMsg<<std::endl;
}

BaseAction::~BaseAction() {}


///OpenTrainer

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):BaseAction(), trainerId(id),customers(customersList),str("") {}

void OpenTrainer::act(Studio &studio) {
    Trainer * tr = studio.getTrainer(trainerId);
    if(tr==nullptr || tr->isOpen())     ///if there's no such trainer OR it's already open
        error("Trainer does not exist or is already open");
    else {
        str="open " +std::to_string(trainerId) +" ";
        tr->openTrainer();      ///first we open trainer
        for (Customer *cst: customers) {      ///now we add the customers
            tr->addCustomer(cst);
            str+=cst->getName() +","+ cst->toString() +" ";
        }
        //std::cout<<str<<std::endl;
        complete();
    }
}

std::string OpenTrainer::toString() const {
    if(getStatus()==ERROR)
        return("Error: Trainer does not exist or is already open");
    else{
        return str + " Completed";
    }
}

OpenTrainer *OpenTrainer::copy() {
    return(new OpenTrainer(*this));
}








///Order

Order::Order(int id):BaseAction(),trainerId(id),str("") {}

void Order::act(Studio &studio) {
    Trainer * tr = studio.getTrainer(trainerId);
    if(tr== nullptr || (!tr->isOpen()))    ///if there's no such trainer OR it's not open
        error("Error: Trainer does not exist or is not open");
    else {
        for (Customer * cst: tr->getCustomers()) {        //for each customer, gets his orders and inserts them into the order list
            std::vector<int> cs_orders = cst->order(studio.getWorkoutOptions());
            tr->order(cst->getId(), cs_orders, studio.getWorkoutOptions());
        }
        complete();
        //str+="Order " + std::to_string(trainerId);
        for(OrderPair op : tr->getOrders()) {
            str += tr->getCustomer(op.first)->getName() + " Is Doing " + op.second.getName()+'\n';
        }
        std::cout<<str<<std::endl;
    }
}

std::string Order::toString() const {
    if(getStatus()==ERROR)
        return  "Error: Trainer does not exist or is not open";
    else
        return str + " Completed";
}

Order *Order::copy() {
    return new Order(*this);
}





///MoveCustomer

MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTrainer(src),dstTrainer(dst),id(customerId),closed(false) {}

void MoveCustomer::act(Studio &studio) {
    Trainer * dst = studio.getTrainer(dstTrainer);         //destination trainer
    Trainer * src = studio.getTrainer(srcTrainer);          //source trainer
    Customer * cst = src->getCustomer(id);                //the customer
    if(dst==nullptr || src==nullptr || !dst->isOpen() || !src->isOpen() || cst==nullptr || (int)dst->getCapacity()==(int)dst->getCustomers().size())
        error("Error: can't move customer");
    else{
        src->removeCustomer(id);
        dst->addCustomer(cst);
        if(dst->getOrders().size()!=0) {        //adding ths customer's orders to the destination trainer
            std::vector<int> cs_orders = cst->order(studio.getWorkoutOptions());
            dst->order(cst->getId(), cs_orders, studio.getWorkoutOptions());
        }
        if(src->getCustomers().size()==0){            //closes trainer session if he doesn't have another customers
            src->closeTrainer();
            this->closed=true;
        }
        //std::cout<<"move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id)<<std::endl;
        complete();
    }
}

std::string MoveCustomer::toString() const {
    if(getStatus()==ERROR)
        return("Error: can't move customer");
    else
        return("move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id) + " Completed");
}

MoveCustomer *MoveCustomer::copy() {return new MoveCustomer(*this);}

bool MoveCustomer::getClosed() {return closed;}




///close

Close::Close(int id): BaseAction(), trainerId((int) id) {}

void Close::act(Studio &studio) {

    Trainer * tr = studio.getTrainer(trainerId);
    if(tr== nullptr || !tr->isOpen())
        error("Error: Trainer does not exist or is already closed");
    else{
        tr->setSalary(tr->getSalary());
        salary=tr->getSalary();
        tr->closeTrainer();
        complete();
        std::string str = "Trainer " + std::to_string((int)trainerId) +" closed. Salary " + std::to_string(tr->getSalary()) + "NIS";
        std::cout<<str<<std::endl;
    }
}


std::string Close::toString() const {
    if(getStatus()==ERROR)
        return ("Error: Trainer does not exist or is already closed");
    else
        return("Trainer " + std::to_string(trainerId) +" closed. Salary " + std::to_string(salary)) + "NIS Completed";
}

Close * Close::copy() {return new Close(*this);}



///CloseAll

CloseAll::CloseAll():BaseAction(), str("") {}

void CloseAll::act(Studio &studio) {
    for(int i=0;i<studio.getNumOfTrainers();i++){          //closes each trainer
        Trainer * tr = studio.getTrainer(i);
        if(tr->isOpen()){
            Close cls = Close(i);
            cls.act(studio);
            str+=cls.toString()+'\n';
        }
    }
    complete();
}

std::string CloseAll::toString() const {return str+"Completed";}

CloseAll *CloseAll::copy() {return new CloseAll(*this);}






///PrintWorkoutOptions

PrintWorkoutOptions::PrintWorkoutOptions():BaseAction() {}

void PrintWorkoutOptions::act(Studio &studio) {
    for(Workout wo : studio.getWorkoutOptions()){
        if(wo.getType()==ANAEROBIC)
            std::cout<< wo.getName() + ", " + std::to_string(wo.getPrice()) + ", Anaerobic"<<std::endl;
        else if(wo.getType()==MIXED)
            std::cout<< wo.getName() + ", " + std::to_string(wo.getPrice()) + ", Mixed"<<std::endl;
        if(wo.getType()==CARDIO)
            std::cout<< wo.getName() + ", " + std::to_string(wo.getPrice()) + ", Cardio"<<std::endl;
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const {return "PrintWorkoutOptions Completed";}

PrintWorkoutOptions *PrintWorkoutOptions::copy() {return new PrintWorkoutOptions(*this);}



///PrintTrainerStatus

PrintTrainerStatus::PrintTrainerStatus(int id):BaseAction(),trainerId(id) {}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer * tr = studio.getTrainer(trainerId);
    if(!tr->isOpen())
        std::cout<<"Trainer " + std::to_string(trainerId) + " status: closed"<<std::endl;
    else{            //trainer is open
        std::cout<<"Trainer " + std::to_string(trainerId) + " status: open"<<std::endl;
        std::cout<<"Customers:"<<std::endl;
        for(Customer * cst : tr->getCustomers())            //prints customers information
            std::cout<<std::to_string(cst->getId()) + " " + cst->getName()<<std::endl;
        for(OrderPair op : tr->getOrders())                  //print orders
            std::cout<<op.second.getName()+" "+std::to_string(op.second.getPrice())+"NIS "+std::to_string(op.first)<<std::endl;
        std::cout<<"Current Trainer's Salary: " + std::to_string(tr->getSalary())+"NIS"<<std::endl;
    }
    complete();
}

std::string PrintTrainerStatus::toString() const {return "PrintTrainerStatus Completed";}

PrintTrainerStatus *PrintTrainerStatus::copy() {return new PrintTrainerStatus(*this);}




///PrintActionLog

PrintActionsLog::PrintActionsLog():BaseAction() {}

void PrintActionsLog::act(Studio &studio) {
    for(BaseAction * act : studio.getActionsLog())
        std::cout<<act->toString()<<std::endl;
    complete();
}

std::string PrintActionsLog::toString() const {return "PrintActionsLog Completed";}

PrintActionsLog *PrintActionsLog::copy() {return new PrintActionsLog(*this);}



///BackupStudio
BackupStudio::BackupStudio():BaseAction() {}

void BackupStudio::act(Studio &studio) {
    extern Studio * backup;
    if(backup!=nullptr) {
        delete (backup);
    }
    backup= new Studio(studio);
    complete();
}

std::string BackupStudio::toString() const {return "Backup Completed";}

BackupStudio *BackupStudio::copy() {return new BackupStudio(*this);}



///RestoreStudio

RestoreStudio::RestoreStudio():BaseAction() {}

void RestoreStudio::act(Studio &studio) {
    if(backup== nullptr)
        error("Backup does not exist");
    else {
        studio = *backup;
        complete();
    }
}

std::string RestoreStudio::toString() const {
    if(getStatus()==ERROR)
        return "Backup does not exist";
    else
        return "Restore Completed";
}

RestoreStudio *RestoreStudio::copy() {return new RestoreStudio(*this);}