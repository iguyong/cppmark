//
// Created by yong gu on 5/3/18.
// class Ruler is used to maintain rules of top-core, block and inline.
//

#include <vector>
#include <string>
#include <unordered_map>

using std::vector;
using std::string;
using std::unordered_map;

class Rule {
    string name;

    void apply();
};

class Ruler {
    vector<Rule> rules;
    unordered_map<string, vector<Rule>> __cache__;
};