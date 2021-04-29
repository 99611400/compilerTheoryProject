#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <cstring>
#include <map>
#include "productionForm.h"



std::map<std::string, std::vector<std::string>> *getFirstMap(std::map<std::string, std::vector<std::string>> *pMap);
std::map<std::string, std::vector<std::string>> *getFollowMap(std::map<std::string, std::vector<std::string>> *pMap);
std::map<productionForm, std::vector<std::string>> *
getSelectMap(std::map<std::string, std::vector<std::string>> *,
             std::map<std::string, std::vector<std::string>> *,
             std::map<productionForm, std::vector<std::string>> *);

using std::cin;
using std::cout;
using std::endl;

std::string trim(std::string& str){
    str.erase(0, str.find_first_not_of(" \t")); // 去掉头部空格
    str.erase(str.find_last_not_of(" \t") + 1); // 去掉尾部空格
    return str;
}


std::string removeSpace(std::string str) {
    for(auto iter = str.begin();iter != str.end();iter ++){
        if(isblank(*iter) != 0){  	 //isblank判断是否为制表符或者空格，是的话返回非零值
            str.erase(iter--); // --是因为重新判断一下填过来的字符
        }
    }
    return str;

}


std::vector<std::string> * split(const std::string& str, const std::string& delim) {
    std::vector<std::string> *res = new std::vector<std::string>();
    if("" == str) return res;
    //先将要切割的字符串从std::string类型转换为char*类型
    char * strs = new char[str.length() + 1] ; //不要忘了
    strcpy(strs, str.c_str());

    char * d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        std::string s = p; //分割得到的字符串转换为std::string类型
        res->push_back(s); //存入结果数组
        p = strtok(NULL, d);
    }
    return res;
}





//terminatorVector, noneTerminatorVector, productionFormVector

void getInput(std::vector<std::string> *terminatorVector,
              std::vector<std::string> *noneTerminatorVector,
              std::vector<productionForm> *productionFormVector) {
    terminatorVector = new std::vector<std::string>();
    noneTerminatorVector = new std::vector<std::string>();
    productionFormVector = new std::vector<productionForm>();

    std::ifstream myfile("../input");
    std::string temp;
    if (!myfile.is_open())
    {
        cout << "未成功打开文件" << endl;
    }
    int i = 0;
    while(getline(myfile,temp))
    {
        if (i==0){
            terminatorVector = split(temp," ");
            std::cout<<"所有的终结符有："<<temp<<std::endl;
        } else if(i==1){
            noneTerminatorVector = split(temp," ");
            cout<<"所有的非终结符有："<<temp<<endl;
        } else if (i >=2) {
            if (i == 2)
                cout << "产生式有：" << endl;
            std::string afterTrimStr = removeSpace(temp);
            if (afterTrimStr != "end") {
                std::vector<std::string> *tempVector = split(afterTrimStr, "->");
                productionFormVector->push_back(
                        *new productionForm(afterTrimStr, (*tempVector)[0], split((*tempVector)[1], "|")));
                cout << temp << endl;
            }
        }
        i++;
    }

    cout<<"总共有 "<<productionFormVector->size()<<" 个产生式"<<endl;
    // 打印输出 处理前的内容
    int m =0;
    for(productionForm ele : *productionFormVector){
        cout<<m+1<<".";
        m++;
        ele.print();
    }
    myfile.close();
}


int main() {
    // 定义变量
    std::vector<std::string> *terminatorVector = nullptr; //终结符的集合  不重复
    std::vector<std::string> *noneTerminatorVector = nullptr; //非终结符的集合  不重复
    std::vector<productionForm> *productionFormVector = nullptr;
    std::map<std::string,std::vector<std::string>> *firstMap = nullptr;  // 所有非终结符的first集
    std::map<std::string,std::vector<std::string>> *followMap = nullptr;  // 所有非终结符的follow集
    std::map<productionForm,std::vector<std::string>> *selectMap = nullptr; // 所有产生式的select集 用于构造分析表


    // 调用函数
    getInput(terminatorVector, noneTerminatorVector, productionFormVector);
    firstMap = getFirstMap(firstMap);
    followMap = getFollowMap(followMap);
    selectMap = getSelectMap(firstMap, followMap, selectMap);


    // 释放空间
    free(terminatorVector);
    free(noneTerminatorVector);
    free(productionFormVector);
    free(firstMap);
    free(followMap);
    free(selectMap);

    return 0;
}




std::map<std::string, std::vector<std::string>> *
getFirstMap(std::map<std::string,std::vector<std::string>> *firstMap) {
    firstMap = new std::map<std::string, std::vector<std::string>>();


    return firstMap;
}

std::map<std::string, std::vector<std::string>> *
getFollowMap(std::map<std::string, std::vector<std::string>> *followMap) {
    followMap = new std::map<std::string, std::vector<std::string>>();

    return followMap;
}

std::map<productionForm, std::vector<std::string>> *
getSelectMap(std::map<std::string, std::vector<std::string>> *firstMap,
             std::map<std::string, std::vector<std::string>> *followMap,
             std::map<productionForm, std::vector<std::string>> *selectMap) {

    selectMap = new std::map<productionForm, std::vector<std::string>>();


    return selectMap;
}
