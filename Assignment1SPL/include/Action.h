#ifndef ACTION_H_
#define ACTION_H_
#include "Studio.h"
#include <string>
#include <iostream>
#include "Customer.h"


enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;
extern Studio* backup;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    //additional functions
    virtual BaseAction* clone()=0;
    virtual ~BaseAction();
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    void act(Studio &studio);
    std::string toString() const;
    //additional functions
    BaseAction* clone();
    virtual ~OpenTrainer();
private:
	const int trainerId;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio &studio);
    std::string toString() const;
    //additional functions
    BaseAction* clone();
    virtual ~Order();
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio &studio);
    std::string toString() const;
    //additional functions
    BaseAction* clone();
    virtual ~MoveCustomer();
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Studio &studio);
    std::string toString() const;
    //additional functions
    BaseAction* clone();
    virtual ~Close();
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string toString() const;
    //additional functions
    BaseAction* clone();
    virtual ~CloseAll();
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
    //additional functions
    BaseAction* clone();
    virtual ~PrintWorkoutOptions();
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio &studio);
    std::string toString() const;
    //additional functions
    BaseAction* clone();
    virtual ~PrintTrainerStatus();
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
    //additional functions
    BaseAction* clone();
    virtual ~PrintActionsLog();
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
    //additional functions
    BaseAction* clone();
    virtual ~BackupStudio();
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
    //additional functions
    BaseAction* clone();
virtual ~RestoreStudio();

};


#endif