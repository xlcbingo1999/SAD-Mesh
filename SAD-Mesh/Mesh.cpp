
#include "User.hpp"
/***********************************************************************
 * TO-DO: 请在本区域加入你想要加入的头文件，本程序不进行冲突确保
 * 
 */
#include <iostream>
#include <fstream>
using namespace std;
fstream infile2;
/***********************************************************************
 * TO-DO: 请在本区域自定义你的diyUser类
 * 
 */
class diyUser: public User {
private:
    /* data */
    bool outputToFile = false;
    bool odd = false;
public:
    diyUser();
    ~diyUser();
    meshHead* getMeshStructHead();
    size_t getMeshElementCount();

    ArguMap* initAllElementValue(double d);

    int add_coef(struct Coefficient * c);
    int add_attr(enum Attr a); 
    int set_form(Form f);

    double Execute(struct ArguMap** argus, int index); // virtual 函数，必须实现

    void displayResult(int ref, double ret); // virtual 函数，必须实现

    void beginSave(); // virtual 函数，必须实现
    void saveItem(double ret); // virtual 函数，必须实现
    void endSave(); // virtual 函数，必须实现

    void setOutputToFile();
    bool getOutputToFile();
};

diyUser::diyUser()
{
    
}

diyUser::~diyUser()
{

}

meshHead* diyUser::getMeshStructHead() {
    return &realMeshHead;
}

size_t diyUser::getMeshElementCount() {
    return getMeshStructHead()->elementsSet.numElements;
}

ArguMap* diyUser::initAllElementValue(double init) {
    return execManager.getNewArguMap(getMeshElementCount(), init);
}

int diyUser::add_coef(struct Coefficient * c) {
    return execManager.add_coef(c);
}

int diyUser::add_attr(enum Attr a) {
    return execManager.add_attr(a);
} 

int diyUser::set_form(Form f) {
    return execManager.set_form(f);
}

double diyUser::Execute(struct ArguMap** argus, int index) {
    return execManager.Execute(argus, index);
}

void diyUser::displayResult(int ref, double ret) {
    printf("%d %.2f\n",ref ,ret);
}

void diyUser::setOutputToFile() {
    outputToFile = true;
}

bool diyUser::getOutputToFile() {
    return outputToFile;
}

void diyUser::beginSave() {
    infile2.open("result.txt", ios::out);
}
    
void diyUser::saveItem(double ret) {
    infile2 << ret;
    if(odd)
        infile2 << "\n";
    else 
        infile2 << " ";
    odd = !odd;
}

    
void diyUser::endSave() {
    infile2.close();
}

/***********************************************************************
 * 以下主函数即是用户需要些的kernel_function
 * 以热传导仿真实验为例
 */
int main() {
    // -------------
    // 读入文件
    // -------------
    diyUser MAS;
    // 读入
    if(MAS.importApplication() == false) return -1;
    if(MAS.chooseDataStruct() == false) return -1;
    if(MAS.beginImporting() == false) return -1;
    // MAS.outputDataStruct(); // 输出
    MAS.diy();

/***********************************************************************
 * TO-DO: 请在本区域完成参数的定义和后台Mesh数据的拉取
 * 
 */
    meshHead *myMeshHead = MAS.getMeshStructHead();
    size_t elementCount = MAS.getMeshElementCount();

    printf("element count: %d\n", elementCount);

    struct ArguMap* maplist[1]; // 存放参数的结构体
    double init = 10;    // 设置初始热量
    maplist[0] = MAS.initAllElementValue(init);
    size_t origin = 1415; // 设置热源
    maplist[0]->value[origin] = 250;

/*********************************************************************
 * TO-DO: 请在本区域完成计算公式的设置
 * 示例：计算公式设置: x[n+1]= (x[n] + x1[n] + x2[n] + x3[n])/4
 * x[n]: n时刻编号为x的element的温度
 * x1,x2,x3为x的相邻节点，不一定全部存在
 */

    struct Coefficient* coef1 = new Coefficient[1];
    // 添加系数
    for (int i = 0; i < 4; i ++) coef1->value.push_back(1);
    MAS.add_coef(coef1);
    // 设置运算属性
    MAS.add_attr(ADDARGU);
    // 设置结果计算方法，即所有相邻原件的影响的平均值
    MAS.add_attr(END);
    MAS.set_form(AVG);

/*********************************************************************
 * TO-DO: 定义运算顺序(以后可以考虑独立出来成为我们系统提供的接口)
 * 
 */
    
/*********************************************************************
 * TO-DO: 请在这里使用Execute进行计算、显示和结果导出
 * 例子：计算出经过第一个时间周期后，温度产生变化的element变化后的温度
 */
    MAS.setOutputToFile();
    if(MAS.getOutputToFile() == true) {
        MAS.beginSave();
    }
    for (int i = 0; i < elementCount; i ++) {
        double res = MAS.Execute(maplist, i); 
        if (res != init) {
            MAS.displayResult(i+1,res);
        }
        MAS.saveItem(i+1);
        MAS.saveItem(res);
    }
    if(MAS.getOutputToFile() == true) {
        MAS.endSave();
    }
    // Execute(&express, maplist, myMeshHead, 0);

    return 0;
}