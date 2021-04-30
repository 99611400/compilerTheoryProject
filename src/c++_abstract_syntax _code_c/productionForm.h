//
// Created by yhn on 2021/4/26.
//

#ifndef C___ABSTRACT_SYNTAX__CODE_C_PRODUCTIONFORM_H
#define C___ABSTRACT_SYNTAX__CODE_C_PRODUCTIONFORM_H
#include <string>
#include <set>
#include <map>

class productionForm {
public:
    productionForm(std::string all, std::string left, std::vector<std::vector<std::string>>* right);
    void print();
    std::string getAll();
    std::vector<std::vector<std::string>> * getRight();

    ~productionForm();

private:
    std::string all; // 全部
    std::string left; // 左部 上下文无关语言的产生式的左侧只可能有有一个字符串
    std::vector<std::vector<std::string>> *right; // 产生式的右侧  可能用| 分割 表示或 所以需要判断是否是多个
    std::map<std::string,std::vector<std::vector<std::string>*>*> *selectMap;
};


#endif //C___ABSTRACT_SYNTAX__CODE_C_PRODUCTIONFORM_H
