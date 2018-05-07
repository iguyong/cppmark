//
// Created by yong gu on 5/3/18.
//

#include "state_block.h"

StateBlock::StateBlock(std::string src) : src(src) {
    lineNum = generateMarks();
}


int StateBlock::generateMarks() {
    int lineNo = 0;
    int len = src.size();

    int begin = 0;
    int end = 0;
    int offset = 0;
    int indent = 0;

    char ch = 0;
    bool indent_found = false;
    for (int pos = 0; pos < len; pos++) {
        ch = src[pos];
        if (!indent_found) {
            if (ch == 0x09 || ch == 0x20) {
                indent++;
                if (ch == 0x09) {      //TAB expansion
                    offset += 4 - offset % 4;
                } else {
                    offset++;
                }
                continue;
            } else {
                indent_found = true;
            }
        }

        if (ch == 0x0A || pos == len - 1) {
            if (ch != 0x0A) { pos++; }   // for the last line, set "end" to pos+1
            end = pos;
            lineMarks.emplace_back(begin, end, offset, indent);  //finish one line
            lineNo++;
            begin = pos + 1;
            indent_found = false;
            indent = 0;
            offset = 0;
        }

    }
    return lineNo;
}


