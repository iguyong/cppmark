//
// Created by yong gu on 5/3/18.
//
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "parser.h"
#include "doc.h"
using namespace std;


int main(int argc, char **argv) {

    std::ifstream ifile("../test/main.md");
    if(!ifile){
        std::cerr<<"open file failed !"<<std::endl;
        return -1;
    }
    std::stringstream mdSS;
    mdSS << ifile.rdbuf();
    string mdStr = mdSS.str();

    Doc doc(mdStr);
    Parser parser("gfm");
    parser.token(doc);

    for(auto t:doc.tokens){
        cout<<t.type<<endl;
    }

    return 0;
}