//
// Created by yhn on 2021/4/26.
//

#include <vector>
#include <iostream>
#include "productionForm.h"

using std::cin;
using std::cout;
using std::endl;


productionForm::productionForm(std::string all,
                               std::string left,
                               std::vector<std::vector<std::string>> *right)
                               : all(std::move(all)), left(std::move(left)),right(right) {
}



void productionForm::print() {
    cout<<"产生式："<<all<<endl;
    cout<<"产生式左部："<<left<<endl;
    cout<<"产生式右部：";

    std::vector<std::vector<std::string>>::iterator  iter;
    for (iter=right->begin();iter!=right->end();iter++){
        std::vector<std::string>::iterator iterator;
        for (iterator=(*iter).begin();iterator!=(*iter).end();iterator++ ){
            if (iterator==(*iter).end()-1)
                cout<<*iterator;
            else
                cout<<*iterator<<" ";
        }
        if (iter!=right->end()-1)
            cout<<"|";
    }
    cout<<endl;
}

std::string productionForm::getAll() {
    return all;
}

std::vector<std::vector<std::string>>* productionForm::getRight() {
    return right;
}

productionForm::~productionForm() {

}








