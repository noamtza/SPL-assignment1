#include "Studio.h"
#include <iostream>
#include <sstream>
#include <fstream>


using namespace std;
Studio* backup = nullptr;

int main(int argc, char** argv){
//    if(argc!=2){
//        std::cout << "usage: studio <config_path>" << std::endl;
//        return 0;
//    }
    string configurationFile = argv[1];
    string path="";
    Studio studio(path);
    studio.start();
    if(backup!=nullptr){
        delete backup;
        backup = nullptr;
    }
    return 0;
}
