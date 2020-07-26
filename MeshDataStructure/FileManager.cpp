#ifndef FILEMANAGER_CPP
#define FILEMANAGER_CPP

#include "FileManager.hpp"

FileManager fileManager;
ifstream infile;

FileManager::FileManager() {
    fileName = "";
    dataStructOptionCount = 1;
    dataStructOption = new string[dataStructOptionCount];
    dataStructOption->assign("GMsh");
    currentStruct = "";
}

FileManager::~FileManager() {
}

bool FileManager::receiveImportApplication(string to_open) {
    cout << "to_open: " << to_open << "\n";
    if (to_open == "")
        return false;
    else {
        fileName = to_open;
        cout << "Your Application is received.\n";
        return true;
    }
}

int FileManager::getDataStructOptionCount() {
    return dataStructOptionCount;
}

string FileManager::getDataStructOption(int did) {
    if (did >= dataStructOptionCount)
        return NULL;
    else {
        return dataStructOption[did];
    }
}

bool FileManager::usingDataStruct(int did) {
    if (did >= dataStructOptionCount)
        return false;
    else {
        currentStruct = dataStructOption[did];
        return true;
    }
}

bool FileManager::importPoint() {
    meshHead *myMeshHead = &realMeshHead;
    infile >> myMeshHead->pointsNum >> myMeshHead->curvesNum >> myMeshHead->surfacesNum >> myMeshHead->volumesNum;
    int pointTag;
    double x, y, z;
    int physicalTag;
    for (int i = 0; i < myMeshHead->pointsNum; ++i) {
        infile >> pointTag;
        infile >> x;
        infile >> y;
        infile >> z;
        infile >> physicalTag;
        PointDescription *point = new PointDescription;
        point->pointTag = pointTag;
        point->x = x;
        point->y = y;
        point->z = z;
        point->physicalTag = physicalTag;
        myMeshHead->pointCatalog.insert(std::pair<int, PointDescription *>(point->pointTag, point));
    }
    return true;
}

bool FileManager::importCurve() {
    meshHead *myMeshHead = &realMeshHead;
    int viceInt;
    int curveTag;
    double startx;
    double starty;
    double startz;
    double endx;
    double endy;
    double endz;
    int physicalTag;
    size_t numPhysicalTags;
    size_t numBoundingPoints;
    int startPoint;
    int endPoint;
    for (int i = 0; i < myMeshHead->curvesNum; ++i) {
        infile >> curveTag;
        infile >> startx >> starty >> startz;
        infile >> endx >> endy >> endz;
        infile >> physicalTag >> numBoundingPoints;
        infile >> startPoint;
        infile >> viceInt;
        endPoint = (0 - viceInt);
        CurveDescription *curve = new CurveDescription;
        curve->curveTag = curveTag;
        curve->startx = startx;
        curve->starty = starty;
        curve->startz = startz;
        curve->endx = endx;
        curve->endy = endy;
        curve->endz = endz;
        curve->physicalTag = physicalTag;
        curve->numBoundingPoints = numBoundingPoints;
        curve->startPoint = startPoint;
        curve->endPoint = endPoint;
        myMeshHead->curveCatalog.insert(std::pair<int, CurveDescription *>(curve->curveTag, curve));
    }
    return true;
}

bool FileManager::importSurface() {
    meshHead *myMeshHead = &realMeshHead;
    int viceInt;
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
                    myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curveCatalog.getDescription(viceInt - 1)->startPoint;
                    tempJ++;
                    myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curveCatalog.getDescription(viceInt - 1)->endPoint;
                    tempJ++;
                } else {
                    viceInt = 0 - myMeshHead->surfaces[i].vectors[j];
                    myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curveCatalog.getDescription(viceInt - 1)->endPoint;
                    tempJ++;
                    myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curveCatalog.getDescription(viceInt - 1)->startPoint;
                    tempJ++;
                }
            } else if (j == myMeshHead->surfaces[i].numBoundingCurves - 1) {
            } else {
                if (myMeshHead->surfaces[i].vectors[j] > 0) {
                    viceInt = myMeshHead->surfaces[i].vectors[j];
                    myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curveCatalog.getDescription(viceInt - 1)->endPoint;
                    tempJ++;
                } else {
                    viceInt = 0 - myMeshHead->surfaces[i].vectors[j];
                    myMeshHead->surfaces[i].pointsLine[tempJ] = myMeshHead->curveCatalog.getDescription(viceInt - 1)->startPoint;
                    tempJ++;
                }
            }
        }
    }
    return true;
}

bool FileManager::importVolume() {
    meshHead *myMeshHead = &realMeshHead;
    int viceInt;
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
    }
    return true;
}

bool FileManager::importNode() {
    meshHead *myMeshHead = &realMeshHead;
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
    return true;
}

bool FileManager::importElement() {
    meshHead *myMeshHead = &realMeshHead;
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
    return true;
}

bool FileManager::beginImporting() {
    meshHead *myMeshHead = &realMeshHead;
    infile.open(fileName);
    string str;
    bool result;
    cout << "Reading from the file.\n";
    int index;
    float viceFloat;
    int viceInt;
    while (infile.good()) {
        getline(infile, str);
        if (str == "$MeshFormat") {
            infile >> myMeshHead->version >> myMeshHead->filetype >> myMeshHead->datalength;
        } else if (str == "$Entities") {
            if ((result = importPoint()) == false) return false;
            if ((result = importCurve()) == false) return false;
            if ((result = importSurface()) == false) return false;
            if ((result = importVolume()) == false) return false;
        } else if (str == "$Nodes") {
            if ((result = importNode()) == false) return false;
        } else if (str == "$Elements") {
            if ((result = importElement()) == false) return false;
        }
    }
    infile.close();
    return true;
}

void FileManager::outputPoint() {
    meshHead *myMeshHead = &realMeshHead;
    for (int i = 0; i < myMeshHead->pointsNum; ++i) {
        cout << setprecision(16) << myMeshHead->pointCatalog.getDescription(i + 1)->pointTag << " " << myMeshHead->pointCatalog.getDescription(i + 1)->x << " " << myMeshHead->pointCatalog.getDescription(i + 1)->y << " " << myMeshHead->pointCatalog.getDescription(i + 1)->z << " " << myMeshHead->pointCatalog.getDescription(i + 1)->physicalTag << "\n";
    }
}

void FileManager::outputCurve() {
    meshHead *myMeshHead = &realMeshHead;
    for (int i = 0; i < myMeshHead->curvesNum; ++i) {
        cout << myMeshHead->curveCatalog.getDescription(i + 1)->curveTag << setprecision(16) << " ";
        cout << myMeshHead->curveCatalog.getDescription(i + 1)->startx << " " << myMeshHead->curveCatalog.getDescription(i + 1)->starty << " " << myMeshHead->curveCatalog.getDescription(i + 1)->startz << " ";
        cout << myMeshHead->curveCatalog.getDescription(i + 1)->endx << " " << myMeshHead->curveCatalog.getDescription(i + 1)->endy << " " << myMeshHead->curveCatalog.getDescription(i + 1)->endz << " ";
        cout << myMeshHead->curveCatalog.getDescription(i + 1)->physicalTag << " " << myMeshHead->curveCatalog.getDescription(i + 1)->numBoundingPoints << " " << myMeshHead->curveCatalog.getDescription(i + 1)->startPoint << " " << myMeshHead->curveCatalog.getDescription(i + 1)->endPoint << "\n";
    }
}

void FileManager::outputSurface() {
    meshHead *myMeshHead = &realMeshHead;
    for (int i = 0; i < myMeshHead->surfacesNum; ++i) {
        cout << myMeshHead->surfaces[i].surfaceTag << setprecision(16) << " ";
        cout << myMeshHead->surfaces[i].startx << " " << myMeshHead->surfaces[i].starty << " " << myMeshHead->surfaces[i].startz << " ";
        cout << myMeshHead->surfaces[i].endx << " " << myMeshHead->surfaces[i].endy << " " << myMeshHead->surfaces[i].endz << " ";
        cout << myMeshHead->surfaces[i].physicalTag << " " << myMeshHead->surfaces[i].numBoundingCurves << " ";
        for (int j = 0; j < myMeshHead->surfaces[i].numBoundingCurves; ++j) {
            cout << myMeshHead->surfaces[i].vectors[j] << " ";
        }
        // Unfinished!
        cout << "\npointsLine: ";
        for (int j = 0; j < myMeshHead->surfaces[i].numBoundingCurves; ++j) {
            cout << myMeshHead->surfaces[i].pointsLine[j] << " ";
        }
        cout << "\n";
    }
}

void FileManager::outputVolume() {
    meshHead *myMeshHead = &realMeshHead;
    for (int i = 0; i < myMeshHead->volumesNum; ++i) {
        cout << myMeshHead->volumes[i].volumeTag << setprecision(16) << " ";
        cout << myMeshHead->volumes[i].startx << " " << myMeshHead->volumes[i].starty << " " << myMeshHead->volumes[i].startz << " ";
        cout << myMeshHead->volumes[i].endx << " " << myMeshHead->volumes[i].endy << " " << myMeshHead->volumes[i].endz << " ";
        cout << myMeshHead->volumes[i].numPhysicalTags << " " << myMeshHead->volumes[i].physicalTag << " " << myMeshHead->volumes[i].numBoundngSurfaces << " ";
        for (int j = 0; j < myMeshHead->volumes[i].numBoundngSurfaces; ++j) {
            cout << myMeshHead->volumes[i].surfaces[j] << " ";
        }
        cout << "\n";
    }
}

void FileManager::outputNode() {
    meshHead *myMeshHead = &realMeshHead;
    cout << "$Nodes\n"
         << setprecision(16);
    cout << myMeshHead->nodesSet.numEntityBlocks << " ";
    cout << myMeshHead->nodesSet.numNodes << " ";
    cout << myMeshHead->nodesSet.minNodeTag << " ";
    cout << myMeshHead->nodesSet.maxNodeTag << "\n";
    for (int i = 0; i < myMeshHead->nodesSet.numEntityBlocks; ++i) {
        cout << myMeshHead->nodesSet.entityblocks[i].entityDim << " ";
        cout << myMeshHead->nodesSet.entityblocks[i].entityTag << " ";
        cout << myMeshHead->nodesSet.entityblocks[i].parametric << " ";
        cout << myMeshHead->nodesSet.entityblocks[i].numNodesInBlock << "\n";
        for (int j = 0; j < myMeshHead->nodesSet.entityblocks[i].numNodesInBlock; ++j) {
            cout << myMeshHead->nodesSet.entityblocks[i].nodesIndex[j] << "\n";
        }
        for (int j = 0; j < myMeshHead->nodesSet.entityblocks[i].numNodesInBlock; ++j) {
            size_t tempNodesIndex = myMeshHead->nodesSet.entityblocks[i].nodesIndex[j];
            cout << myMeshHead->nodes[tempNodesIndex].x << " ";
            cout << myMeshHead->nodes[tempNodesIndex].y << " ";
            cout << myMeshHead->nodes[tempNodesIndex].z << "\n";
        }
    }
    cout << "$EndNodes\n";
}

void FileManager::outputElement() {
    meshHead *myMeshHead = &realMeshHead;
    cout << "$Elements\n";
    cout << myMeshHead->elementsSet.numEntityBlocks << " ";
    cout << myMeshHead->elementsSet.numElements << " ";
    cout << myMeshHead->elementsSet.minElementTag << " ";
    cout << myMeshHead->elementsSet.maxElementTag << "\n";
    for (int i = 0; i < myMeshHead->elementsSet.numEntityBlocks; ++i) {
        cout << myMeshHead->elementsSet.entityblocks[i].entityDim << " ";
        cout << myMeshHead->elementsSet.entityblocks[i].entityTag << " ";
        cout << myMeshHead->elementsSet.entityblocks[i].elementType << " ";
        cout << myMeshHead->elementsSet.entityblocks[i].numElementsInBlock << "\n";
        for (int j = 0; j < myMeshHead->elementsSet.entityblocks[i].numElementsInBlock; ++j) {
            size_t tempIndex = myMeshHead->elementsSet.entityblocks[i].elementsIndex[j];
            cout << myMeshHead->elementsSet.entityblocks[i].elementsIndex[j] << " ";
            int dim = myMeshHead->elementsSet.entityblocks[i].entityDim;
            for (int k = 0; k < dim + 1; ++k) {
                cout << myMeshHead->elements[tempIndex].nodesIndex[k] << " ";
            }
            // for (set<int>::iterator iter = myMeshHead->elements[tempIndex].nearbyElement.begin(); iter != myMeshHead->elements[tempIndex].nearbyElement.end(); ++iter) {
            //     cout << *iter << " ";
            // }
            cout << "\n";
        }
    }
    cout << "$EndElements\n";
}

void FileManager::outputDataStruct() {
    meshHead *myMeshHead = &realMeshHead;
    cout << "$MeshFormat\n"
         << myMeshHead->version << " " << myMeshHead->filetype << " " << myMeshHead->datalength << "\n$EndMeshFormat\n";
    cout << "$Entities\n"
         << myMeshHead->pointsNum << " " << myMeshHead->curvesNum << " " << myMeshHead->surfacesNum << " " << myMeshHead->volumesNum << "\n";
    outputPoint();
    outputCurve();
    outputSurface();
    outputVolume();
    outputNode();
    outputElement();
}

#endif