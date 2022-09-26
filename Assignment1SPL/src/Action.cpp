#include "../include/Trainer.h"
#include "../include/Action.h"
/*
 * BaseAction
 */

BaseAction::BaseAction():errorMsg(),status() {}
ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status= COMPLETED;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
void BaseAction::error(std::string errorMsg) {
    status=ERROR;
    this->errorMsg=errorMsg;
    std::cout<< getErrorMsg()<<std::endl;
}
BaseAction::~BaseAction() noexcept {}

/*
 * OpenTrainer
 */
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):trainerId(id),customers(customersList) {}

void OpenTrainer::act(Studio &studio) {
    if(studio.getNumOfTrainers()<=trainerId){
        error("Error:<Workout session does not exist or is already open>");
        return;}
    Trainer *currentT=studio.getTrainer(trainerId);
    if(currentT->isOpen()){
        error("Error:<Workout session does not exist or is already open>");
        return;}
    currentT->openTrainer();
    for(int i=0;i<currentT->getCapacity()&& (unsigned)i<customers.size();i++){
        currentT->addCustomer(customers[i]->clone());
    }
    complete();
}//end of OpenTrainer act

std::string OpenTrainer::toString() const {
    std::string customerString;
    for(int i=0; (unsigned)i<customers.size(); i++)
    {
        customerString = customerString+ " " + customers[i]->toString();
    }
    if(getStatus()==COMPLETED) // case action completed
    {
        return "open " + std::to_string(trainerId) +customerString +" Completed";
    }
    else //case action got error
    {
        return "open "+std::to_string(trainerId)+ customerString +" "+this->getErrorMsg(); //casting int for String
    }
}//end of toString

BaseAction *OpenTrainer::clone() {
    std::vector<Customer*> customerListCopy;
    for(int i=0; (unsigned)i<this->customers.size();i++)
    {
        customerListCopy.push_back(customers[i]->clone());
    }
    return new OpenTrainer(this->trainerId,customerListCopy);
}

OpenTrainer::~OpenTrainer() noexcept {
    for(int i=0; (unsigned)i<customers.size();i++)
    {
        delete customers[i];
        customers[i]=nullptr;
    }
    customers.clear();
}

/*
 * Order
 */
Order::Order(int id):trainerId(id) {}

void Order::act(Studio &studio) {
    if(studio.getNumOfTrainers()<=trainerId){
        error("Error:<Trainer does not exist or is not open>");
        return;}
    Trainer *currentT = studio.getTrainer(trainerId);//the wanted trainer
    if(!currentT->isOpen()){
        error("Error:<Trainer does not exist or is not open>");
        return;}
    std::vector<Customer*> currentC=currentT->getCustomers();//the customers of the trainer
    std::vector<Workout> workout_options = studio.getWorkoutOptions();//the workout options
    for(int i=0;i<currentT->getCapacity()&&(unsigned)i<currentC.size();i++){//goes through each customer and asks for his order then inserts the order to the trainer order list
        std::vector<int> desired=currentC[i]->order(workout_options);
        currentT->order(currentC[i]->getId(), desired,workout_options);
    }
    std::vector <OrderPair> toPrint = currentT->getOrders();//access the order list
    for(OrderPair op : toPrint){// for each order print it
        for(int i=0;(unsigned)i<currentC.size();i++){
            if(currentC[i]->getId()==op.first)//find the customer with the right id
                std::cout<< currentC[i]->getName()<< " is doing "<< op.second.getName()<<std::endl;
        }
    }
    complete();
}

std::string Order::toString() const {
    if(this->getStatus()==COMPLETED)
    {
        return "order "+std::to_string(this->trainerId)+" Completed";
    }
    else// case error
    {
        return "order "+ std::to_string(this->trainerId)+ this->getErrorMsg();
    }
}//end of Order toString

BaseAction *Order::clone() {
    return new Order(this->trainerId);
}

Order::~Order() noexcept {}

/*
 * MoveCustomer
 */

MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(src),dstTrainer(dst),id(customerId) {}

void MoveCustomer::act(Studio &studio) {
    if (studio.getNumOfTrainers() <= srcTrainer || studio.getNumOfTrainers() <= dstTrainer) {// check if trainers exists
        error("Cannot move customer");
        return;
    }
    Trainer *Tsrc=studio.getTrainer(srcTrainer);
    Trainer *Tdst=studio.getTrainer(dstTrainer);
    if (!Tsrc->isOpen() | !Tdst->isOpen()) {//checks if trainers are open
        error("Cannot move customer");
        return;
    }
    Customer *movingCustomer=Tsrc->getCustomer(id);
    if(movingCustomer== nullptr){
        error("Cannot move customer");
        return;
    }
    else{
        std::vector<int> desired=movingCustomer->order(studio.getWorkoutOptions());
        Tdst->addCustomer(movingCustomer);
        Tsrc->removeCustomer(id);
        Tdst->order(id,desired,studio.getWorkoutOptions());
    }
    complete();
}

std::string MoveCustomer::toString() const {
    if(this->getStatus()==COMPLETED)
    {
        return "move "+std::to_string(this->srcTrainer)+" "+std::to_string(this->dstTrainer)+" "+std::to_string(this->id)+" Completed";
    }
    else
    {
        return "move "+std::to_string(this->srcTrainer)+" "+std::to_string(this->dstTrainer)+" "+std::to_string(this->id)+this->getErrorMsg();
    }
}//end of toString MoveCustomer
BaseAction *MoveCustomer::clone() {
    return new MoveCustomer(srcTrainer,dstTrainer,id);
}

MoveCustomer::~MoveCustomer(){}

/*
 * Close
 */
Close::Close(int id):trainerId(id) {}

void Close::act(Studio &studio){
    /*
     ****need to make sure salary is accumulated!!!!******************************
     */
    if(studio.getNumOfTrainers()<=trainerId){ //case there's no such trainer in studio
        BaseAction::error("Error:<Trainer does not exist or is not open>");
        return;
    }
    Trainer *currentT= studio.getTrainer(trainerId); //Trainer to close
    if(!(*currentT).isOpen()){ //case trainer is already closed
        BaseAction::error("Error:<Trainer does not exist or is not open>");
        return;
    }
    //case there is no errors
    std::cout << "Trainer " << trainerId << " closed. Salary " << currentT->getSalary() << "NIS"<< std::endl;
    for(int i=0;(unsigned)i<(*currentT).getCustomers().size();i++)
    {
        delete (*currentT).getCustomers()[i];
        (*currentT).getCustomers()[i]=0;
    }
    (*currentT).getCustomers().clear(); //check with shoham if we shouldn't do it with removeCustomer
    (*currentT).closeTrainer();
    complete();
}//end of Close act

std::string Close::toString() const
{
    if(this->getStatus()==COMPLETED)
    {
        return "close "+std::to_string(this->trainerId)+" completed";
    }
    else //case action got error
    {
        return "close "+std::to_string(this->trainerId)+" "+(*this).getErrorMsg();
    }
}// end of Close toString

BaseAction *Close::clone() {
    return new Close(trainerId);
}

Close::~Close(){}

/*
 * CloseAll
 */
CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for(int i=0; i<studio.getNumOfTrainers(); i++)
    {
        Trainer *currentT = studio.getTrainer(i);
        if(currentT->isOpen())
        {
            Close(i).act(studio);
            //need to delete instance from heap
        }
    }
    complete();
}//end of CloseAll act

std::string CloseAll::toString() const {
    return "closeall Completed";
}//end of closeAll toString

BaseAction *CloseAll::clone() {
    return new CloseAll();
}

CloseAll:: ~CloseAll(){}
/*
 * PrintWorkoutOptions
 */
PrintWorkoutOptions::PrintWorkoutOptions() {}

void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> workouts = studio.getWorkoutOptions();
    std:: string type;
    for(int i=0; (unsigned)i<workouts.size(); i++)
    {
        if(workouts[i].getType()==0)
            type = "Anaerobic";
        if(workouts[i].getType()==1)
            type="Mixed";
        if(workouts[i].getType()==2)
            type="Cardio";
        std::cout <<workouts[i].getName()<<", "<< type <<", "<<workouts[i].getPrice()<<std::endl;
    }
    complete();
}//end of PrintWorkoutOptions

std::string PrintWorkoutOptions::toString() const {
    return "workout_options Completed";
}//end of PrintWorkoutOptions toString
BaseAction *PrintWorkoutOptions::clone() {
    return new PrintWorkoutOptions();
}

PrintWorkoutOptions::~PrintWorkoutOptions() noexcept {}

/*
 * PrintTrainerStatus
 */
PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id) {}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer *currentT= studio.getTrainer(trainerId);
    //case trainer session is closed
    if(!currentT->isOpen())
    {
        std::cout <<"Trainer "<<std::to_string(trainerId)<<" status: closed"<<std::endl;
        return;
    }
    //case trainer session is open
    std::vector<Customer*>  trainerCustomers = currentT->getCustomers();
    std::cout <<"Trainer "<<trainerId<<" status: open"<<std::endl; //print status
    std::cout <<"Customers:"<<std::endl;
    for(int i=0; (unsigned)i<trainerCustomers.size();i++)//print all trainer customers
    {
        std::cout <<trainerCustomers[i]->getId()<<" "<<trainerCustomers[i]->getName()<<std::endl;
    }
    std::cout <<"Orders:"<<std::endl;
    std::vector<OrderPair> oList = (*currentT).getOrders();
    for(int i =0; (unsigned)i<oList.size();i++)
    {
        std::string name = oList[i].second.getName();
        std::string price = std::to_string(oList[i].second.getPrice());
        std::string customer = std::to_string(oList[i].first);
        std::cout <<name+" "+price+" "+customer<<std::endl;
    }
    std::cout <<"Current Trainer's Salary: "<<currentT->getSalary()<<"NIS"<<std::endl;
    complete();
}//end of PrintTrainerStatus

std::string PrintTrainerStatus::toString() const {
    return "status "+ std::to_string((*this).trainerId)+" Completed";
}//end of PrintTrainerStatus toString
BaseAction *PrintTrainerStatus::clone() {
    return new PrintTrainerStatus(this->trainerId);
}

PrintTrainerStatus::~PrintTrainerStatus() noexcept {}

/*
 * PrintActionsLog
 */
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Studio &studio) {
    std::vector<BaseAction *> sLog = studio.getActionsLog(); //Studio actions log
    for(int i=0; (unsigned)i<sLog.size();i++)
    {
        std:: cout<<sLog[i]->toString()<<std::endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log Complete";
}//end of PrintActionsLog toString

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog();
}

PrintActionsLog::~PrintActionsLog() noexcept {}

/*
 *BackupStudio
 */
BackupStudio::BackupStudio() {}


BaseAction *BackupStudio::clone() {
    return new BackupStudio();
}

void BackupStudio::act(Studio &studio) {
    if(backup!= nullptr)
        delete backup;
    backup = new Studio(studio);
    complete();
}

std::string BackupStudio::toString() const {
    return "backup Complete";
}

BackupStudio::~BackupStudio() noexcept {}
/*
 *RestoreStudio
 */
RestoreStudio::RestoreStudio() {}

BaseAction *RestoreStudio::clone() {
    return new RestoreStudio();
}

void RestoreStudio::act(Studio &studio) {
    if(backup == nullptr){
        error("No backup available");
        return;
    }
    else{
        complete();
        studio = *backup;
    }

}

std::string RestoreStudio::toString() const {
    if(this->getStatus()==COMPLETED)
    {
        return "restore Completed";
    }
    else
    {
        return "restore "+this->getErrorMsg();
    }
}

RestoreStudio::~RestoreStudio() noexcept {}