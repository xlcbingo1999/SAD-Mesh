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
public:
    FileManager();
    ~FileManager();
    bool receiveImportApplication(string to_open);

    int getDataStructOptionCount();
    string getDataStructOption(int did);
    bool usingDataStruct(int did);

    bool beginImporting();

    void outputDataStruct();
};

extern FileManager fileManager;

#endif