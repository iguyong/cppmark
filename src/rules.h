//
// Created by yong gu on 5/3/18.
//

#ifndef CPPMARK_RULE_H
#define CPPMARK_RULE_H

#include <string>
#include <regex>

class Rules {
public:
    Rules();
    
    //block rules
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
    std::regex checkbox_re;
    
    //inline rules
    std::regex escape_re {"\\\\([\\\\`*{}\\[\\]()#+\\$\\-.!_>])"};
    std::regex autolink_re {"^<([^ >]+(@|:\\/)[^ >]+)>"};
    std::regex url_re {"^(https?:\\/\\/[^\\s<]+[^<.,:;\"')\\]\\s])"};
    std::regex tag_re {"^<!--[\\s\\S]*?-->|^<\\/?\\w+(?:\"[^\"]*\"|'[^']*'|[^'\">])*?>"};
    std::regex inlinemath_re {"^\\$([\\s\\S]+?)\\$"};
    std::regex link_re;
    std::regex reflink_re;
    std::regex nolink_re {"^!?\\[((?:\\[[^\\]]*\\]|[^\\[\\]])*)\\]"};
    std::regex strong_re {"^__([\\s\\S]+?)__(?!_)|^\\*\\*([\\s\\S]+?)\\*\\*(?!\\*)"};
    std::regex em_re {"^\\b_((?:[^_]|__)+?)_\\b|^\\*((?:\\*\\*|[\\s\\S])+?)\\*(?!\\*)"};
    std::regex icode_re {"^(`+)\\s*([\\s\\S]*?[^`])\\s*\\1(?!`)"};
    std::regex br_re {"^ {2,}\\n(?!\\s*$)"};
    std::regex del_re {"^~~(?=\\S)([\\s\\S]*?\\S)~~"};
    std::regex itext_re {"^[\\s\\S]+?(?=[\\\\<!\\[_*`\\$~]|https?://| {2,}\\n|$)"};

    

private:
    //block regex string
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
    std::string def {"^ *\\[([^\\]]+)\\]: *<?([^\\s>]+)>?(?: +[\"(]([^\\n]+)[\")])? *(?:\\n+|$)"}; //link reference definition
    std::string table {""};
    std::string paragraph {
            "^((?:[^\\n]+\\n?(?!hr|heading|lheading|blockquote|tag|def|displaymath))+)\\n*"};
    std::string text { "(^[^\\n]+)"};
    std::string bullet {"(?:[*+-]|\\d+\\.)"};
    std::string item { "^( *)(bull) [^\\n]*(?:\\n(?!\\1bull )[^\\n]*)*"};
    std::string checkbox {"^\\[([ x])\\] +"};
    
    //inline regex string
    std::string link {"^!?\\[(inside)\\]\\(href\\)"};
    std::string reflink {"^!?\\[(inside)\\]\\s*\\[([^\\]]*)\\]"};
};



#endif //CPPMARK_RULE_H
