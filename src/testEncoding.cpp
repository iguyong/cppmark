//
// Created by yong gu on 5/9/18.
//


#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <regex>
using namespace std;

int main(int argc, char** argv){
    ifstream inFile("../test/article.md");
    ofstream outFile("../test/encoding.md");
    if(!inFile || !outFile){
        cout<<"open file failed!";
        return -1;
    }
    outFile<<"a\u00a0b";
//    stringstream mdSS;
//    mdSS<<inFile.rdbuf();
//    string mdStr = mdSS.str();
//
//    mdStr = " \xe6\xa0\x87";
//    cout<<mdStr<<endl;
//    regex_replace(mdStr,regex("\u00a0")," ");
//    cout<<mdStr<<endl;
//    for(int i= 0; i < mdStr.length();i++){
//        cout<<i<<": "<<std::hex<<(int)mdStr[i]<<endl;
//    }
//    cout<<endl;
}
