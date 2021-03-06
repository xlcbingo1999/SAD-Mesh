#ifndef DATASTRUCTGMSH_HPP
#define DATASTRUCTGMSH_HPP

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <vector>

using std::map;
using std::priority_queue;
using std::set;
using std::string;
using std::vector;

struct PointDescription {
    int pointTag;
    double x;
    double y;
    double z;
    int physicalTag;
};

struct CurveDescription {
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

class PointCatalog {
private:
    map<int,PointDescription*> catalogMap;
public:
    PointCatalog();
    ~PointCatalog();
    void insert(std::pair<int,PointDescription*>);
    PointDescription* getDescription(int);
};

class CurveCatalog {
private:
    map<int,CurveDescription*> catalogMap;
public:
    CurveCatalog();
    ~CurveCatalog();
    void insert(std::pair<int,CurveDescription*>);
    CurveDescription* getDescription(int);    
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

struct ElementAttribute {
    size_t elementIndex;
    vector<string> elementAttributeName;
    vector<string> elementAttributeClass;
    vector<string> elementAttributeValue;
};

struct PartitionedEntities {
    size_t numPartitions;
    size_t numGhostEntities;
    int ghostEntityTag;
    int partition;
};

struct meshHead {
    string version;
    int filetype;
    int datalength;
    size_t pointsNum;
    size_t curvesNum;
    size_t surfacesNum;
    size_t volumesNum;
    PointCatalog pointCatalog;
    CurveCatalog curveCatalog;
    Surface *surfaces;
    Volume *volumes;
    NodeSet nodesSet;
    Node *nodes;
    ElementSet elementsSet;
    Element *elements;
    ElementAttribute *elementAttributesTable;
};

extern meshHead realMeshHead;

#endif