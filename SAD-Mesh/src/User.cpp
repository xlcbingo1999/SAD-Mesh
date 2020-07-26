#ifndef USER_CPP
#define USER_CPP

#include "User.hpp"

#define FILE_PATH "../file/TEST.msh" 
#define FILE_PATH_HALF "../file/" 

User::User(/* args */)
{
    cout << "-------------------------Mesh System---------------------------\n" << "You sign in Mesh System successfully!\n" ;
    cout << "Next Step is to apply for importing.\n";
}

User::~User()
{
    cout << "Log Out. Thank you for your using.\n"  << "-------------------------Mesh System---------------------------\n" ;
}


bool User::importApplication() {
    cout << "Step 1: Apply for importing.\n" << "Please input you file name: ";
    string to_open = FILE_PATH_HALF;
    string appen;
    cin >> appen;
    to_open += appen;
    string default_to_open = FILE_PATH;
    if( importApplicationToFileManager(to_open) == false ){
        return false;
    } else {
        if(importApplicationToFileManager(default_to_open) == false) return false;
        return true;
    }
}


bool User::importApplicationToFileManager(string to_open) {
    if(fileManager.receiveImportApplication(to_open) == false) 
        return false;
    return true;
}

bool User::chooseDataStruct() {
    cout << "Step 2: Choose Data Struct Type.\n";
    string *dataStructType;
    int dataStructOptionCount;
    dataStructOptionCount = fileManager.getDataStructOptionCount();
    dataStructType = new string[dataStructOptionCount];
    for(int i = 0; i < dataStructOptionCount; ++i) {
        cout << "["<<  i <<"]" << fileManager.getDataStructOption(i) << "\n";
    }
    cout << "Input your datastruct ID you like. For example: 1\n";
    int did;
    cin >> did;
    if(fileManager.usingDataStruct(did) == false) return false;
    return true;
}

bool User::beginImporting() {
    if(fileManager.beginImporting() == false) return false;
    return true;
}

void User::outputDataStruct() {
    fileManager.outputDataStruct();
}

void User::diy() {
    cout << "Now you have to write your code in TO-DO area in TEST.cpp file.\n" << "And we will use your code to exec!\n" << "Hope you write all good code.\n";
    execManager;
}

#endif
