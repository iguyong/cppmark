//
// Created by yong gu on 5/4/18.
//

#ifndef CPPMARK_LEXER_H
#define CPPMARK_LEXER_H

#include "parser.h"
#include "token.h"
#include <vector>
class Lexer{
public:
    Lexer(Block &rules):rules(rules){}
    Block rules;
    void lex(string& src);
    void testToken();
    void token();

    string src;
    std::vector<Token> tokens;
};
#endif //CPPMARK_LEXER_H
