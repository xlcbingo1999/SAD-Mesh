#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP
#include "DataStructGMsh.hpp"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <iomanip>
#include <vector>
using std::cin;
using std::cout;
using std::ifstream;
using std::iterator;
using std::map;
using std::priority_queue;
using std::set;
using std::string;
using std::vector;
using std::setprecision;



class FileManager {
private:
    string fileName;
    int dataStructOptionCount;
    string *dataStructOption;
    string currentStruct;
    bool importPoint();
    bool importCurve();
    bool importSurface();
    bool importVolume();
    bool importNode();
    bool importElement();
    void outputPoint();
    void outputCurve();
    void outputSurface();
    void outputVolume();
    void outputNode();
    void outputElement();
    bool receiveImportFileName(string to_open);
    int getDataStructOptionCount();
    string getDataStructOption(int did);
    bool usingDataStruct(int did);  
public:
    FileManager();
    ~FileManager();
    bool receiveImportApplication();
    bool receiveChooseDataStruct();
    bool beginImporting();
    void outputDataStruct();
    void diyBeginSignal();
    void signIn();
    void signOut();
};

extern FileManager fileManager;

#endif