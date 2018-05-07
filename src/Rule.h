//
// Created by yong gu on 5/3/18.
//

#ifndef CPPMARK_RULE_H
#define CPPMARK_RULE_H

#include <string>
#include <vector>

class Rule {
    std::vector<std::string> alts;

    virtual bool apply();
};

class Heading : public Rule {
    static std::string name = "heading";

};


#endif //CPPMARK_RULE_H
