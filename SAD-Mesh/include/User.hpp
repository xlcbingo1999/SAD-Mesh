#ifndef USER_HPP
#define USER_HPP

#include "FileManager.hpp"
#include "ExecManager.hpp"

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