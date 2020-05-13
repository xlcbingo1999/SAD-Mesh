

#include <bits/stdc++.h>
#include <fstream>
using namespace std;

struct Point {
    int pointTag;
    double x;
    double y;
    double z;
    size_t numPhysicalTags;
    int physicalTag;
};

struct Curve {
    int curveTag;
    double startx;
    double starty;
    double startz;
    double endx;
    double endy;
    double endz;
    size_t numPhysicalTags;
    int physicalTag;
    size_t numBoundingPoints;
    int startPoint;
    int endPoint;
};

struct Surface {
    int surfaceTag;
    double startx;
    double starty;
    double startz;
    double endx;
    double endy;
    double endz;
    size_t numPhysicalTags;
    int physicalTag;
    size_t numBoundingCurves;
    int *vectors;
    int *pointsLine;
};

struct Volume {
    int volumeTag;
    double startx;
    double starty;
    double startz;
    double endx;
    double endy;
    double endz;
    size_t numPhysicalTags;
    size_t physicalTag;
    size_t numBoundngSurfaces;
    int *surfaces;
};

struct Node {
    double x;
    double y;
    double z;
    vector<int> connectedElements;
};

struct EntityBlockNode {
    int entityDim;
    int entityTag;
    int parametric;
    size_t numNodesInBlock;
    size_t *nodesIndex;
};

struct NodeSet {
    size_t numEntityBlocks;
    size_t numNodes;
    size_t minNodeTag;
    size_t maxNodeTag;
    EntityBlockNode *entityblocks;
};

struct EntityBlockElement {
    int entityDim;
    int entityTag;
    int elementType;
    size_t numElementsInBlock;
    size_t *elementsIndex;
};

struct ElementSet {
    size_t numEntityBlocks;
    size_t numElements;
    size_t minElementTag;
    size_t maxElementTag;
    EntityBlockElement *entityblocks;
};

struct Element {
    int entityDim;
    size_t *nodesIndex;
    size_t *nearbyEntityDim;
    set<int> nearbyElement;
};

struct meshHead {
    string version;
    int filetype;
    int datalength;
    size_t pointsNum;
    size_t curvesNum;
    size_t surfacesNum;
    size_t volumesNum;
    Point *Points;
    Curve *curves;
    Surface *surfaces;
    Volume *volumes;
    NodeSet nodesSet;
    Node *nodes;
    ElementSet elementsSet;
    Element *elements;
};

void readMsh(const char *filename, meshHead *myMeshHead) {
    // 以读模式打开文件
    ifstream infile;
    infile.open(filename);
    string str;
    cout << "Reading from the file" << endl;
    int index;
    float viceFloat;
    int viceInt;
    while (infile.good()) {
        getline(infile, str);
        if (str == "$MeshFormat") {
            infile >> myMeshHead->version >> myMeshHead->filetype >> myMeshHead->datalength;
            // cout << "$MeshFormat\n" << myMeshHead->version << " " << myMeshHead->filetype << " " << myMeshHead->datalength << endl << "$EndMeshFormat\n";
        } else if (str == "$Entities") {
            infile >> myMeshHead->pointsNum >> myMeshHead->curvesNum >> myMeshHead->surfacesNum >> myMeshHead->volumesNum;
            myMeshHead->Points = new Point[myMeshHead->pointsNum + 1];
            cout << "$Entities\n"
                 << myMeshHead->pointsNum << " " << myMeshHead->curvesNum << " " << myMeshHead->surfacesNum << " " << myMeshHead->volumesNum << endl;
            for (int i = 0; i < myMeshHead->pointsNum; ++i) {
                infile >> myMeshHead->Points[i].pointTag;
                infile >> myMeshHead->Points[i].x;
                infile >> myMeshHead->Points[i].y;
                infile >> myMeshHead->Points[i].z;
                infile >> myMeshHead->Points[i].physicalTag;
                // cout << setprecision(16) << myMeshHead->Points[i].pointTag << " " << myMeshHead->Points[i].x <<" " <<  myMeshHead->Points[i].y << " " << myMeshHead->Points[i].z << " " << myMeshHead->Points[i].physicalTag << endl;
            }
            myMeshHead->curves = new Curve[myMeshHead->curvesNum + 1];
            for (int i = 0; i < myMeshHead->curvesNum; ++i) {
                infile >> myMeshHead->curves[i].curveTag;
                infile >> myMeshHead->curves[i].startx >> myMeshHead->curves[i].starty >> myMeshHead->curves[i].startz;
                infile >> myMeshHead->curves[i].endx >> myMeshHead->curves[i].endy >> myMeshHead->curves[i].endz;
                infile >> myMeshHead->curves[i].physicalTag >> myMeshHead->curves[i].numBoundingPoints;
                infile >> myMeshHead->curves[i].startPoint;
                infile >> viceInt;
                myMeshHead->curves[i].endPoint = (0 - viceInt);
                // cout << myMeshHead->curves[i].curveTag << setprecision(16) << " ";
                // cout << myMeshHead->curves[i].startx << " " << myMeshHead->curves[i].starty << " " << myMeshHead->curves[i].startz <<" ";
                // cout << myMeshHead->curves[i].endx << " " << myMeshHead->curves[i].endy << " " << myMeshHead->curves[i].endz <<" ";
                // cout << myMeshHead->curves[i].physicalTag << " " << myMeshHead->curves[i].numBoundingPoints << " " << myMeshHead->curves[i].startPoint <<" " << myMeshHead->curves[i].endPoint << endl;
            }
            myMeshHead->surfaces = new Surface[myMeshHead->surfacesNum + 1];
            for (int i = 0; i < myMeshHead->surfacesNum; ++i) {
                infile >> myMeshHead->surfaces[i].surfaceTag;
                infile >> myMeshHead->surfaces[i].startx >> myMeshHead->surfaces[i].starty >> myMeshHead->surfaces[i].startz;
                infile >> myMeshHead->surfaces[i].endx >> myMeshHead->surfaces[i].endy >> myMeshHead->surfaces[i].endz;
                infile >> myMeshHead->surfaces[i].physicalTag >> myMeshHead->surfaces[i].numBoundingCurves;
                viceInt = myMeshHead->surfaces[i].numBoundingCurves + 1;
                myMeshHead->surfaces[i].vectors = new int[viceInt];
                myMeshHead->surfaces[i].pointsLine = new int[viceInt];
                int tempJ = 0;
                for (int j = 0; j < myMeshHead->surfaces[i].numBoundingCurves; ++j) {
                    infile >> myMeshHead->surfaces[i].vectors[j];
                    if (j == 0) {
                        if (myMeshHead->surfaces[i].vectors[j] > 0) {
                            viceInt = myMeshHead->surfaces[i].vectors[j];
                            myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curves[viceInt - 1].startPoint;
                            tempJ++;
                            myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curves[viceInt - 1].endPoint;
                            tempJ++;
                        } else {
                            viceInt = 0 - myMeshHead->surfaces[i].vectors[j];
                            myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curves[viceInt - 1].endPoint;
                            tempJ++;
                            myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curves[viceInt - 1].startPoint;
                            tempJ++;
                        }
                    } else if (j == myMeshHead->surfaces[i].numBoundingCurves - 1) {
                    } else {
                        if (myMeshHead->surfaces[i].vectors[j] > 0) {
                            viceInt = myMeshHead->surfaces[i].vectors[j];
                            myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curves[viceInt - 1].endPoint;
                            tempJ++;
                        } else {
                            viceInt = 0 - myMeshHead->surfaces[i].vectors[j];
                            myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curves[viceInt - 1].startPoint;
                            tempJ++;
                        }
                    }
                }
                // cout << myMeshHead->surfaces[i].surfaceTag << setprecision(16) << " ";
                // cout << myMeshHead->surfaces[i].startx << " " << myMeshHead->surfaces[i].starty << " " << myMeshHead->surfaces[i].startz <<" ";
                // cout << myMeshHead->surfaces[i].endx << " " << myMeshHead->surfaces[i].endy << " " << myMeshHead->surfaces[i].endz <<" ";
                // cout << myMeshHead->surfaces[i].physicalTag << " " << myMeshHead->surfaces[i].numBoundingCurves << " ";
                // for(int j = 0; j < myMeshHead->surfaces[i].numBoundingCurves; ++j){
                //     cout << myMeshHead->surfaces[i].vectors[j] << " ";
                // }
                // // Unfinished!
                // cout << endl <<  "pointsLine: ";
                // for(int j = 0; j < myMeshHead->surfaces[i].numBoundingCurves; ++j){
                //     cout << myMeshHead->surfaces[i].pointsLine[j] << " ";
                // }
                // cout << endl;
            }
            myMeshHead->volumes = new Volume[myMeshHead->volumesNum + 1];
            for (int i = 0; i < myMeshHead->volumesNum; ++i) {
                infile >> myMeshHead->volumes[i].volumeTag;
                infile >> myMeshHead->volumes[i].startx >> myMeshHead->volumes[i].starty >> myMeshHead->volumes[i].startz;
                infile >> myMeshHead->volumes[i].endx >> myMeshHead->volumes[i].endy >> myMeshHead->volumes[i].endz;
                infile >> myMeshHead->volumes[i].numPhysicalTags >> myMeshHead->volumes[i].physicalTag >> myMeshHead->volumes[i].numBoundngSurfaces;
                viceInt = myMeshHead->volumes[i].numBoundngSurfaces + 1;
                myMeshHead->volumes[i].surfaces = new int[viceInt];
                for (int j = 0; j < myMeshHead->volumes[i].numBoundngSurfaces; ++j) {
                    infile >> myMeshHead->volumes[i].surfaces[j];
                }
                // cout << myMeshHead->volumes[i].volumeTag << setprecision(16) << " ";
                // cout << myMeshHead->volumes[i].startx << " " << myMeshHead->volumes[i].starty << " " << myMeshHead->volumes[i].startz <<" ";
                // cout << myMeshHead->volumes[i].endx << " " << myMeshHead->volumes[i].endy << " " << myMeshHead->volumes[i].endz <<" ";
                // cout << myMeshHead->volumes[i].numPhysicalTags << " " << myMeshHead->volumes[i].physicalTag << " " << myMeshHead->volumes[i].numBoundngSurfaces << " ";
                // for(int j = 0; j < myMeshHead->volumes[i].numBoundngSurfaces; ++j){
                //     cout << myMeshHead->volumes[i].surfaces[j] << " ";
                // }
                // cout << endl;
            }
            cout << "$EndEntities\n";
        } else if (str == "$Nodes") {
            infile >> myMeshHead->nodesSet.numEntityBlocks;
            infile >> myMeshHead->nodesSet.numNodes;
            infile >> myMeshHead->nodesSet.minNodeTag;
            infile >> myMeshHead->nodesSet.maxNodeTag;
            myMeshHead->nodes = new Node[myMeshHead->nodesSet.numNodes + 1];
            myMeshHead->nodesSet.entityblocks = new EntityBlockNode[myMeshHead->nodesSet.numEntityBlocks + 1];
            for (int i = 0; i < myMeshHead->nodesSet.numEntityBlocks; ++i) {
                infile >> myMeshHead->nodesSet.entityblocks[i].entityDim;
                infile >> myMeshHead->nodesSet.entityblocks[i].entityTag;
                infile >> myMeshHead->nodesSet.entityblocks[i].parametric;
                infile >> myMeshHead->nodesSet.entityblocks[i].numNodesInBlock;
                size_t count = myMeshHead->nodesSet.entityblocks[i].numNodesInBlock;
                myMeshHead->nodesSet.entityblocks[i].nodesIndex = new size_t[count + 1];
                for (int j = 0; j < myMeshHead->nodesSet.entityblocks[i].numNodesInBlock; ++j) {
                    infile >> myMeshHead->nodesSet.entityblocks[i].nodesIndex[j];
                }
                for (int j = myMeshHead->nodesSet.entityblocks[i].nodesIndex[0]; j < myMeshHead->nodesSet.entityblocks[i].nodesIndex[0] + myMeshHead->nodesSet.entityblocks[i].numNodesInBlock; ++j) {
                    infile >> myMeshHead->nodes[j].x;
                    infile >> myMeshHead->nodes[j].y;
                    infile >> myMeshHead->nodes[j].z;
                }
            }
            // cout << "$Nodes\n" << setprecision(16);
            // cout << myMeshHead->nodesSet.numEntityBlocks << " ";
            // cout << myMeshHead->nodesSet.numNodes << " ";
            // cout << myMeshHead->nodesSet.minNodeTag << " ";
            // cout << myMeshHead->nodesSet.maxNodeTag << " " << endl;
            // for(int i = 0; i < myMeshHead->nodesSet.numEntityBlocks; ++i){
            //     cout << myMeshHead->nodesSet.entityblocks[i].entityDim << " ";
            //     cout << myMeshHead->nodesSet.entityblocks[i].entityTag << " ";
            //     cout << myMeshHead->nodesSet.entityblocks[i].parametric << " ";
            //     cout << myMeshHead->nodesSet.entityblocks[i].numNodesInBlock << " " << endl;
            //     for(int j = 0; j < myMeshHead->nodesSet.entityblocks[i].numNodesInBlock; ++j){
            //         cout << myMeshHead->nodesSet.entityblocks[i].nodesIndex[j] << endl;
            //     }
            //     for(int j = 0; j < myMeshHead->nodesSet.entityblocks[i].numNodesInBlock; ++j){
            //         size_t tempNodesIndex = myMeshHead->nodesSet.entityblocks[i].nodesIndex[j];
            //         cout << myMeshHead->nodes[tempNodesIndex].x << " ";
            //         cout << myMeshHead->nodes[tempNodesIndex].y << " ";
            //         cout << myMeshHead->nodes[tempNodesIndex].z << " " << endl;
            //     }
            // }
            // cout << "$EndNodes\n";
        } else if (str == "$Elements") {
            infile >> myMeshHead->elementsSet.numEntityBlocks;
            infile >> myMeshHead->elementsSet.numElements;
            infile >> myMeshHead->elementsSet.minElementTag;
            infile >> myMeshHead->elementsSet.maxElementTag;
            myMeshHead->elements = new Element[myMeshHead->elementsSet.numElements + 1];
            myMeshHead->elementsSet.entityblocks = new EntityBlockElement[myMeshHead->elementsSet.numEntityBlocks + 1];
            for (int i = 0; i < myMeshHead->elementsSet.numEntityBlocks; ++i) {
                infile >> myMeshHead->elementsSet.entityblocks[i].entityDim;
                infile >> myMeshHead->elementsSet.entityblocks[i].entityTag;
                infile >> myMeshHead->elementsSet.entityblocks[i].elementType;
                infile >> myMeshHead->elementsSet.entityblocks[i].numElementsInBlock;
                size_t count = myMeshHead->elementsSet.entityblocks[i].numElementsInBlock;
                myMeshHead->elementsSet.entityblocks[i].elementsIndex = new size_t[count + 1];
                for (int j = 0; j < myMeshHead->elementsSet.entityblocks[i].numElementsInBlock; ++j) {
                    infile >> myMeshHead->elementsSet.entityblocks[i].elementsIndex[j];
                    size_t tempIndex = myMeshHead->elementsSet.entityblocks[i].elementsIndex[j];
                    int dim = myMeshHead->elementsSet.entityblocks[i].entityDim;
                    myMeshHead->elements[tempIndex].entityDim = dim;
                    myMeshHead->elements[tempIndex].nodesIndex = new size_t[dim + 1];
                    for (int k = 0; k < dim + 1; ++k) {
                        infile >> myMeshHead->elements[tempIndex].nodesIndex[k];
                        size_t tempNodesIndex = myMeshHead->elements[tempIndex].nodesIndex[k];
                        myMeshHead->nodes[tempNodesIndex].connectedElements.push_back(tempIndex);
                    }
                }
            }
            vector<int> tempContianer;
            for (int i = 1; i <= myMeshHead->elementsSet.numElements; ++i) {
                tempContianer.clear();
                for (int j = 0; j < myMeshHead->elements[i].entityDim + 1; ++j) {
                    size_t tempNodesIndex = myMeshHead->elements[i].nodesIndex[j];
                    for (vector<int>::iterator iter = myMeshHead->nodes[tempNodesIndex].connectedElements.begin(); iter != myMeshHead->nodes[tempNodesIndex].connectedElements.end(); ++iter) {
                        if (myMeshHead->elements[i].entityDim == myMeshHead->elements[*iter].entityDim)
                            tempContianer.push_back(*iter);
                    }
                }
                for (vector<int>::iterator iter = tempContianer.begin(); iter != tempContianer.end(); ++iter) {
                    if (count(tempContianer.begin(), tempContianer.end(), *iter) >= myMeshHead->elements[i].entityDim) {
                        if (*iter != i)
                            myMeshHead->elements[i].nearbyElement.insert(*iter);
                    }
                }
            }
            // cout << "$Elements\n";
            // cout << myMeshHead->elementsSet.numEntityBlocks << " ";
            // cout << myMeshHead->elementsSet.numElements << " ";
            // cout << myMeshHead->elementsSet.minElementTag << " ";
            // cout << myMeshHead->elementsSet.maxElementTag << " " << endl;
            // for (int i = 0; i < myMeshHead->elementsSet.numEntityBlocks; ++i) {
            //     cout << myMeshHead->elementsSet.entityblocks[i].entityDim << " ";
            //     cout << myMeshHead->elementsSet.entityblocks[i].entityTag << " ";
            //     cout << myMeshHead->elementsSet.entityblocks[i].elementType << " ";
            //     cout << myMeshHead->elementsSet.entityblocks[i].numElementsInBlock << " " << endl;
            //     for (int j = 0; j < myMeshHead->elementsSet.entityblocks[i].numElementsInBlock; ++j) {
            //         size_t tempIndex = myMeshHead->elementsSet.entityblocks[i].elementsIndex[j];
            //         cout << myMeshHead->elementsSet.entityblocks[i].elementsIndex[j] << " ";
            //         int dim = myMeshHead->elementsSet.entityblocks[i].entityDim;
            //         for(int k = 0; k < dim + 1; ++k){
            //             cout << myMeshHead->elements[tempIndex].nodesIndex[k] << " ";
            //         }
            //         // for (set<int>::iterator iter = myMeshHead->elements[tempIndex].nearbyElement.begin(); iter != myMeshHead->elements[tempIndex].nearbyElement.end(); ++iter) {
            //         //     cout << *iter << " ";
            //         // }
            //         cout << endl;
            //     }
            // }
            // cout << "$EndElements\n";
        }
    }
    infile.close();
}

void HotPass(meshHead *myMeshHead, double initTemp, double hotOriginTemp, int hotOriginElement, int passStep) {
    size_t elementCount = myMeshHead->elementsSet.numElements;
    double *originTempStr;
    double *currentStr;
    originTempStr = new double[elementCount + 1];
    currentStr = new double[elementCount + 1];
    for (int i = 1; i <= elementCount; ++i) {
        originTempStr[i] = initTemp;
        currentStr[i] = initTemp;
    }
    originTempStr[hotOriginElement] = hotOriginTemp;
    currentStr[hotOriginElement] = hotOriginTemp;
    for (int step = 0; step < passStep; ++step) {
        for (int i = 1; i <= elementCount; ++i) {
            for (set<int>::iterator iter1 = myMeshHead->elements[i].nearbyElement.begin(); iter1 != myMeshHead->elements[i].nearbyElement.end(); ++iter1) {
                currentStr[i] += originTempStr[*iter1];
            }
            currentStr[i] = currentStr[i] / (myMeshHead->elements[i].nearbyElement.size() + 1);
        }
        for (int i = 1; i <= elementCount; ++i) {
            originTempStr[i] = currentStr[i];
        }
        cout << "step " << step + 1<< ": " << endl;
        for (int i = 1; i <= elementCount; ++i) {
            if(originTempStr[i] != initTemp){
                cout << i << ": " << originTempStr[i] << endl;
            }
        }
    }

    // map<int,double> updateTempTable;
    // updateTempTable.insert({hotOriginElement,hotOriginTemp});
    // for(int i = 0; i < passStep; ++i){
    //     for(map<int,double>::iterator iter = updateTempTable.begin(); iter != updateTempTable.end() ; ++iter){
    //         int everChangedElement = iter->first;
    //         for(set<int>::iterator iter1 = myMeshHead->elements[everChangedElement].nearbyElement.begin(); iter1 != myMeshHead->elements[everChangedElement].nearbyElement.end(); ++iter1){

    //         }
    //     }
    // }
}

int main() {
    meshHead realMeshHead;
    meshHead *myMeshHead = &realMeshHead;
    const char *filename = "t4.msh";
    readMsh(filename, myMeshHead);
    double initTemp = 10;
    double hotOriginTemp = 2500000;
    size_t hotOriginElement = 1415;
    int passStep = 100;
    HotPass(myMeshHead, initTemp, hotOriginTemp, hotOriginElement, passStep);
    return 0;
}