//
// Created by yhn on 2021/4/26.
//

#ifndef C___ABSTRACT_SYNTAX__CODE_C_LL1_H
#define C___ABSTRACT_SYNTAX__CODE_C_LL1_H
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <list>
#include <cstring>
#include <map>
#include <stack>
#include "productionForm.h"
using std::cin;
using std::cout;
using std::endl;


class LL1 {
private:
    std::string removeSpace(std::string str);  // 删除空格 自己写的
    std::vector<std::string> * split(const std::string& str, const std::string& delim); //  注意这个有bug  网上找到 根据符号分割字符串 返回一个vector数组    注意这个有bug 分不清楚 -> 和- 这种共同前缀的
    std::vector<std::string> *splitByTerminatorAndNoneTer(std::string str);   //  因为为了方便 非终结符和终结符的长度可以不是1 所以需要先将这里边的字符分割开来

    bool judgeNoneOrTerminatorList2None(std::vector<std::string> noneTerminatorStr);  // 给出传入一个非终结符和终结符混杂串 判断其是否可以推出空符 @ 比如  @MN  M和N都能推出空串 那么 这个就可以推出空串
    bool judgeAStrInStringVector(const std::string& str,std::vector<std::string>* vector);  //判断一个字符串是否包含在一个stringVector中
    bool judgeNoneTerminator2None(std::string noneTerminatorStr);   // 判断一个非终结符 看他是否能够推出空字符串
    void recursionCalNoneTerminatorFirst(std::set<std::string> *firstSet, // 该非终结符的first集合 如果没有会new一个
                                         std::string currentNoneTerminatorStr, //  当前非终结符
                                         std::vector<std::vector<std::string>> *NoneTerminatorRight); //  当前非中介符对应的右端
    void parseAndPrintStack(); // 根据用户输入的串 输出栈的变化过程
    bool parseLine(std::string currentStackStr, std::string leftStackStr); // 分析并打印每一行的   currentStack栈和leftStack栈变化情况


    void getInputFromFile(const std::string& path); // 从文件中读取数据 然后进行相应数据的存储 方便后面使用
    void calFirstMap();     // 计算first集
    void calFollowMap();    // 计算follow集
    void calSelectMap();    // 计算select集
    /**
    current stack      left stack      output
    E$                 ((i+i*(i+i)))$
     */
    void initStack();       // 初始化两个栈 currentStack 和  leftStack 例子
    void printDetail();    // 打印详细信息 比如 终结符有什么 非终结符有什么 产生式有哪些 first集 follow集 select集是什么




    std::string startNonTerminator;  //记录初始非终结符    默认第一个产生式的左侧是起始终结符
    std::vector<std::string> *terminatorVector; //终结符的集合  不重复
    std::vector<std::string> *noneTerminatorVector; //非终结符的集合  不重复
    std::vector<productionForm*> *productionFormVector; // 产生式 vector
    std::map<std::string,productionForm> * expressionMap; // key是产生式左侧 value是产生式类的实例
    std::map<std::string,std::set<std::string>*> *firstMap;  // 所有非终结符的first集
    std::map<std::string,std::set<std::string>*> *followMap;  // 所有非终结符的follow集
    std::map<std::string,std::vector<std::string>*> *selectMap;  // 所有非终结符的follow集
    std::map<std::string,std::map<std::string,std::string>*> forecastAnalysisTable;
    std::vector<std::string> *mergeTerAndNoneVector;  // 合并非终结符与终结符的vector
    std::string  tobeResolvedInput;  // 记录了要解析的串 比如((i+i*(i+i)))$
    std::list<std::string> *currentStack;  //使用双向链表来模拟栈 方便打印 这个是左侧的包含产生式的栈变化 初始话后是 开始符号$
    std::list<std::string> *leftStack;   //使用双向链表来模拟栈 方便打印  这个栈是用来记录输入串被解析的变换 比如  i*i+i
public:

    explicit LL1(std::string path);
    ~LL1();
};


#endif //C___ABSTRACT_SYNTAX__CODE_C_LL1_H
