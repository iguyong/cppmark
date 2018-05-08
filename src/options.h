//
// Created by yong gu on 5/8/18.
//

#ifndef CPPMARK_OPTION_H
#define CPPMARK_OPTION_H

#include <string>

class Options{
public:
    bool codeTags = true;
    bool lineNumber = false;
    std::string langPrefix;
    std::string headerPrefix;
};
#endif //CPPMARK_OPTION_H
