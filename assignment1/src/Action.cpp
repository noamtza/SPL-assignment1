
//Created by spl211 on 08/11/2021.

#include <Trainer.h>
#include "Action.h"
#include "Studio.h"

extern Studio *backup;

BaseAction::BaseAction() : errorMsg(""), status(ERROR) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList)
        : BaseAction(), trainerId(id), customers(customersList), strHelper("") {}

void OpenTrainer::act(Studio &studio) {
    strHelper = "Open " + std::to_string(trainerId) + " ";
    for (size_t i = 0; i < customers.size(); i++)
        strHelper = strHelper + customers[i]->toString();
    strHelper = strHelper.substr(0, strHelper.length() - 2);
    Trainer *currTrainer = studio.getTrainer(trainerId);
    if ((currTrainer == nullptr) ||(trainerId>studio.getNumOfTrainers())||(currTrainer->isOpen())) {
        //  clone();
        error("Trainer does not exist or is already open");
        std::cout << getErrorMsg() << std::endl;
        strHelper = strHelper + " ERROR:" + getErrorMsg();
        for(size_t i=0;i<customers.size();i++) {
            if(customers[i] != nullptr) {
                delete customers[i];
                customers[i] = nullptr;
            }
     }
       customers.clear();
    } else {
        complete();
        currTrainer->openTrainer();
        currTrainer->setFirstOpen();
        for (size_t i = 0; (i < customers.size()) & (currTrainer->getCapacity() > 0); i++) {
            currTrainer->addCustomer(customers[i]);
            strHelper = strHelper + " Completed";
        }
        for (int i = (int) currTrainer->getCustomers().size(); (i <(int)customers.size()); i++) {
            delete customers[i];
        }
    }

}

void OpenTrainer::setString(std::string str) {
    strHelper = str;
}

std::string OpenTrainer::toString() const {
    return strHelper;
}

BaseAction *OpenTrainer::clone() {
    std::vector<Customer *> otherCustomersList;
    OpenTrainer *other = new OpenTrainer(trainerId, otherCustomersList);
    other->setString(strHelper);
    return other;
}

OpenTrainer::~OpenTrainer() = default;

Order::Order(int id) : BaseAction(), trainerId(id) {}


void Order::act(Studio &studio) {
    Trainer *currTrainer = studio.getTrainer(trainerId);
    if (currTrainer == nullptr || !(currTrainer->isOpen())) {
        error("Trainer does not exist or is already open");
        std::cout << getErrorMsg() << std::endl;

    } else {
        complete();
        std::vector<int> workoutIds;
        for (size_t j = 0; j < studio.getWorkoutOptions().size(); j++) {
            workoutIds.push_back(j);
        }
        for (size_t i = 0; i < currTrainer->getCustomers().size(); i++)//PASS ON THE LIST OF CUSTOMERS OF THE TRAINER
        {
            int cusId = currTrainer->getCustomers()[i]->getId();
            currTrainer->order(cusId, workoutIds,
                               studio.getWorkoutOptions());//the orders of the specific customer of the trainer.
        }
    }
};

std::string Order::toString() const {
    std::string str = "Order " + std::to_string(trainerId) + " ";
    if (getStatus() == 1) {
        str = str + " ERROR:" + getErrorMsg();
    } else {
        str = str + " Completed";
    }
    return str;

}

BaseAction *Order::clone() {
    return new Order(*this);
}

Order::~Order() = default;

MoveCustomer::MoveCustomer(int src, int dst, int customerId)
        : BaseAction(), srcTrainer(src), dstTrainer(dst), id(customerId) {}

void MoveCustomer::act(Studio &studio) {//need to move customer from trainer with id src to trainer with id dst
    Trainer *srcT = studio.getTrainer(srcTrainer);
    Trainer *dstT = studio.getTrainer(dstTrainer);
    if (srcT == nullptr || !(srcT->isOpen()) || dstT == nullptr || !(dstT->isOpen())) {
        error("Trainer does not exist or is already open");
        std::cout << getErrorMsg() << std::endl;
    } else if (dstT->getCapacity() == 0) {
        error("Cannot Move Customer");
        std::cout << getErrorMsg() << std::endl;
    } else {
        bool isFound = false;
        for (size_t i = 0; i < srcT->getCustomers().size() && !isFound; i++) {
            if (srcT->getCustomers()[i]->getId() == id)
                isFound = true;
        }
        if (!isFound) {
            error("Trainer does not have this customer");
            std::cout << getErrorMsg() << std::endl;
        } else {
            complete();
            std::vector<OrderPair> newOrderList;
            Customer *moveCus = srcT->getCustomer(id);
            dstT->addCustomer(moveCus);
            for (auto it = srcT->getOrders().begin(); it != srcT->getOrders().end(); it++) {
                if ((*it).first == moveCus->getId()) {//we found order that we need to move
                    dstT->getOrders().push_back(*it);//we add the order to the dst orderList
                    dstT->setSalary(dstT->getSalary() + (*it).second.getPrice());
                    srcT->setSalary(srcT->getSalary() - (*it).second.getPrice());
                    //want to erase order from src trainer

                } else {
                    newOrderList.push_back(*it);//orders that are not belong to the customer we removed.
                    //std::cout << std::to_string((*it).first) + (*it).second.getName() << std::endl;
                }
            }
            newOrderList.swap(srcT->getOrders());
            newOrderList.clear();
            srcT->removeCustomer(id);
            if (srcT->getCustomers().size() == 0) {//trainer has no customers left, close his session
                srcT->closeTrainer();
                std::cout << "Trainer " + std::to_string(srcTrainer) + " closed. Salary " +
                             std::to_string(srcT->getSalary()) + "NIS" << std::endl;
            }

        }

    }
}

std::string MoveCustomer::toString() const {
    std::string str =
            "Move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id);
    if (getStatus() == 1) {
        str = str + " ERROR:" + getErrorMsg();
    } else {
        str = str + " Completed";
    }
    return str;


}

BaseAction *MoveCustomer::clone() {
    return new MoveCustomer(*this);
}

MoveCustomer::~MoveCustomer() = default;

Close::Close(int id) : BaseAction(), trainerId(id) {}

void Close::act(Studio &studio) {
    Trainer *myTrainer = studio.getTrainer(trainerId);
    if (myTrainer == nullptr || !(myTrainer->isOpen())) {
        error("Trainer does not exist or is alreadyClose");
        std::cout << getErrorMsg() << std::endl;

    } else {
        complete();
        myTrainer->closeTrainer();
        std::cout << "Trainer " + std::to_string(trainerId) + " closed. Salary " +
                     std::to_string(myTrainer->getSalary()) + "NIS" << std::endl;

    }
}

std::string Close::toString() const {
    std::string str = "Close " + std::to_string(trainerId);
    if (getStatus() == 1) {
        str = str + " ERROR:" + getErrorMsg();
    } else {
        str = str + " Completed";
    }
    return str;
}

BaseAction *Close::clone() {
    return new Close(*this);
}

Close::~Close() = default;

CloseAll::CloseAll() : BaseAction() {}

void CloseAll::act(Studio &studio) {
    for (int i = 0; i < studio.getNumOfTrainers(); i++) {
        Trainer *myTrainer = studio.getTrainer(i);//i= mytrainer id
        if (myTrainer == nullptr || !(myTrainer->isOpen())) {
            error("Trainer does not exist or is already Close");
        } else {
            complete();
            myTrainer->closeTrainer();
            std::cout << "Trainer " + std::to_string(i) + " closed. Salary " + std::to_string(myTrainer->getSalary()) +
                         "NIS" << std::endl;
        }
    }//need to delete the emptyvec
    studio.myDelete();
}

std::string CloseAll::toString() const {
    std::string str = "CloseAll ";
    if (getStatus() == 1) {
        str = str + " ERROR:" + getErrorMsg();
    } else {
        str = str + " Completed";
    }
    return str;
}

BaseAction *CloseAll::clone() {
    return new CloseAll(*this);
}

CloseAll::~CloseAll() = default;

PrintWorkoutOptions::PrintWorkoutOptions() : BaseAction() {}

void PrintWorkoutOptions::act(Studio &studio) {
    complete();
    for (size_t i = 0; i < studio.getWorkoutOptions().size(); i++) {
        std::cout << studio.getWorkoutOptions()[i].toString() << std::endl;
    }
}

std::string PrintWorkoutOptions::toString() const {
    std::string str = " PrintWorkoutOptions Completed";
    return str;
}

BaseAction *PrintWorkoutOptions::clone() {
    return new PrintWorkoutOptions(*this);
}

PrintWorkoutOptions::~PrintWorkoutOptions() = default;

PrintTrainerStatus::PrintTrainerStatus(int id) : BaseAction(), trainerId(id) {}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !(trainer->isFirstOpen())) {
        error("Trainer is not exist or his workout session has not started");
        std::cout << getErrorMsg() << std::endl;
    } else {
        complete();
        if (trainer->isOpen()) {
            std::cout << "Trainer " + std::to_string(trainerId) + " status: open" << std::endl;
            std::cout << "Customers:" << std::endl;
            for (size_t i = 0; i < trainer->getCustomers().size(); i++) {
                Customer *cus = trainer->getCustomers()[i];
                std::cout << std::to_string(cus->getId()) + " " + cus->getName() << std::endl;
            }
            std::cout << "Orders:" << std::endl;
            for (size_t i = 0; i < trainer->getOrders().size(); i++) {
                //name,price,id
                Workout curr = trainer->getOrders()[i].second;
                std::cout << curr.getName() + " " + std::to_string(curr.getPrice()) + "NIS " +
                             std::to_string(trainer->getOrders()[i].first) << std::endl;
            }
            std::cout << "Current Trainer's Salary: " + std::to_string(trainer->getSalary()) + "NIS" << std::endl;
        } else {//trainer is closed
            std::cout << "Trainer " + std::to_string(trainerId) + " status: closed" << std::endl;
        }

    }

}

std::string PrintTrainerStatus::toString() const {
    std::string str = "PrintTrainerStatus";
    if (getStatus() == 1) {
        str = str + " ERROR:" + getErrorMsg();
    } else {
        str = str + " Completed";
    }
    return str;

}

BaseAction *PrintTrainerStatus::clone() {
    return new PrintTrainerStatus(*this);
}

PrintTrainerStatus::~PrintTrainerStatus() = default;

PrintActionsLog::PrintActionsLog() : BaseAction() {};

void PrintActionsLog::act(Studio &studio) {
    complete();
    for (size_t i = 0; i < studio.getActionsLog().size(); i++) {
        BaseAction *myAction = studio.getActionsLog()[i];
        std::cout << myAction->toString() << std::endl;
    }
}

std::string PrintActionsLog::toString() const {
    std::string str = " PrintActionsLog Completed";
    return str;
}

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

PrintActionsLog::~PrintActionsLog() = default;

BackupStudio::BackupStudio():BaseAction() {};
void BackupStudio::act(Studio &studio) {
    complete();
    //extern Studio* backup(&studio);
   Studio *tobackup=backup;
    backup=new Studio(studio);
    delete tobackup;
}
std::string BackupStudio::toString() const {
    std::string str="Backup Completed";
    return str;
}
BaseAction* BackupStudio::clone(){
    return new BackupStudio();
}
BackupStudio::~BackupStudio() = default;

RestoreStudio::RestoreStudio() :  BaseAction() {}
void RestoreStudio::act(Studio &studio){
    if(backup!=nullptr){
        studio=*backup;
        complete();}
        else {
            error("No Backup Availible");
            std::cout << getErrorMsg() << std::endl;
        }
    }
std::string RestoreStudio::toString() const {
    std::string str = "RestoreStudio";
    if (getStatus() == 1) {
        str = str + " ERROR:" + getErrorMsg();
    } else {
        str = str + " Completed";
    }
    return str;
}
BaseAction* RestoreStudio::clone(){
    return new RestoreStudio(*this);
}
RestoreStudio::~RestoreStudio() = default;