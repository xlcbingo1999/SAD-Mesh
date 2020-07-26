#ifndef EXECMANAGER_HPP
#define EXECMANAGER_HPP

#include "DataStructGMsh.hpp"
#include <vector>
#include <sstream>
#include <math.h>


enum Attr{END, WEI, ADDARGU, MULARGU, POW, EXP, LOG};
enum Form{SUM, AVG};

// 存放参数的结构体
struct ArguMap {
    int num;
    vector<double> value;
}; 

struct ArguList {
    int num;
    string name;
    vector<int> list;
    vector<string> value;
};

// 系数
struct Coefficient {
    vector<double> value;
};

class ExecManager {
private:
    vector<struct Coefficient *> coefs;
    vector<enum Attr> attr;
    enum Form form;

public:
    int add_coef(struct Coefficient * c); // 添加系数
    int add_attr(enum Attr a); // 添加属性
    int set_form(Form f); // 设置计算模式
    double Execute(struct ArguMap** argus, int index);  
    ArguMap* getNewArguMap(unsigned long long num, double init); // 获取一个初始化完成的map
    ArguList* getNewArguList(string name); // 获取一个初始化完成的list
    double GetArguData(const string str); // 获取attribute数据
    double power(int v, int p);
    void sortWithArgus(vector<int>& near, const struct ElementAttribute* argus, int argu_id);
    void sortByArgus(vector<int> &near, const struct ArguMap* argu);
};
extern ExecManager execManager;


#endif