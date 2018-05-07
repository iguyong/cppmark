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
    string tag;
    std::vector<std::pair<string, string>> attrs;
/**
 * Token#nesting -> Number
 *
 * Level change (number in {-1, 0, 1} set), where:
 *
 * -  `1` means the tag is opening
 * -  `0` means the tag is self-closing
 * - `-1` means the tag is closing
 **/
    int nesting;
    int level;
    string content;
    string markup;
    string info;
    bool isBlock;
    bool hiden;
};

#endif //CPPMARK_TOKEN_H
