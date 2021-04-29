//
// Created by yhn on 2021/4/26.
//

#ifndef C___ABSTRACT_SYNTAX__CODE_C_LL1_H
#define C___ABSTRACT_SYNTAX__CODE_C_LL1_H
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <cstring>
#include <map>
#include "productionForm.h"
using std::cin;
using std::cout;
using std::endl;


class LL1 {
private:
    std::string removeSpace(std::string str);
    std::vector<std::string> * split(const std::string& str, const std::string& delim);
    std::vector<std::string> *splitByTerminatorAndNoneTer(std::string str);
    // 判断一个非终结符 可不可以推出空
    bool judgeNoneOrTerminatorList2None(std::vector<std::string> noneTerminatorStr);
    bool judgeAStrInStringVector(const std::string& str,std::vector<std::string>* vector);
    bool judgeNoneTerminator2None(std::string noneTerminatorStr);

    void getInput(const std::string& path);
    void calFirstMap();

    void recursionCalNoneTerminatorFirst(std::set<std::string> *firstSet, // 该非终结符的first集合 如果没有会new一个
                                         std::string currentNoneTerminatorStr, //  当前非终结符
                                         std::vector<std::vector<std::string>> *NoneTerminatorRight); //  当前非中介符对应的右端
    void calFollowMap();
    void calSelectMap();
    void printByNonTerminator(std::string );

    std::string startNonTerminator;
    std::vector<std::string> *terminatorVector; //终结符的集合  不重复
    std::vector<std::string> *noneTerminatorVector; //非终结符的集合  不重复
    std::vector<productionForm> *productionFormVector;
    std::map<std::string,productionForm> * expressionMap;
    std::map<std::string,std::set<std::string>*> *firstMap;  // 所有非终结符的first集
    std::map<std::string,std::set<std::string>*> *followMap;  // 所有非终结符的follow集
    std::map<std::string,std::vector<std::vector<std::string>*>*> *selectMap;  // 所有非终结符的follow集

    std::vector<std::string> *mergeTerAndNoneVector;  // 合并非终结符与终结符的vector
public:

    void printAll();
    explicit LL1(std::string path);
    ~LL1();

};


#endif //C___ABSTRACT_SYNTAX__CODE_C_LL1_H
