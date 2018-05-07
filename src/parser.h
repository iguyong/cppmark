//
// Created by yong gu on 5/3/18.
//

#ifndef CPPMARK_BLOCK_H
#define CPPMARK_BLOCK_H

#include <regex>
#include <string>
#include "doc.h"

class Parser {
public:
    Parser(std::string flavor = "normal");
    void token(Doc& doc, bool top = true);
    std::regex newline_re;
    std::regex code_re;
    std::regex fences_re;
    std::regex hr_re;
    std::regex heading_re;
    std::regex blockquote_re;
    std::regex list_re;
    std::regex item_re;
    std::regex html_re;
    std::regex def_re;
    std::regex lheading_re;
    std::regex displaymath_re;
    std::regex paragraph_re;
    std::regex text_re;

private:
    //std::regex sources
    std::string newline {"^\\n+"};
    std::string code {"^( {4}[^\\n]+\\n*)+"};
    std::string fences {"^ *(`{3,}|~{3,})[ \\.]*(\\S+)? *\\n([\\s\\S]*?)\\n? *\\1 *(?:\\n+|$)"};
    std::string hr {"^( *[-*_]){3,} *(?:\\n+|$)"};
    std::string heading {"^ *(#{1,6}) +([^\\n]+?) *#* *(?:\\n+|$)"};
    std::string lheading { "^([^\\n]+)\\n *(=|-){2,} *(?:\\n+|$)"};
    std::string displaymath {"^ *\\$\\$([\\s\\S]+?)\\$\\$ *"};
    std::string nptable {""};
    std::string blockquote {"^( *>[^\\n]+(\\n(?!def)[^\\n]+)*\\n*)+"};
    std::string list {"^( *)(bull) [\\s\\S]+?(?:hr|def|\\n{2,}(?! )(?!\\1bull )\\n*|\\s*$)"};
    std::string html {"^ *(?:comment *(?:\\n|\\s*$)|closed *(?:\\n{2,}|\\s*$)|closing *(?:\\n{2,}|\\s*$))"};
    std::string def {"^ *\\[([^\\]]+)\\]: *<?([^\\s>]+)>?(?: +[\"(]([^\\n]+)[\")])? *(?:\\n+|$)"};
    std::string table {""};
    std::string paragraph {
            "^((?:[^\\n]+\\n?(?!hr|heading|lheading|blockquote|tag|def|displaymath))+)\\n*"};
    std::string text { "(^[^\\n]+)"};
    std::string bullet {"(?:[*+-]|\\d+\\.)"};
    std::string item { "^( *)(bull) [^\\n]*(?:\\n(?!\\1bull )[^\\n]*)*"};
    std::string checkbox {"^\\[([ x])\\] +"};
};



#endif //CPPMARK_BLOCK_H
