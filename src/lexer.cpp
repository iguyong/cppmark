//
// Created by yong gu on 5/4/18.
//

#include "lexer.h"
#include <iostream>
using namespace std;

void Lexer::testToken(){
    cout<<src<<endl;
    cout<<endl<<"begin testing..."<<endl<<endl;
    src = regex_replace(src, std::regex("^ +$"),"");
    std::smatch matchs;

    rules.list = "^( *)- [\\s\\S]+?(?:\\n{2,}(?! )(?!\\1- )\\n*|\\s*$)";
    cout<<"matching list:"<<rules.list<<endl;
    try{
        std::regex re(rules.list);
        if(regex_match(src,matchs,re)){
            cout<<"matched"<<endl;
        }else{
            cout<<"not matched."<<endl;
        }
    }
    catch (const std::regex_error& e) {
        cout<<"regex error:"<<e.what()<<endl;
    }

}
void Lexer::lex(string& mdSrc) {
    src = mdSrc;
    src = regex_replace(src, std::regex("\\r\\n|\\r"),"\n");
    src = regex_replace(src, std::regex("\\t"), "    ");
    src = regex_replace(src, std::regex("\\u00a0"), " ");
    src = regex_replace(src, std::regex("\\u2424"), "\n");
    testToken();
}


void Lexer::token() {
//    std::cout<<src<<std::endl;
    src = regex_replace(src, std::regex("^ +$"),"");
//    std::cout<<src<<std::endl;
    std::smatch matchs;
    Token t;
    while(!src.empty()){
        // newline
        if(regex_search(src,matchs,std::regex(rules.newline))){
            src = src.substr(matchs[0].str().size());
            t.type="space";
            tokens.push_back(t);
        }
        // heading
        if(regex_search(src,matchs,std::regex(rules.heading))){
//            std::cout<<matchs[0]<<std::endl;
            src = src.substr(matchs[0].str().size());
            t.type="heading";
            t.level=matchs[1].str().size();
            t.content = matchs[2].str();
            tokens.push_back(t);    // copy (not move)
            cout<<"matched: heading"<<endl;
            continue;
        }
        // code
        if(regex_search(src,matchs,std::regex(rules.code))){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            tokens.push_back(t);
            cout<<"matched: code"<<endl;
            continue;
        }
        // fences

        if(regex_search(src,matchs,std::regex(rules.fences))){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type="fence";
            tokens.push_back(t);
            cout<<"matched: fence"<<endl;
            continue;
        }

        // hr
        if(regex_search(src,matchs,std::regex(rules.hr))){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type = "hr";
            tokens.push_back(t);
            cout<<"matched: hr"<<endl;
            continue;
        }
        // blockquote
        if(regex_search(src,matchs,std::regex(rules.blockquote))){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type="blockquote";
            tokens.push_back(t);
            cout<<"matched: blockquote"<<endl;
            continue;
        }
        // list
        if(regex_search(src,matchs,std::regex(rules.list))){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type="list";
            tokens.push_back(t);
            cout<<"matched: list"<<endl;
            continue;
        }
//        // html
//        if(regex_search(src,matchs,std::regex(rules.html))){
//            src = src.substr(matchs.size());
////            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
//            tokens.push_back(t);
//            continue;
//        }
        // top-level paragraph
        if(regex_search(src,matchs,std::regex(rules.paragraph))){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type="paragraph";
            tokens.push_back(t);
            cout<<"matched: paragraph"<<endl;
            continue;
        }
        // text
        if(regex_search(src,matchs,std::regex(rules.text))){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type="text";
            tokens.push_back(t);
            cout<<"matched: text"<<endl;
            continue;
        }
        if(!src.empty()){
            std::cerr<<"Infinite loop at:"<<std::endl<<src<<std::endl;
        }
    }

}

