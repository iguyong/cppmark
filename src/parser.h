//
// Created by yong gu on 5/3/18.
//

#ifndef CPPMARK_BLOCK_H
#define CPPMARK_BLOCK_H

#include <regex>
#include <string>
#include <unordered_map>
#include "token.h"
#include "rules.h"
#include "options.h"
#include "renderer.h"

class Link{
public:
//    Link(std::string href, std::string title):href(href),title(title){}
//    Link& operator=(Link& link) = default;
    string href;
    string title;
};

class Parser {
public:
    Parser(){};
    Parser(Options& options):options(options),renderer(options){}
    Renderer renderer;
    Rules rules;
    void tokenBlock(std::string &src, bool top = true, bool bq = false);     // get block tokens
    void parse();
    std::string parseToken();
    std::string parseText();
    std::string parseInline(std::string& text);

    std::string out;   //output html
    Options options;
    std::vector<Token> tokens;
    std::unordered_map<std::string,Link> links;

private:
    //tokens access
    int tokenIndex = -1;
    Token token;  //current token
    bool next(){
        if (tokenIndex + 1 < (int)tokens.size()){
            tokenIndex ++;
            token = tokens[tokenIndex];
            return true;
        }else{
            return false;
        }
    }
    bool peek(Token& nToken){
        if (tokenIndex + 1 < (int)tokens.size()){
            nToken = tokens[tokenIndex+1];            // preview next token
            return true;
        }else{
            return false;
        }
    }
};



#endif //CPPMARK_BLOCK_H
