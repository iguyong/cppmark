//
// Created by yong gu on 5/6/18.
//

#ifndef CPPMARK_DOC_H
#define CPPMARK_DOC_H

#include <string>
#include <vector>
#include "token.h"
class Doc{
public:
    Doc(std::string src):src(src){}
    std::string src;
    std::vector<Token> tokens;
};
#endif //CPPMARK_DOC_H
