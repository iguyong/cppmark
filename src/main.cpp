//
// Created by yong gu on 5/3/18.
//
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "parser.h"
using namespace std;


int main(int argc, char **argv) {

    std::ifstream inFile("../test/encoding.md");
    ofstream outFile("../test/index.html");
    ifstream tmpFile("../test/tmp.html");
    if(!inFile || !tmpFile || !outFile){
        std::cerr<<"open file failed !"<<std::endl;
        return -1;
    }
    std::stringstream mdSS;
    mdSS << inFile.rdbuf();
    string mdStr = mdSS.str();
    string tmp;
    stringstream tmpSS;
    tmpSS << tmpFile.rdbuf();
    tmp = tmpSS.str();
    mdStr = "a\u00a0b";

    Parser parser;
    parser.tokenBlock(mdStr);

    for(const auto& t: parser.tokens){
        t.print();
    }
    for(const auto& l: parser.links){
        cout<<"link["<<l.first<<"]: "<<"["<<l.second.title<<"]("<<l.second.href<<")"<<endl;
    }

    parser.parse();


    string outHtml = regex_replace(tmp,regex("\\{\\{ *body *\\}\\}"),parser.out);
    outFile << outHtml;

    outFile.close();
    inFile.close();
    tmpFile.close();
    cout<<"finished."<<endl;
    return 0;
}