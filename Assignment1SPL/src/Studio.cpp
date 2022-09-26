#include "../include/Studio.h"


   Studio::Studio(const std::string &configFilePath) {
        std::ifstream cFile(configFilePath);
        if (cFile.is_open())
        {
            open=true;
            std::string line;
            getline(cFile, line);
            getline(cFile, line);
            int numOfTrainers=std::stoi(line);
            getline(cFile, line);
            getline(cFile, line);
            getline(cFile, line);
            int start=0 , stop=0;
            for(int i=0;i<numOfTrainers;i++){
                while(line.length()>(unsigned)stop && line[stop]!=',')
                    stop++;
                trainers.push_back(new Trainer (std::stoi(line.substr(start,stop)))) ;//greetings shoham and amit from the future, you need to update this line and the constructor to include salary and index
                stop++;
                start=stop;
            }
            getline(cFile, line);
            getline(cFile, line);
            getline(cFile, line);
            start=0 , stop=0;
            int workoutId=0;
            while(!line.empty()) {
                std::string info[3];
                for (int i = 0; i < 3; i++) {
                    while (line.length() > (unsigned)stop && line[stop] != ',')
                        stop++;
                    info[i]=line.substr(start,stop-start);
                    stop++;
                    stop++;
                    start=stop;
                }
                WorkoutType type;
                if(info[1][0]=='A')
                    type=ANAEROBIC;
                else if(info[1][0]=='M')
                    type=MIXED;
                else type=CARDIO;
                //Workout w=new Workout(workoutId, info[0], std::stoi(info[2]), type);
                workout_options.push_back(Workout(workoutId, info[0], std::stoi(info[2],0,10), type));
                workoutId++;
                getline(cFile, line);
                start=0;
                stop=0;
            }
        }
        else{
            std::cout<<"failed to open"<<std::endl;
        }
    }

void Studio::start() {
    std::cout << "Studio is now open!" << std::endl;//need to change that to studio is now open!
    bool shutDown = false;
    while (!shutDown) {
        int start = 0, stop = 0;
        std::string input;
        std::getline(std::cin,input);
        while (input.length() > (unsigned)stop && input[stop] != ' ')
            stop++;
        std::string action = input.substr(start, stop-start);
        stop++;
        start=stop;

        //***openTrainer***
        if(action=="open")
        {
            std::vector<Customer*> customersList; //input for openTrainer
            while(input.length() > (unsigned)stop && input[stop] !=' ')
                stop++;
            int tId = stoi(input.substr(start,stop-start)); //trainer Id from input
            stop++;
            start=stop;
            while(input.length()>(unsigned)start){
                while (input.length() > (unsigned)stop && input[stop] != ' '){
                    stop++;
                }
                    std::string customer = input.substr(start,stop-start); //customer and type
                    int begin=0, end=0;
                    while(customer.length()>(unsigned)end && customer[end]!=',')
                        end++;
                    std::string cName = customer.substr(begin,end-begin); // only customer
                    std::string cType = customer.substr(end+1,3);// only type
                    if(cType=="swt")
                    {
                        customersList.push_back(new SweatyCustomer( cName, customersId));
                    }
                    if(cType=="chp")
                    {
                        customersList.push_back(new CheapCustomer(cName,customersId));
                    }
                    if(cType=="mcl")
                    {
                        customersList.push_back(new HeavyMuscleCustomer(cName,customersId));
                    }
                    if(cType=="fbd")
                    {
                        customersList.push_back(new FullBodyCustomer(cName,customersId));
                    }

                    customersId++;//updating customers count
                stop++;
                start=stop;
            }//-------------maybe need a clone-----------
            OpenTrainer *command = new OpenTrainer(tId, customersList);
            actionsLog.push_back(command); //maybe *
            command->act(*this); //need to check that
        }//end of openTrainer

        //***Order***
        if(action=="order"){
            int tId=stoi(input.substr(stop));
            Order *command = new Order(tId);
            actionsLog.push_back(command);
            command->act(*this);
        }//end of order

        //***move***
        if(action=="move"){
            int begin=stop, end=stop;
            while(input.length() > (unsigned)end && input[end] !=' ')
                end++;
            int srcTrainerId = stoi(input.substr(begin,end-begin)); //trainer Id from input
            end++;

            begin=end;
            while(input.length() > (unsigned)end && input[end] !=' ')
                end++;
            int dstTrainerId = stoi(input.substr(begin,end-begin)); //trainer Id from input
            end++;

            begin=end;
            while(input.length() > (unsigned)end && input[end] !=' ')
                end++;
            int MovingCostumerId = stoi(input.substr(begin,end-begin));

            MoveCustomer *command = new MoveCustomer(srcTrainerId,dstTrainerId,MovingCostumerId);
            actionsLog.push_back(command);
            command->act(*this);
        }//end of move

        //***Close***
        if(action=="close")
        {
            while(input.length() > (unsigned)stop && input[stop] !=' ')
                stop++;
            int tId = stoi(input.substr(start,stop));
             Close *command = new Close(tId);
            actionsLog.push_back(command);
            command->act(*this);
        }//end of close

        //***CloseAll***
        if(action=="closeall")
        {
            CloseAll *command = new CloseAll();
            actionsLog.push_back(command);
            command->act(*this);
            open=false;
            shutDown=true;
            //break; //exit start
        }//end of CloseAll

        //***PrintWorkoutOptions***
        if(action=="workout_options")
        {
            PrintWorkoutOptions *command = new PrintWorkoutOptions();
            actionsLog.push_back(command);
            command->act(*this);
        }//end of PrintWorkoutOptions

        //***PrintTrainerStatus***
        if(action=="status")
        {
        while(input.length() > (unsigned)stop && input[stop] !=' ')
            stop++;
        int tId = stoi(input.substr(start,stop));
        PrintTrainerStatus *command = new PrintTrainerStatus(tId);
        actionsLog.push_back(command);
        command->act(*this);
        }//end of PrintTrainerStatus

        //***PrintActionsLog
        if(action=="log")
        {
            PrintActionsLog *command = new PrintActionsLog();
            actionsLog.push_back(command);
            command->act(*this);
        }//end of PrintActionsLog

        //***BackupStudio
        if(action=="backup")
        {
            BackupStudio *command = new BackupStudio();
            actionsLog.push_back(command);
            command->act(*this);
        }//end BackuoStudio

        //**RestoreStudio
        if(action=="restore")
        {
            RestoreStudio *command = new RestoreStudio();
            actionsLog.push_back(command);
            command->act(*this);
        }//restore



    }//end of while(!Shutdown)
}//end of start

    int Studio::getNumOfTrainers() const {
    return trainers.size();
}

    Trainer *Studio::getTrainer(int tid) {
    return trainers[tid];
}

    const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
    }

    std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}
    Studio::~Studio() {
    for(int i=0;(unsigned)i<trainers.size();i++){
        delete trainers[i];
        trainers[i]= nullptr;
    }
    trainers.clear();
    for(int i=0;(unsigned)i<actionsLog.size();i++){
        delete actionsLog[i];
        actionsLog[i]= nullptr;
    }
    actionsLog.clear();
    }

Studio::Studio(const Studio &other) {
    this->open=other.open;
    this->customersId=other.customersId;
    for(int i=0;(unsigned)i<other.actionsLog.size();i++){
        this->actionsLog.push_back(other.actionsLog[i]->clone());
    }
    for(int i=0;(unsigned)i<other.trainers.size();i++){
        this->trainers.push_back(new Trainer(*other.trainers[i]));
    }
    for(int i=0;(unsigned )i<other.workout_options.size();i++){
        this->workout_options.push_back(other.workout_options[i]);
    }

}

const Studio &Studio::operator=(const Studio &other) {
    if(this!=&other) {
        this->open = other.open;
        this->customersId = other.customersId;
        this->workout_options.clear();
        for (int i = 0; (unsigned) i < other.workout_options.size(); i++)
            this->workout_options.push_back(other.workout_options[i]);

        for (int i = 0; (unsigned) i < this->trainers.size(); i++) {
            delete this->trainers[i];
            this->trainers[i] = nullptr;
        }
        this->trainers.clear();
        for (int i = 0; (unsigned) i < other.trainers.size(); i++) {
            this->trainers.push_back(new Trainer(*other.trainers[i]));
        }
        for (int i = 0; (unsigned) i < this->actionsLog.size(); i++) {
            delete this->actionsLog[i];
            this->actionsLog[i] = nullptr;
        }
        this->actionsLog.clear();
        for (int i = 0; (unsigned) i < other.actionsLog.size(); i++) {
            this->actionsLog.push_back(other.actionsLog[i]->clone());
        }
    }
    return *this;
}

Studio::Studio(Studio &&other) {
    this->open=other.open;
    this->customersId=other.customersId;
    this->actionsLog=std::move(other.actionsLog);
    this->trainers= std::move(other.trainers);
    this->workout_options= std::move(other.workout_options);
}

const Studio &Studio::operator=(Studio &&other) {
    this->open=other.open;
    this->customersId=other.customersId;
    for(int i=0;(unsigned)i<this->trainers.size();i++){
        delete this->trainers[i];
        this->trainers[i]= nullptr;
    }
    trainers.clear();
    this->trainers= std::move(other.trainers);
    for(int i=0;(unsigned)i<this->actionsLog.size();i++){
        delete this->actionsLog[i];
        this->actionsLog[i]= nullptr;
    }
    this->actionsLog.clear();
    this->actionsLog=std::move(other.actionsLog);
    this->workout_options.clear();
    this->workout_options= std::move(other.workout_options);
    return *this;
}

