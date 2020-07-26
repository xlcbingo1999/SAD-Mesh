#ifndef USER_HPP
#define USER_HPP
#include <iostream>
#include <string>
#include "FileManager.hpp"
#include "ExecManager.hpp"

using std::cin;
using std::cout;
using std::string;

class User
{
private:
    bool importApplicationToFileManager(string to_open);
public:
    User();
    ~User();
    bool importApplication();
    bool chooseDataStruct();
    bool beginImporting();
    void outputDataStruct();
    void diy();

    virtual double Execute(struct ArguMap** argus, int index) = 0;
    virtual void displayResult(int ref,double ret) = 0;

    virtual void beginSave() = 0;
    virtual void saveItem(double ret) = 0;
    virtual void endSave() = 0;
};



#endif