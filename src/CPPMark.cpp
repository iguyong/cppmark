//
// Created by yong gu on 5/3/18.
//
#include "CPPMark.h"

#include <regex>
#include <fstream>
#include <sstream>

using std::regex;

CPPMark::CPPMark(string mdStr) : src(mdStr) {
}

void CPPMark::parse() {
    normalize();
}

void CPPMark::normalize() {
    regex NEWLINES_RE("\r[\n\u0085]?|[\u2424\u2028\u0085]");
    regex NULL_RE("\u0000");
    std::cout << src << std::endl << std::endl << "normized to" << std::endl;
    src = std::regex_replace(src, NEWLINES_RE, "\n");

    src = std::regex_replace(src, NULL_RE, "\uFFFD");
    std::cout << src << std::endl;
}