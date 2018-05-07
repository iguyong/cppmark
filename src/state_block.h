//
// Created by yong gu on 5/3/18.
//

#ifndef CPPMARK_STATE_BLOCK_H
#define CPPMARK_STATE_BLOCK_H

#include <vector>
#include <string>
#include <iostream>

class LineMark {
public:
    LineMark(int begin = 0, int end = 0, int indent = 0, int offset = 0) : begin(begin), end(end), indent(indent),
                                                                           offset(offset) {}

    int begin;     //position of the beginning of each line (in the whole source string)
    int end;       //position of the end of each line
    int offset;    //offset(tab expanded) of each line
    int indent;    //indent(tab not expanded) of each line

    void print(std::ostream &out) {
        out << begin << " : " << end << " : " << indent << " : " << offset << std::endl;
    }
};

class StateBlock {
public:
    StateBlock(std::string src);

    std::string src;
    int lineNum;
    std::vector<LineMark> lineMarks;
private:
    int generateMarks();
};

#endif //CPPMARK_STATE_BLOCK_H
