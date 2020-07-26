#ifndef DATASTRUCTGMSH_CPP
#define DATASTRUCTGMSH_CPP
#include "DataStructGMsh.hpp"

meshHead realMeshHead;


void PointCatalog::insert(std::pair<int,PointDescription*> to_insert){
    catalogMap.insert(to_insert);
}

PointDescription* PointCatalog::getDescription(int tag){
    auto search = catalogMap.find(tag);
    if(search != catalogMap.end()) {
        return search->second;
    }
}

void CurveCatalog::insert(std::pair<int,CurveDescription*> to_insert){
    catalogMap.insert(to_insert);
}

CurveDescription* CurveCatalog::getDescription(int tag){
    auto search = catalogMap.find(tag);
    if(search != catalogMap.end()) {
        return search->second;
    }
}

CurveCatalog::CurveCatalog() {

}

CurveCatalog::~CurveCatalog() {

}

PointCatalog::PointCatalog() {

}

PointCatalog::~PointCatalog() {
    
}


#endif