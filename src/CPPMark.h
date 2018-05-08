//
// Created by yong gu on 5/3/18.
//

#ifndef CPPMARK_CPPMARK_H
#define CPPMARK_CPPMARK_H

#include "token.h"
#include <string>

using std::string;

class CPPMark {
public:
    explicit CPPMark(string mdStr);

    string src;
    State state;

    void parse();

private:
    void normalize();
//    void block();
//    void inline();
};

#endif //CPPMARK_CPPMARK_H
