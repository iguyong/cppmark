//
// Created by yong gu on 5/3/18.
//
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../src/CPPMark.h"

using std::string;



int main(int argc,char** argv){
    std::ifstream ifile("../test/main.md");
    if(!ifile){
        std::cerr<<"open file failed !"<<std::endl;
        return -1;
    }
    std::stringstream mdSS;
    mdSS << ifile.rdbuf();
    string mdStr = mdSS.str();

    CPPMark md(mdStr);
    md.parse();
    ifile.close();
}