#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id) {}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}
Customer::~Customer() noexcept {}
/*
 * SweatyCustomer
 */
SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> desired;
    //vector<Workout>::iterator wrktIter=workout_options.begin();
    //while(wrktIter.hasNext()){
    for(int i=0;(unsigned)i<workout_options.size();i++){
        Workout wrkt=workout_options[i];
        if(wrkt.getType()==CARDIO)
            desired.push_back(wrkt.getId());
    }
    return desired;
}

std::string SweatyCustomer::toString() const {
    return this->getName()+","+"swt";
}

Customer *SweatyCustomer::clone() {
    return new SweatyCustomer(this->getName(),this->getId());
}
SweatyCustomer::~SweatyCustomer() noexcept {}

/*
 * CheapCustomer
 */
CheapCustomer::CheapCustomer(std::string name, int id) :Customer(name,id){}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> desired;
    int minP=workout_options[0].getPrice(),minId=0;
    //std::iterator wrktIter=&workout_options.begin();
    //while(wrktIter.hasNext()){
    for(int i=0;(unsigned)i<workout_options.size();i++){
        Workout wrkt=workout_options[i];
        if(wrkt.getPrice()<minP){
            minP=wrkt.getPrice();
            minId=wrkt.getId();
        }
    }
    desired.push_back(minId);
    return desired;
}

std::string CheapCustomer::toString() const {
    return this->getName()+","+"chp";
}

Customer *CheapCustomer::clone() {
    return new CheapCustomer(this->getName(),this->getId());
}
CheapCustomer::~CheapCustomer() noexcept {}
/*
 * heavyMusclethingy
 */
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name,id) {}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> desired;
    std::vector<Workout> temp;//holds all the anerobics workouts so i can sort them by price.
    //std::iterator wrktIter = &workout_options.begin();
    //while (wrktIter.hasNext()) {
    for(int i=0;(unsigned)i<workout_options.size();i++){
        Workout wrkt = workout_options[i];
        if (wrkt.getType() == ANAEROBIC)//collect all the anerobics to temp
            temp.push_back(wrkt);
    }
    for(Workout w : temp){// for each workout in temp find its rightful position in desired.
        if(desired.empty())
            desired.push_back(w.getId());
        else{
            for (int i = 0; (unsigned)i < desired.size(); i++){
                if (w.getPrice() > workout_options[desired[i]].getPrice()){
                    desired.insert(desired.begin()+i,w.getId());
                        break;
                    }
                }
            }
    }
    return desired;
}

std::string HeavyMuscleCustomer::toString() const {
    return this->getName()+","+"mcl";
}

Customer *HeavyMuscleCustomer::clone() {
    return new HeavyMuscleCustomer(this->getName(),this->getId());
}
HeavyMuscleCustomer::~HeavyMuscleCustomer() noexcept {}
/*
 * FBD
 */

FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name,id) {}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    int minPCardio=INT16_MAX,maxPMixed=0,minPAnaerobic=INT16_MAX;
    int cardioId,mixedId,anaerobicId;
    std::vector<int> desired;
    //std::iterator<workout_options> wrktIter=&workout_options.begin();
    //while(wrktIter.hasNext()){
    for(int i=0;(unsigned)i<workout_options.size();i++){
        Workout wrkt=workout_options[i];
        if(wrkt.getType()==CARDIO)
            if(wrkt.getPrice()<minPCardio){
                minPCardio=wrkt.getPrice();
                cardioId=wrkt.getId();}
        if(wrkt.getType()==MIXED)
            if(wrkt.getPrice()>maxPMixed){
                maxPMixed=wrkt.getPrice();
                mixedId=wrkt.getId();}
        if(wrkt.getType()==ANAEROBIC)
            if(wrkt.getPrice()<minPAnaerobic){
                minPAnaerobic=wrkt.getPrice();
                anaerobicId=wrkt.getId();}
    }

    desired.push_back(cardioId);
    desired.push_back(mixedId);
    desired.push_back(anaerobicId);
    return desired;
}

std::string FullBodyCustomer::toString() const {
    return this->getName()+","+"swt";
}

Customer *FullBodyCustomer::clone() {
    return new FullBodyCustomer(this->getName(),this->getId());
}
FullBodyCustomer::~FullBodyCustomer() noexcept {}