#include <bits/stdc++.h>

#include "Parser.h"

void read(FILE *fp,char *charArray) {
    char c;
    int i = 0;
    while(!feof(fp)){
        char c = fgetc(fp);
        charArray[i++] = c;
    }
}

int main(int argc, char *argv[], char *env[]) {
    argc--;
    argv++;
    if(argc == 0){
        printf("致命错误：无输入文件路径\n");
        return 1;
    }else{
        char fileIn[10000];
        memset(fileIn, 0, sizeof(fileIn));
        while(argc > 0){
            FILE *fp  = fopen(argv[0], "r");
            read(fp, fileIn);
            printf("\n************\n待输入的字符串为:\n%s\n************\n", fileIn);
            Parser parser(fileIn);
            while (true) {
                int status = parser.parseNext();
                if (status == -1) {
                    printf("parse success!");
                    break;
                }
                if (status == 0) {
                    printf("parse error!");
                    break;
                }
                parser.showStatus();
            }
            argv++;
            argc--;
        }
    }

}