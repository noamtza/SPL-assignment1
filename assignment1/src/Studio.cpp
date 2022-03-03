
//Created by spl211 on 08/11/2021.

#include <fstream>
#include "Studio.h"
#include <iostream>
//class Trainer;#include <sstream>
//class Action;

Studio::Studio() : open(false),counterCusId(0),trainers(),workout_options(),actionsLog(){};

Studio::Studio(const Studio &other) : open(other.open) ,counterCusId(other.counterCusId),trainers(other.trainers),workout_options(other.workout_options),actionsLog(other.actionsLog){//copy c
    for (size_t i = 0; i < other.trainers.size(); i++) {
        trainers[i] = other.trainers[i]->clone();
    }
    for (size_t i = 0; i < other.workout_options.size(); i++) {
        workout_options.push_back(other.workout_options[i]);
    }
    for (size_t i = 0; i < other.actionsLog.size(); i++) {
        actionsLog[i] = other.actionsLog[i]->clone();
    }
}

const Studio &Studio::operator=(const Studio &other) {//copy assignment
    if (this != &other) {
        myDelete();
        open = other.open;
        counterCusId=other.counterCusId;
        for (size_t i = 0; i < other.trainers.size(); i++) {
            trainers.push_back(other.trainers[i]->clone());
        }
        for (size_t i = 0; i < other.workout_options.size(); i++) {
            workout_options.push_back(other.workout_options[i]);
        }
        for (size_t i = 0; i < other.actionsLog.size(); i++) {
            actionsLog.push_back(other.actionsLog[i]->clone());
        }
    }
    return *this;
}

Studio::Studio(Studio &&other) : open(other.open),counterCusId(other.counterCusId),trainers(other.trainers) ,workout_options(other.workout_options),actionsLog(other.actionsLog) {//move constructor
    for (size_t i = 0; i < other.actionsLog.size(); i++) {
        actionsLog[i] = other.actionsLog[i];
        other.actionsLog[i] = nullptr;
    }
    for (size_t i = 0; i < other.trainers.size(); i++) {
        trainers[i] = other.trainers[i];
        other.trainers[i] = nullptr;
    }

}

void Studio::myDelete() {//for the destructor
    open=false;
    counterCusId=0;
    for (size_t i = 0; i < trainers.size(); i++) {
        delete trainers[i];
      //  trainers[i] = nullptr;
    }
    trainers.clear();
    workout_options.clear();
    for (size_t i = 0; i < actionsLog.size(); i++) {
        delete actionsLog[i];
       // actionsLog[i] = nullptr;
    }
    actionsLog.clear();
}

Studio::~Studio() { myDelete(); }//destructor


const Studio &Studio::operator=(Studio &&other) {//move assignment
    open = other.open;
    counterCusId=other.counterCusId;
    if (this != &other) {
        myDelete();
        for (size_t i = 0; i < other.actionsLog.size(); i++) {
            actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i] = nullptr;
        }
        workout_options = other.workout_options;
        for (size_t i = 0; i < other.trainers.size(); i++) {
            trainers.push_back(other.trainers[i]);
            other.trainers[i] = nullptr;
        }
    }
    return *this;
}

Studio:: Studio(const std::string &configFilePath): open(false),counterCusId(0),trainers(),workout_options(),actionsLog(){
    //Studio studio=Studio();
    std::ifstream inputFile;
    inputFile.open(configFilePath);
    std::string line;
    int countLoops=0;
//    int numOfTrainers=0;
    int workId = 0;
    if (inputFile.is_open()){
        while(std:: getline(inputFile,line)){
            if (line[0] == '#'){
                continue;
            }
            if (line[0] == '\0'){
                continue;
            }
            if (countLoops == 0&& std::isdigit(line[0])){
                countLoops++;
//                numOfTrainers = std::stoi(line);
                continue;
            }
            if (countLoops==1 &&std::isdigit(line[0])){
                int index=0;
                int len = 0;
                std::string str(line);
                bool finish=false;
                for(int i=0;str[i]!='\0'&&!finish;i++){
                    if (str[i] != ','){
                        len++;
                    }
                    else
                    {
                        Trainer *myTrainer = new Trainer(stoi(str.substr(index, len)));
                        trainers.push_back(myTrainer);
                        index=index+len;
                        if(str[index]=='\0')
                            finish=true;
                        else
                            index++;
                        len = 0;
                    }
                }
                Trainer *myTrainer = new Trainer(stoi(str.substr(index, len)));
                trainers.push_back(myTrainer);
                countLoops++;
                continue;
            }
            if (countLoops>1){
                int index1 = 0;
                int workPrice;
                std::string str1(line);
                std:: string Workname;
                while (str1[index1] != ','){
                    Workname=Workname+str1[index1];
                    index1++;
                }
               index1++;
                while(str1[index1] == ' '){
                    index1++;
                }
                WorkoutType woType;
                if(str1[index1] == 'A'){
                    woType = ANAEROBIC;
                }
                if(str1[index1]  =='M'){
                    woType = MIXED;
                }
                if(str1[index1] == 'C'){
                    woType = CARDIO;
                }
                while(str1[index1]!=',')
                    index1++;
                index1++;
                int len1 = 0;
                while(str1[index1] == ' '){
                    index1++;
                }
                int index2=index1;
                while (str1[index1] != '\0'){
                    index1++;
                    len1++;
                }
                workPrice = stoi(str1.substr(index2, len1));
                workout_options.push_back(Workout( workId, Workname,workPrice, woType));
                workId++;
                countLoops++;
            }
        }
        inputFile.close();
    }

}

void Studio::start() {//start function
    open = true;
     counterCusId = 0;
    std::cout << "Studio is now open!" << std::endl;
    std::string input = "";
    while (open) {
        getline(std::cin, input);
        BaseAction *baseAc = curAction(input);//curr action is our helper function
        baseAc->act(*this);//change
        actionsLog.push_back(baseAc );
           // delete baseAc;
           // return;
    }
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

Trainer *Studio::getTrainer(int tid) {
    return trainers[tid];
}


std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}


BaseAction* Studio::curAction(std::string input) {//dont forget to do customerid++

    if (input.substr(0, 4) == "open") {
        bool tocontinue = true;
        std::vector<Customer *> customersList;
        int trainerNum;
        int start = 5;
        int len = 0;
        while ((input[start + len] != '\0') && (input[start + len] != ' ')) {//if num have more than one digit
            len++;
        }
        trainerNum = stoi(input.substr(start, len));
        if (input[start + len] == '\0')
            tocontinue = false;
        start = start + len + 1;
        while (tocontinue == true) {
            std::string name = "";
            while ((input[start] != ',') & (tocontinue) & (input[start] != '\0')) {
                name = name + input[start];
                start++;
            }
            start = start + 1;
            if (input[start] == 's') {
                Customer *cus = new SweatyCustomer(name, counterCusId);
                customersList.push_back(cus);
                start = start + 3;
            } else if (input[start] == 'c') {
                customersList.push_back(new CheapCustomer(name, counterCusId));
                start = start + 3;
            } else if (input[start] == 'm') {
                customersList.push_back(new HeavyMuscleCustomer(name, counterCusId));
                start = start + 3;
            } else if (input[start] == 'f') {
                customersList.push_back(new FullBodyCustomer(name, counterCusId));
                start = start + 3;
            }
            counterCusId++;
            if (input[start] == '\0') {
                tocontinue = false;
            } else {
                start = start + 1;
            }
        }

        return new OpenTrainer(trainerNum, customersList);
    }

    int o_id = 0;
    if (input.substr(0, 5) == "order") {
        std::string trainerNum = " ";
        for (int i = 6; input[i] != '\0'; i++) {
            trainerNum = trainerNum + input[i];
        }
        if (trainerNum != " ") {
            o_id = std::stoi(trainerNum);
        }//char toInt?}
        return new Order(o_id);
    }
    if (input.substr(0, 6) == "status") {
        std::string trainerNum = " ";
        for (int i = 7; input[i] != '\0'; i++) {
            trainerNum = trainerNum + input[i];
        }
        if (trainerNum != " ") {
            o_id = std::stoi(trainerNum);
        }//char toInt}

        return new PrintTrainerStatus(o_id);
    }
    if (input.substr(0, 4) == "move") {
        int st_id = -1;
        int dt_id = -1;
        int mc_id = -1;
        std::string src_t_Num = "";
        int start = 5;//first relevant num
        for (int i = start; input[i] != ' '; i++) {
            src_t_Num = src_t_Num + input[i];//foud source trainer id
        }
        if (src_t_Num != " ") {
            st_id = std::stoi(src_t_Num);//found source trainer id
            start = start + src_t_Num.length();
        }
        std::string dst_t_Num = "";
        for (int i = start + 1; input[i] != ' '; i++) {
            dst_t_Num = dst_t_Num + input[i];
        }
        if (dst_t_Num != " ") {
            dt_id = std::stoi(dst_t_Num);//found destination trainer id
            start = start + src_t_Num.length();
        }

        std::string cus_Num = "";
        for (int i = start + 1; input[i] != '\0'; i++) {
            cus_Num = cus_Num + input[i];
        }
        if (cus_Num != " ") {
            mc_id = std::stoi(cus_Num);
        }//found customer id;

        return new MoveCustomer(st_id, dt_id, mc_id);
    }
    if (input.substr(0, 5) == "close") {
        if (input[5] == 'a') {
            return new CloseAll();
        } else {
            int ct_id = -1;
            std::string trainerNum = " ";
            for (int i = 6; input[i] != '\0'; i++) {
                trainerNum = trainerNum + input[i];
            }
            if (trainerNum != " ") {
                ct_id = std::stoi(trainerNum);
            }
            return new Close(ct_id);;
        }

    }

    if ((input.substr(0, 15)) == "workout_options") {
        return new PrintWorkoutOptions();
    }
    if ((input.substr(0, 3)) == "log") {
        return new PrintActionsLog();
    }
//    return 0;
//}
    if ((input.substr(0, 6)) == "backup") {
        return new BackupStudio();;
    }
    if ((input.substr(0, 7)) == "restore") {
        return new RestoreStudio();
    }
    return 0;
}
