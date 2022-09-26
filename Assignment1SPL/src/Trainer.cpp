#include "../include/Trainer.h"

    Trainer::Trainer(int t_capacity){
        open=false;
        salary=0;
        capacity=t_capacity;
        //trainerId=static_id;
       // stat_id++;
    }
    int Trainer::getCapacity() const {
        return capacity;
    }


void Trainer::addCustomer(Customer *customer) {
    if(customersList.size()<(unsigned)capacity-1)
        customersList.push_back(customer);
}
    void Trainer::removeCustomer(int id) {
    std::vector<Customer*> customer_update;
    for(int i=0;(unsigned)i<customersList.size();i++){
        if(customersList[i]->getId()!=id)
            customer_update.push_back(customersList[i]);
        else
        customersList[i]=nullptr;
    }
    customersList.clear();
    customersList=customer_update;
    customer_update.clear();
    std::vector<OrderPair> order_update;
    for(int i=0;(unsigned)i<orderList.size();i++){
        if(orderList[i].first!=id) {
            order_update.push_back(orderList[i]);
        }
        else{
            salary-=orderList[i].second.getPrice();
        }
    }
    orderList.clear();
    orderList=std::vector<OrderPair>(order_update);
    if(customersList.size()==0)
        this->closeTrainer();

}
    Customer *Trainer::getCustomer(int id) {
    for(int i=0; (unsigned)i<customersList.size();i++){
        if(customersList[i]->getId()==id)
            return customersList[i];
    }
    /*Customer c= nullptr;
    for(Customer cstmr : customersList){
        if(cstmr->getId()==id)
            c=cstmr;
    }*/
    //return c;
    return nullptr;
}
    std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}
    void Trainer::order(const int customer_id, const std::vector<int> workout_ids,
                        const std::vector<Workout> &workout_options) {
    for(int orderId : workout_ids){
        OrderPair order={customer_id,workout_options[orderId]};
        salary+=order.second.getPrice();
        orderList.push_back(order);
    }
    }
    void Trainer::openTrainer() {
        open = true;
    }

    void Trainer::closeTrainer(){
        open=false;
    }

    int Trainer::getSalary() {
        return salary;
    }



    bool Trainer::isOpen() {
        return open;
    }

    Trainer::~Trainer() {
    if (!customersList.empty()) {
        for (int i = 0; (unsigned)i < customersList.size(); i++) {
            delete customersList[i];
            customersList[i] = nullptr;
        }
    }
    customersList.clear();
}
    Trainer::Trainer(const Trainer &other) {
        this->open=other.open;
        for(int i=0;(unsigned)i<other.customersList.size();i++){

            this->customersList.push_back(other.customersList[i]->clone());
        }
        this->capacity=other.capacity;
        for(int i=0;(unsigned)i<other.orderList.size();i++){
            OrderPair copy={other.orderList[i].first , other.orderList[i].second};
            this->orderList.push_back(copy);
        }
       // this->trainerId=other.trainerId;
        this->salary=other.salary;
}

const Trainer &Trainer::operator=(const Trainer &other) {
    if(this!=&other) {
        this->capacity = other.capacity;
        this->salary = other.salary;
        this->open = other.open;
        //this->trainerId=other.trainerId;
        if (!customersList.empty()) {
            for (int i = 0; (unsigned) i < customersList.size(); i++) {
                delete customersList[i];
                customersList[i] = nullptr;
            }
        }
        this->customersList.clear();
        for (int i = 0; (unsigned) i < other.customersList.size(); i++) {
            this->customersList.push_back(other.customersList[i]->clone());
        }
        this->orderList.clear();
        for (int i = 0; (unsigned) i < other.orderList.size(); i++) {
            this->orderList.push_back(other.orderList[i]);
        }
    }
    return *this;
}
    Trainer::Trainer(Trainer &&other) {
    this->capacity=other.capacity;
    this->open=other.open;
    this->salary=other.salary;
    //this->trainerId=other.trainerId;
    this->customersList=std::move(other.customersList);
    this->orderList=std::move(other.orderList);

}

const Trainer &Trainer::operator=(Trainer &&other) {
    this->capacity=other.capacity;
    this->open=other.open;
    this->salary=other.salary;
    //this->trainerId=other.trainerId;
    if (!customersList.empty()) {
        for (int i = 0; (unsigned)i < customersList.size(); i++) {
            delete customersList[i];
            customersList[i] = nullptr;
        }
    }
    this->customersList.clear();
    this->customersList=std::move(other.customersList);
    this->orderList.clear();
    this->orderList=std::move(other.orderList);
    return *this;
}

