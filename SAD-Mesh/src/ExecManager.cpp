#ifndef EXECMANAGER_CPP
#define EXECMANAGER_CPP
#include "ExecManager.hpp"

ExecManager execManager;

ExecManager::ExecManager() {

}

ExecManager::~ExecManager() {
    
}

ArguMap* ExecManager::getNewArguMap(unsigned long long num, double init = 0) {
    struct ArguMap* map = new ArguMap();
    map->num = num;
    for (int i = 0; i < num; i++) {
        map->value.push_back(init);
    }
    return map;
}

ArguList* ExecManager::getNewArguList(string name) {
    struct ArguList* list = new ArguList();
    list->name = name;
    list->num = 0;
    return list;
}

double ExecManager::GetArguData(const string str) {
    std::istringstream iss(str);
    double num;
    iss >> num;
    return num;
}

double ExecManager::power(int v, int p) {
    double r = 1;
    for (int i = 0; i < p; i++) {
        r *= v;
    }
    return r;
}

void ExecManager::sortWithArgus(vector<int>& near, const struct ElementAttribute* argus, int argu_id) {
    int size = near.size();
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (GetArguData(argus[near[i]].elementAttributeValue[argu_id]) > GetArguData(argus[near[j]].elementAttributeValue[argu_id])) {
                int temp = near[i];
                near[i] = near[j];
                near[j] = temp;
            }
        }
    }
}

void ExecManager::sortByArgus(vector<int> &near, const struct ArguMap* argu) {
    int size = near.size();
    for (int i = 0; i < size; i ++) {
        for (int j = i+1; j < size; j ++) {
            if (argu->value[near[i]] > argu->value[near[j]]) {
                int temp = near[i];
                near[i] = near[j];
                near[j] = temp;
            }
        }
    }
}

// Express 实现

int ExecManager::add_coef(struct Coefficient * c) {
    this->coefs.push_back(c);
    return 0;
}

int ExecManager::add_attr(enum Attr a) {
    this->attr.push_back(a);
    return 0;
}

int ExecManager::set_form(Form f) {
    this->form = f;
    return 0;
}

double ExecManager::Execute(struct ArguMap** argus, int index) {
    
    // 存储每个相邻element带来的影响
    vector<double> res;
    int attr_id = 0;
    int argu_id = 0;
    int coef_id = 0;

    // 找到相邻点
    vector<int> near;
    near.push_back(index);
    res.push_back(0);
    meshHead *mesh = &realMeshHead;
    for (set<int>::iterator iter1 = mesh->elements[index].nearbyElement.begin(); iter1 != mesh->elements[index].nearbyElement.end(); ++iter1) {
        // printf("test\n");
        near.push_back(* iter1);
        res.push_back(0);
    }

    int size = res.size();
    while (this->attr[attr_id] != END) {
        // printf("attr %d", attr_id);
        if (this->attr[attr_id] == WEI) {
            sortByArgus(near, argus[argu_id]);
            
            argu_id ++;
        } else if (this->attr[attr_id] == ADDARGU) { 
            for (int i = 0; i < size; i ++) {
                res[i] += (this->coefs[coef_id]->value[i] * argus[argu_id]->value[near[i]]);
            }

            coef_id ++;
            argu_id ++;
        } else if (this->attr[attr_id] == MULARGU) {
            for (int i = 0; i < size; i ++) {
                res[i] *= (this->coefs[coef_id]->value[i] * argus[argu_id]->value[near[i]]);
            }
            
            coef_id ++;
            argu_id ++;
        } else if (this->attr[attr_id] == POW) {
            for (int i = 0; i < size; i ++) {
                res[i] = power(res[i], this->coefs[coef_id]->value[i]);
            }

            coef_id ++;
        } else if (this->attr[attr_id] == EXP) {
            for (int i = 0; i < size; i ++) {
                res[i] = power(this->coefs[coef_id]->value[i], res[i]);
            }

            coef_id ++;
            argu_id ++;
        } else if (this->attr[attr_id] == LOG) { 
            for (int i = 0; i < size; i ++) {
                res[i] = log(res[i])/log(this->coefs[coef_id]->value[i]);
            }

            coef_id ++;
            argu_id ++;
        } else {
            printf("There maybe something error with enum Attr");
        }

        attr_id ++;
    }

    // 整理出结果
    double result = 0;
    if(this->form == AVG) {
        for (int i = 0; i < size ; i ++) {
            result += res[i];
        }
        result /= size;
    } else if (this->form == SUM) {
        for (int i = 0; i < size ; i ++) {
            result += res[i];
        }
    } else {
        printf("There maybe something error with enum Form");
    }

    return result;
}

#endif