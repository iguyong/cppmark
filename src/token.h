//
// Created by yong gu on 5/3/18.
//

#ifndef CPPMARK_TOKEN_H
#define CPPMARK_TOKEN_H

#include <string>
#include <utility>
#include <vector>

using std::string;

class Token {
public:
    string type;
    int level;
    string language;
    string text;
    bool ordered = false;
    int checked = -1; // -1: not checkbox, 0: not checked, 1: checked
    bool pre;
    bool escaped;

    void print() const;
};

#endif //CPPMARK_TOKEN_H
