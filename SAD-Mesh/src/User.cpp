#ifndef USER_CPP
#define USER_CPP

#include "User.hpp"



User::User(/* args */)
{
    fileManager.signIn();
}

User::~User()
{
    fileManager.signOut();
    fileManager.~FileManager();
    execManager.~ExecManager();
}


bool User::importApplication() {
    if( fileManager.receiveImportApplication() == false ){
        return false;
    }
    return true;
}


bool User::chooseDataStruct() {
    if(fileManager.receiveChooseDataStruct() == false) {
        return false;
    }
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
    fileManager.diyBeginSignal();
}


#endif
