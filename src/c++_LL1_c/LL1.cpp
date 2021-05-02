//
// Created by yhn on 3021/4/26.
//

#include <algorithm>
#include "LL1.h"

std::string LL1::removeSpace(std::string str) {
    for(auto iter = str.begin();iter != str.end();iter ++){
        if(isblank(*iter) != 0){  	 //isblank判断是否为制表符或者空格，是的话返回非零值
            str.erase(iter--); // --是因为重新判断一下填过来的字符
        }
    }
    return str;
}

std::vector<std::string> *LL1::split(const std::string &str, const std::string &delim) {
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

std::vector<std::string> *LL1::splitByTerminatorAndNoneTer(std::string str) {
    // 核心思想是一个个匹配 直到最后point指针指到字符串末尾
    // 算法最坏情况下是 o(n^2)  最好是o(n)
    std::vector<std::string> *res = new  std::vector<std::string>();
    int point = 0;
    int strLength = str.size();
    while (point <= strLength-1){
        // 这个 mergeTerAndNoneVector 不仅包含非终结符 和终结符 还包含 空符@   和终止符$
        for(std::string token: *mergeTerAndNoneVector){
            int tokenLength = token.size();
            if (point  + tokenLength <= strLength && str.substr(point,tokenLength) == token){
                //如果 数组没越界 而且 匹配上了  注意这里是贪婪匹配  也就是 碰到 E' 和 E 优先匹配E'
                // 保证贪婪匹配的关键在于排序  把多字符排在前面 在getInputFromFile 函数中已经排序了
                point += tokenLength;  // 指针后移
                res->push_back(token); // 将匹配到token 放入到结果vector中
                break;
            }
        }
    }
    return res;
}

void LL1::getInputFromFile(const std::string& path) {
    std::ifstream myfile(path);
    std::string temp;
    if (!myfile.is_open())
    {
        cout << "未成功打开文件" << endl;
    }
    int i = 0;
    bool flag = true;
    while(getline(myfile,temp))
    {
        if (i==0){
            terminatorVector = split(temp," ");
            terminatorVector->push_back("@");  // 放入空符 强制规定@是空符
        } else if(i==1){
            noneTerminatorVector = split(temp," ");
        } else if (i >=2) {
            if (i ==2){
                // 得到合并后的包含终结符和非终结符的集合
                mergeTerAndNoneVector->insert(mergeTerAndNoneVector->end(),terminatorVector->begin(),terminatorVector->end());
                mergeTerAndNoneVector->insert(mergeTerAndNoneVector->end(),noneTerminatorVector->begin(),noneTerminatorVector->end());
                //这里需要进行添加 终结符$ 因为后期匹配输入串的时候要用到  如果不添加会死循环
                mergeTerAndNoneVector->push_back("$");
                // 排序 倒序排序  字符数多的在前边 因为 贪婪匹配
                std::sort(mergeTerAndNoneVector->rbegin(),mergeTerAndNoneVector->rend());
//                cout<<mergeTerAndNoneVector->size()<<endl;
            }

            if(expressionMap->size() ==1){
                // 如果产生式map中只存在一个 也就是刚将第一个产生式读进 那么 第一个产生的左部就是起始非终结符
                startNonTerminator = expressionMap->begin()->first;
            }
            std::string afterTrimStr = removeSpace(temp);
            if(!flag){
                // flag用于控制 读入end 表明产生式输入结束后 下一个输入的行里就包括了要解析的字符串
                tobeResolvedInput = afterTrimStr;
                break;
            }
            if (afterTrimStr != "end") {
                // right 是个二维vector  因为一个非终结符 的右侧可能有多个 比如 E => aA|bB|@  所以  第一层分隔了 aA 和bB 和 @  第二层分割了 a   A
                std::vector<std::vector<std::string>> *right = new std::vector<std::vector<std::string>>();
                std::vector<std::string> *splitByArrowVector = split(afterTrimStr, "=>");
                std::vector<std::string> *splitByOrVector = split((*splitByArrowVector)[1], "|");
                for(std::string ele : *splitByOrVector){
                    right->push_back(*splitByTerminatorAndNoneTer(ele));
                }

                // 构造产生式类  塞入productionFormVector和expressionMap  后边方便使用
                auto  aa = new productionForm(afterTrimStr, (*splitByArrowVector)[0],right );
                productionFormVector->push_back(aa);
                expressionMap->insert(std::pair<std::string,productionForm>((*splitByArrowVector)[0],*aa));
            }
            else if(afterTrimStr == "end"){
                flag = false;
            }
        }
        i++;
    }
    myfile.close();
}
//  注意 此程序的计算first集的结果可能不正确
//因为  没考虑 S-> ABCDEFG  first(S) 的值不仅依赖与A 如果A->@ 那么 B的first集也应该放入到first(S)
// 以此类推 如果B->@ 那么 C的first集也应该放入到first(S)中
// 最后如果ABCDEFG都可以推出@ 那么 还需将 @ 放入到first(S)中
void LL1::calFirstMap() {
    firstMap =  new std::map<std::string, std::set<std::string>*>();
    // 遍历所有的非终结符 求出其first集合
    for (std::string currentNoneTerminatorStr :*noneTerminatorVector){
        bool shouldBreak = false;
        std::set<std::string>  *currentNoneTerminatorFirstSet = new  std::set<std::string>();
        auto iter = firstMap->find(currentNoneTerminatorStr);
        // 每次都提前 new 一个 firstSet  防止后边没有建立 产生空指针
        if ( iter == firstMap->end()){
            firstMap->insert(std::pair<std::string, std::set<std::string>*>(currentNoneTerminatorStr, currentNoneTerminatorFirstSet));
        }
        // 拿到 非终结符产生式右侧的数据
        std::vector<std::vector<std::string>> *right = expressionMap->find(currentNoneTerminatorStr)->second.getRight();
        //  递归调用  这里的currentNoneTerminatorFirstSet 调用后会被塞入要求的first集
        recursionCalNoneTerminatorFirst(currentNoneTerminatorFirstSet,currentNoneTerminatorStr,right);
    }
}

void LL1::calFollowMap() {
    followMap =  new std::map<std::string, std::set<std::string>*>();
    // follow(起始终结符) = {"$"}
    std::set<std::string> *startProductionFormSet = new std::set<std::string>();
    startProductionFormSet->insert("$");
    // 拿到第一个产生式获取其左部 强制规定 第一个产生式的左部是起始非终结符
    followMap->insert(std::pair<std::string,std::set<std::string>*>(startNonTerminator,startProductionFormSet));


    std::map<std::string,productionForm>::iterator iter ;
    for(iter=expressionMap->begin();iter!=expressionMap->end();iter++){
        // 获取当前产生式和当前产生式的右侧
        std::string currentNonTerminatorStr = iter->first;
        std::vector<std::vector<std::string>>* right  = iter->second.getRight();
        auto *currentFollowSet = new  std::set<std::string>();
        followMap->insert(std::pair<std::string,std::set<std::string>*>(currentNonTerminatorStr,currentFollowSet));

        // 第一次循环 将所有形如 S->Ab S->AB 这类的follow集找出来  依托的是first集 只需要执行一遍
        for(auto ele :*right){
            // E'->+TE'|@
            if (ele.size() >= 2){
                // 比如 ele是 + T E' 长度大于2 才有可能出现两个非终结符连续的情况
                int i = 0;
                int vectorLength = ele.size();
                std::vector<std::string>::iterator  iterForStringVector ;
                for(iterForStringVector=ele.begin();iterForStringVector!=ele.end();iterForStringVector++,i++){
                    if (!judgeAStrInStringVector(*iterForStringVector,terminatorVector)) {
                        auto nn = followMap->find(*iterForStringVector);
                        std::set<std::string> *followSet;
                        if (nn == followMap->end()) {
                            followSet = new std::set<std::string>();
                            followMap->insert(std::pair<std::string, std::set<std::string> *>(*iterForStringVector,
                                                                                              followSet));
                        } else {
                            followSet = nn->second;
                        }
//                        cout<<"---------\nleft"<<currentNonTerminatorStr<<endl;
                        std::vector<std::string> tempVector(iterForStringVector+1,ele.end());
//                        for (auto ooooooo: tempVector){
//                            cout<<ooooooo<<endl;
//                        }
//                        cout<<"+++++++++\n";
                        // 如果当前字符是非终结符 需要判断下一个是否是非终结符  但是还要防止越界
                        // 1. 寻找 形如 Ab  AB 这种格式的字符串
                        if (i < vectorLength - 1 &&!judgeAStrInStringVector(*(iterForStringVector + 1), terminatorVector)) {

                            // 如果说当前字符是非终结符 下一个字符也是非终结符 形如AB  那么我们就将下一个字符(B)的first集加入到A的follow集中
                            for (auto mm:*firstMap->find(*(iterForStringVector + 1))->second) {
                                if (mm != "@") {
                                    // 除了空都可以加入
                                    followSet->insert(mm);
                                }
                            }

                        }
                        else if (i < vectorLength - 1 && judgeAStrInStringVector(*(iterForStringVector + 1), terminatorVector)) {
                            //  非终结符后边跟随的是终结符  那将终结符 加入到非终结符的follow集中 比如 S->Ab follow(S) 添加b
                            if (*(iterForStringVector + 1) != "@") {
                                followSet->insert(*(iterForStringVector + 1));
                            }
                        }

                    }
                }
            }
        }
        // 找到 形如A-MBN  N为空 将A的follow集加入到B中  至少执行一遍
        for(auto ele :*right){
            int i = 0;
            int vectorLength = ele.size();
            std::vector<std::string>::iterator  iterForStringVector ;
            for(iterForStringVector=ele.begin();iterForStringVector!=ele.end();iterForStringVector++,i++){

                if (!judgeAStrInStringVector(*iterForStringVector,terminatorVector)){
                    // 说明当前字符是非终结符 此时对应了A->MBN 中的 B  那么需要判断N是否是空
                    std::vector<std::string> tempVector(iterForStringVector+1,ele.end());
//                    cout<<"aaaaaaaaaaaaaaaaaaaaaa"<<endl;
//                    cout<<"iterForStringVector"<<*iterForStringVector<<endl;
//                        cout<<currentNonTerminatorStr<<endl;

//                    for(auto item:tempVector){
//                        cout<<item<<endl;
//                    }
//                    cout<<judgeNoneOrTerminatorList2None(tempVector)<<endl;
//                    cout<<"bbbbbbbbbbbbbbbbbbbbbb"<<endl;
                    if(judgeNoneOrTerminatorList2None(tempVector)){
                        // 说明N是空 那么 需要找到B的set和A的set
                        //1. 找到B的set
                        auto BFollow = followMap->find(*iterForStringVector);
                        //2. 找到A的set
                        auto AFollow = followMap->find(currentNonTerminatorStr);
                        for (auto followItem :*AFollow->second ){
                            BFollow->second->insert(followItem);
//                                flag = true;
                        }
                    }
                }

            }
        }
    }


    std::vector<int> *sizeVector = new std::vector<int>();
    for(const auto& tempFollowItem:*followMap){
        sizeVector->push_back(tempFollowItem.second->size()) ;
    }
    bool flag = true;
    // 执行多遍    补全follow集  如果sizeVector的各个数（记录着存储每个非终结符的set的大小）前后两次不一致 那么不结束
    while (flag){
        flag = false;
        for(iter=expressionMap->begin();iter!=expressionMap->end();iter++) {
            std::string currentNonTerminatorStr = iter->first;
            std::vector<std::vector<std::string>>* right  = iter->second.getRight();
            for (auto ele :*right) {
                int i = 0;
                int vectorLength = ele.size();
                std::vector<std::string>::iterator iterForStringVector;
                for (iterForStringVector = ele.begin(); iterForStringVector != ele.end(); iterForStringVector++, i++) {

                    if (!judgeAStrInStringVector(*iterForStringVector, terminatorVector)) {
                        // 说明当前字符是非终结符 此时对应了A->MBN 中的 B  那么需要判断N是否是空
                        std::vector<std::string> tempVector(iterForStringVector + 1, ele.end());
                        if (judgeNoneOrTerminatorList2None(tempVector)) {
                            // 说明N是空 那么 需要找到B的set和A的set
                            //1. 找到B的set
                            auto BFollow = followMap->find(*iterForStringVector);
                            //2. 找到A的set
                            auto AFollow = followMap->find(currentNonTerminatorStr);

                            for (const auto &followItem :*AFollow->second) {
                                BFollow->second->insert(followItem);
                            }
                        }
                    }

                }
            }
        }
        int i = 0;
        for(const auto& tempFollowItem:*followMap){
            if(*(sizeVector->begin()+i) != tempFollowItem.second->size()){
                flag = true;
                sizeVector->clear();
                for(const auto& tempFollowItem:*followMap){
                    sizeVector->push_back(tempFollowItem.second->size()) ;
                }
                break;
            }
            i++;
        }
    }
}

void LL1::calSelectMap() {
    selectMap = new std::map<std::string,std::vector<std::string>*>();
    int i = 0;
    for(auto expressionItem:*expressionMap){
        auto right = expressionItem.second.getRight();
        std::string single;
        for(auto rightItem:*right){
            auto *tempInsideVector = new std::vector<std::string>;
            // 分三种情况
            //第一种情况  A -> aB  a  第一个字符是一个终结符 那么直接将终结符加入到select集中区
            if (!judgeAStrInStringVector(rightItem[0],noneTerminatorVector) && rightItem[0] != "@"){
                single = expressionItem.first+"=>";

                for(auto bbbbb:rightItem){
                    single += bbbbb;
                }

                tempInsideVector->push_back(rightItem[0]);
            }
            //第二种情况  B -> CD  C的first集  第一个字符是一个非终结符 那么将这个非终结符的first集加入到select集中
            else if(judgeAStrInStringVector(rightItem[0],noneTerminatorVector)){
                single = expressionItem.first+"=>";
                for(auto bbbbb:rightItem){
                    single += bbbbb;
                }
                auto firstSet = firstMap->find(rightItem[0])->second;
                for(auto firstItem:*firstSet){
                    tempInsideVector->push_back(firstItem);
                }

            }
            //第三种情况  C -> @  C的follow集
            else if (rightItem[0] == "@"){
                single = expressionItem.first+"=>";
                for(auto bbbbb:rightItem){
                    single += bbbbb;
                }
                auto followSet = followMap->find(expressionItem.first)->second;
                for(auto followItem:*followSet){
                    tempInsideVector->push_back(followItem);
                }
            }
            selectMap->insert(std::pair<std::string,std::vector<std::string>*>(single,tempInsideVector));
            i++;
        }
    }

}

void LL1::printDetail() {
    std::cout<<"所有的终结符有：";
    for(const std::string& terminator : *terminatorVector){
        cout<<terminator<<" ";
    }
    cout<<endl;

    cout<<"所有的非终结符有：";
    for(const std::string& noneTerminator : *noneTerminatorVector){
        cout<<noneTerminator<<" ";
    }
    cout<<endl;

    cout<<"总共有 "<<productionFormVector->size()<<" 个产生式"<<endl;
    // 打印输出 处理前的内容
    int m =0;
    for(productionForm *ele : *productionFormVector){
        cout<<m+1<<".";
        m++;
        ele->print();
        cout<<endl;
    }

    cout<<"所有非终结符的first集是："<<endl;
    std::map<std::string,std::set<std::string>*>::iterator iterForfirstMap;
    for(iterForfirstMap=firstMap->begin(); iterForfirstMap!=firstMap->end(); iterForfirstMap++){
        cout<<"first("<<iterForfirstMap->first<<")=(";
        std::set<std::string>::iterator abc;
        for(abc = iterForfirstMap->second->begin(); abc!=iterForfirstMap->second->end();abc++ ){
            if (abc == iterForfirstMap->second->begin()){
                cout<<'"'<<*abc<<'"';
            }
            else{
                cout<<" ,"<<'"'<<*abc<<'"';
            }
        }
        cout<<")"<<endl;
    }


    cout<<"所有非终结符的follow集是："<<endl;
    std::map<std::string,std::set<std::string>*>::iterator iterForFollowMap;
    for(iterForFollowMap=followMap->begin(); iterForFollowMap!=followMap->end(); iterForFollowMap++){
        cout<<"follow("<<iterForFollowMap->first<<")=(";
        std::set<std::string>::iterator abc;
        for(abc = iterForFollowMap->second->begin(); abc!=iterForFollowMap->second->end();abc++ ){
            if (abc == iterForFollowMap->second->begin()){
                cout<<'"'<<*abc<<'"';
            }
            else{
                cout<<" ,"<<'"'<<*abc<<'"';
            }
        }
        cout<<")"<<endl;
    }


    cout<<"所有的select集有："<<endl;
    for(auto selectMapItem:*selectMap){
        cout<<selectMapItem.first<<"     ";
        for(auto ele :*selectMapItem.second){
            cout<<ele<<" ";
        }
        cout<<endl;
    }

    cout<<"需要解析的串是:"<<tobeResolvedInput<<endl;



}

LL1::LL1(std::string filePath) {
    terminatorVector = new std::vector<std::string>();
    noneTerminatorVector = new std::vector<std::string>();
    productionFormVector = new std::vector<productionForm*>();
    expressionMap = new std::map<std::string,productionForm>();
    mergeTerAndNoneVector = new  std::vector<std::string>();

    getInputFromFile(filePath);

    // 求first集
    calFirstMap();
    // 求follow集
    calFollowMap();
    // 求select集
    calSelectMap();
    initStack();
    printDetail();
    parseAndPrintStack();

}

LL1::~LL1() {
    // 释放空间
    delete terminatorVector;
    delete noneTerminatorVector;

    for(auto ele:*productionFormVector){
        delete ele;
    }
    delete productionFormVector;


    delete firstMap;
    delete followMap;
    delete expressionMap;
    delete mergeTerAndNoneVector;
}


// 介绍一下这个函数的思想
/**
A->B
B->C
C->a
如果想求A的first集 需要求B的  B还依赖于C C的first集 包含 a 结束递归 那么 B的first集包含a A的也包含
 */
void LL1::recursionCalNoneTerminatorFirst(std::set<std::string> *firstSet,
                                          std::string currentNoneTerminatorStr,
                                          std::vector<std::vector<std::string>> *NoneTerminatorRight) {

    // 例子 A->+TA|@
    // firstSet  当前非终结符A的 的first集 不可重复
    // currentNoneTerminatorStr     产生式左部：A     ,
    // NoneTerminatorRight 第一个vector + T A  第二个vector  @

    for(std::vector<std::string> itemVector: *NoneTerminatorRight){

        //  一共两个循环
        //  第一个循环时 itemVector 是 + T A
        //  第二个循环时 itemVector 是 @
        // 如果第一个字符 是非终结符 那么 加入
        if(std::count(terminatorVector->begin(),terminatorVector->end(),itemVector[0]) > 0 ){
            // 此时 + 和 @ 被加入到了 firstSet中
            firstSet->insert(itemVector[0] );
        } else if (std::count(noneTerminatorVector->begin(),noneTerminatorVector->end(),itemVector[0]) > 0){
            // 如果第一个字符是非终结符 那么 需要进行递归调用
            // 未解决的小问题 见 calFirstMap函数上方
//            for (auto ele:itemVector){
//                if (judgeNoneTerminator2None(itemVector[0])){
//                    // 说明 当前产生式右侧第一个字符（非终结符）可以为空 那么需要查看第二个是否为空 以此类推
//
//                }
//            }

            std::set<std::string> *tempSet = new std::set<std::string>();
            std::vector<std::vector<std::string>> *right = expressionMap->find(itemVector[0])->second.getRight();
            recursionCalNoneTerminatorFirst(tempSet,
                                            itemVector[0],
                                            right);
            // 下边这个for循环就是将上一个递归的结果 添加到这次的first集中 否则就丢失数据了
            for (auto ele :  *tempSet ){
                firstSet->insert(ele);
            }
            free(tempSet);
        }
    }

}


/*
 * 传入一个非终结符和终结符混杂串 判断其是否可以推出空符 @
 * 比如  @MN  M和N都能推出空串 那么 这个就可以推出空串
 * 如果可以的话 那么 返回true 主要用于求first集中使用
 */
bool  LL1::judgeNoneOrTerminatorList2None(std::vector<std::string> strVector) {

    if(strVector.size() == 1 && judgeAStrInStringVector(*strVector.begin(),noneTerminatorVector) )
        return true;

    for (auto ele:strVector){
        auto count = std::count(noneTerminatorVector->begin(),noneTerminatorVector->end(),ele);
        // 说明 这个符号不在非终结符中
        if (count == 0 ){
            // 且不是 @ 那么说明这一长串不可能推出空 那么 直接进行返回false
            if (ele!="@")
                return false;
        }else{
            // 如果说不能判断此非终结符能够推出@空集   那么就直接false
            if (!judgeNoneTerminator2None(ele) ){
                return false;
            }
        }
    }
    return true;
}

// 还是用的find函数  从两行变成一行了
bool LL1::judgeAStrInStringVector(const std::string& str,std::vector<std::string>* vector) {
    auto iter = std::find(vector->begin(),vector->end(),str);
    return iter != vector->end();
}

/*
 * 传入一个非终结符 判断其是否可以推出空符 @  如果可以的话 那么 返回true 主要用于求first集中使用
 */
bool  LL1::judgeNoneTerminator2None(std::string noneTerminatorStr) {
    std::string allStr =  expressionMap->find(noneTerminatorStr)->second.getAll();

    if ( allStr.find('@') ==  std::string::npos){
        return false;
    }
    return true;
}

/**
 * 初始化两个栈  一个是当前栈 一个是剩余输入栈
当前栈      剩余输入栈      输出
E$         id+id*id$
TE'$       id+id*id$    E->TE'

 */
void LL1::initStack() {
    // 初始化 lefStack  填入   例子 id+id*id$
    leftStack = new std::list<std::string>();
    auto tempList =  splitByTerminatorAndNoneTer(tobeResolvedInput);

    for(auto ele : *tempList){
        leftStack->push_back(ele);
    }
    // 初始化 currentStack 填入 初始符号 和 结束符$
    currentStack = new std::list<std::string>();
    currentStack->push_back(startNonTerminator);
    currentStack->push_back("$");
}

void LL1::parseAndPrintStack() {
    std::printf("%-30s %-30s %-30s\n","current stack","left stack","output");
    std::string currentStackStr = "";
    std::string leftStackStr = "";
    for(auto ele:*currentStack){
        currentStackStr += ele;
    }
    for (auto ele:*leftStack){
        leftStackStr += ele;
    }
    std::printf("%-30s %-30s\n",currentStackStr.c_str(),leftStackStr.c_str());
    // 如果解析失败或者是解析完成 会返回false 循环会继续执行
    bool flag = true;
    while(flag){
       flag = parseLine(*currentStack->begin(), *leftStack->begin());
    }
    // 两个栈中开头都是$ 说明解析成功了
    if(*currentStack->begin()== "$" && *leftStack->begin() == "$")
        cout<<"解析成功"<<endl;
    else{
        cout<<"解析失败,此串不是该文法的语言"<<endl;
    }
}

/**
E$         id+id*id$
TE'$       id+id*id$    E->TE'

 */
bool LL1::parseLine(std::string currentStackStr, std::string leftStackStr) {
    if(leftStackStr == "$" && currentStackStr == "$")
        return false;
    // 如果出现这种情况   idT'E'$             id+id*id$           F->id
    // 都出栈 然后返回true 还需要进行下一步循环
    if(currentStackStr == leftStackStr){
        currentStack->erase(currentStack->begin());
        leftStack->erase(leftStack->begin());
        return true;
    }

    std::string test = "";
    for(auto selectMapItem:*selectMap){
        std::string productionFormStr = selectMapItem.first;
        auto splitVector = split(selectMapItem.first,"=>");
        for(auto str:*selectMapItem.second){
            if(str == leftStackStr && (*splitVector)[0] ==currentStackStr ){
                // 例子 currentStackStr 是E    leftStackStr是 id
                // str 是id    leftStackStr 是id
                // 应该将E替换为 TE'
                currentStack->erase(currentStack->begin()); //  清除栈顶元素 其实是list的头元素
                auto splitRightVector = splitByTerminatorAndNoneTer((*splitVector)[1]); // TE'
                std::vector<std::string>::reverse_iterator re_iter ;
                for(re_iter = splitRightVector->rbegin();re_iter!=splitRightVector->rend();re_iter++){
                    if (*re_iter != "@") //这个for循环是逆序的 注意  将栈顶元素出栈后 之后将替代元素压入栈中  比如 E->TE'  将E出栈 TE'进栈
                        currentStack->push_front(*re_iter);
                }
                test = selectMapItem.first;


                //打印输出 当前两个栈的信息  和 当前执行的产生式
                std::string temp1 = "";
                std::string temp2 = "";
                for(auto ele:*currentStack){
                    temp1 += ele;
                }
                for (auto ele:*leftStack){
                    temp2 += ele;
                }
                std::printf("%-30s%-30s%-30s\n",temp1.c_str(),temp2.c_str(),test.c_str());
                return true;
            }
        }
    }
    // 如果没有在前边返回true 说明解析出错 停止解析
    return false;
}






