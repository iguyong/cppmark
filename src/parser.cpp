//
// Created by yong gu on 5/3/18.
//

#include "parser.h"

#include <iostream>

using namespace std;

//void replace(string &source, string const &name, string val) {
//    val = regex_replace(val, std::regex("(^|[^\\[])\\^"), "$1");   // remove begin mark ^
//    for (string::size_type i = 0; (i = source.find(name, i)) != string::npos;) {
//        source.replace(i, name.length(), val);
//        i += val.length();
//    }
//}

string mangle(string text){
    return text;
}

void Parser::tokenBlock(string &src, bool top, bool bq) {
    //normalize
    src = regex_replace(src, std::regex("\\r\\n|\\r"), "\n");
    src = regex_replace(src, std::regex("\\t"), "    ");
    src = regex_replace(src, std::regex("\\u00a0"), " ");
    src = regex_replace(src, std::regex("\\u2424"), "\n");
    src = regex_replace(src, std::regex("(^|\n) +(?=\n|$)"), "$1");   //blank line

    //
    smatch matchs;

    while (!src.empty()) {
        Token t;
        // newline
        if (std::regex_search(src, matchs, rules.newline_re)) {
            if (matchs[1].str().length() > 1) {
                t.type = "space";
                tokens.push_back(t);
            }
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // code
        if (regex_search(src, matchs, regex("^( {4}[^\\n]+\\n*)+"))) {
            t.type = "code";
            t.text = regex_replace(matchs[0].str(), regex("(\\n*) {4}"), "$1");
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());  //chaging src would affect matchs
            continue;
        }

        // fences
        if (regex_search(src, matchs, rules.fences_re)) {
            t.type = "code";
            t.language = matchs[2].str();
            t.text = matchs[3].str();
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // heading
        if (regex_search(src, matchs, rules.heading_re)) {
            t.type = "heading";
            t.level = matchs[1].str().size();
            t.text = matchs[2].str();
            tokens.push_back(t);    // copy (not move)
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // lheading
        if (regex_search(src, matchs, rules.lheading_re)) {
            t.type = "heading";
            t.level = matchs[2].str() == "=" ? 1 : 2;
            t.text = matchs[1].str();
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //displaymath

        // hr
        if (regex_search(src, matchs, rules.hr_re)) {
            t.type = "hr";
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());
            continue;
        }
        // blockquote
        if (regex_search(src, matchs, rules.blockquote_re)) {
            t.type = "blockquote_start";
            tokens.push_back(t);
            string content = regex_replace(matchs[0].str(), regex("(^|\\n) *> ?"), "$1");
//            cout<<content<<endl;
            tokenBlock(content, top, true);
            t.type = "blockquote_end";
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());
            continue;
        }
        // list
        if (regex_search(src, matchs, rules.list_re)) {
            t.type = "list_start";
            t.ordered = matchs[2].str().size() > 1;
            tokens.push_back(t);

            //match items
            string content = matchs[0].str();
            smatch submatchs;
            bool next = false;
            vector<string> items;
            while(regex_search(content,submatchs,rules.item_re)){
                items.push_back(submatchs[0].str());
                int len = submatchs[0].str().size();
                if (len < content.size() - 1){        //note: to mimic multiline match
                    len++;
                }
                content = content.substr(len);
            }
            int l = items.size();
            for(int i = 0; i < l; i++){
                Token t;
                int space = items[i].length();
                string item = regex_replace(items[i],regex("^ *([*+-]|\\d+\\.) +"),"");
                smatch checkMatchs;
                if (regex_search(item,checkMatchs,rules.checkbox_re)){
                    t.checked = checkMatchs[1].str() == "x" ? 1 : 0;
                    item = regex_replace(item, regex(rules.checkbox_re),"");
                }else{
                    t.checked = -1;
                }

                //outdent whatever the list item contains. Hacky
                if(item.find("\n ") != string::npos){
                    space-= item.length();
                    item = regex_replace(item, regex(string("(^|\\n) {1,") + std::to_string(space) +"}"), "$1");
                }
//                cout<<item<<endl;
                bool loose = next || regex_search(item,regex("\\n\\n(?!\\s*$)"));
                if (i != l-1){
                    next = item[item.length() - 1] == '\n';
                    if (!loose){
                        loose = next;
                    }
                }

                t.type = loose ? "loose_item_start" : "list_item_start";
                tokens.push_back(t);
                tokenBlock(item, false, bq);
                t.type = "list_item_end";
                tokens.push_back(t);
            }
            t.type = "list_end";
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // html
        if (regex_search(src, matchs, rules.html_re)) {
            t.type = "html";
            string tmp = matchs[1].str();
            t.pre = (tmp == "pre" || tmp == "script" || tmp == "style");
            t.text = matchs[0].str();
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // link reference def
        if (!bq && top && regex_search(src, matchs, rules.def_re)) {
            Link link;
            link.href = matchs[2].str();
            link.title = matchs[3].str();
            links[matchs[1].str()] = link;
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // top-level paragraph
        if (top && regex_search(src, matchs, rules.paragraph_re)) {
            t.type = "paragraph";
            t.text = regex_replace(matchs[1].str(),regex("\\n$"),"");
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());
            continue;
        }
        // text
        if (regex_search(src, matchs, rules.text_re)) {
            // Top-level should never reach here.
            t.type = "text";
            t.text = matchs[0].str();
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());
            continue;
        }
        if (!src.empty()) {
            std::cerr << "Infinite loop at:" << std::endl << src << std::endl;
        }
    }
}

void Parser::parse() {
    tokenIndex = -1;
    while(next()){
        out += parseToken();
    }
}

string Parser::parseToken(){
    const string &type = token.type;
    string body("");
    //space
    if (type == "space"){
        return "";
    }

    //hr
    if (type == "hr"){
        return renderer.hr();
    }

    //heading
    if (type == "heading"){
        return renderer.heading(token.text,token.level,token.text);
    }

    //displaymath
    if (type == "displaymath"){
        return renderer.displaymath(token.text);
    }

    //code
    if (type == "code"){
        return renderer.code(token.text,token.language,token.escaped);
    }

    //blockquote_start
    if (type == "blockquote_start"){
        while(next() && token.type!= "blockquote_end"){
            body += parseToken();
        }
        return renderer.blockquote(body);
    }

    //list_start
    if (type == "list_start"){
        bool taskList = false;
        bool ordered = token.ordered;
        while (next() && token.type != "list_end"){
            if(token.checked >= 0){
                taskList = true;
            }
            body += parseToken();
        }
        return renderer.list(body,ordered,taskList);
    }

    //list_item_start
    if (type == "list_item_start"){
        while (next() && token.type != "list_item_end"){
            if(token.type == "text"){
                body += parseText();
            }else {
                body += parseToken();
            }
        }
        return renderer.listitem(body,token.checked);
    }

    //loose_item_start
    if (type == "loose_item_start") {
        while (next() && token.type != "list_item_end"){
            body += parseToken();
        }
        return renderer.listitem(body, token.checked);
    }

    //html
    if (type == "html"){
        string html;
        if (!token.pre){
            html = parseInline(token.text);
        }else{
            html = token.text;
        }
        return renderer.html(html);
    }

    //paragraph
    if (type == "paragraph"){
        return renderer.paragraph(parseInline(token.text));
    }

    //text
    if (type == "text"){
        return renderer.paragraph(parseText());
    }
    return "";
}
std::string Parser::parseText(){
    string body = token.text;
    Token nToken;
    while (peek(nToken) && nToken.type == "text"){
        next();
        body += "\n" + token.text;
    }
    return parseInline(body);
}
std::string Parser::parseInline(std::string& src){
    string out("");
    smatch matchs;
    while(!src.empty()){
        //escape
        if (regex_search(src,matchs,rules.escape_re)){
            out += matchs[1].str();
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //autolink
        if (regex_search(src,matchs,rules.autolink_re)){
            string text;
            string href;
            if(matchs[2].str() == "@"){
                if (matchs[1].str()[6] == ':'){
                    text = mangle(matchs[1].str().substr(7));
                }else{
                    text = mangle(matchs[1].str());
                }
                href = mangle("mailto:") + text;
            }else{
                text = escape(matchs[1].str(),false);
                href = text;
            }
            out += renderer.link(href,"", text);
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //url
        if (regex_search(src,matchs,rules.url_re)){
            string text = escape(matchs[1].str(),false);
            string href = text;
            out += renderer.link(href,"", text);
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //tag
        if (regex_search(src,matchs,rules.tag_re)){
            if(!renderer.inlink && regex_search(matchs[0].str(),regex("^<a ", std::regex::icase))){
                renderer.inlink = true;
            }else if(renderer.inlink && regex_search(matchs[0].str(),regex("^<\\/a ", std::regex::icase))){
                renderer.inlink = false;
            }
            out += matchs[0].str();
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //link or image
        if (regex_search(src,matchs,rules.link_re)){
            string href = escape(matchs[2].str(),false);
            string title = escape(matchs[3].str(),false);
            string text = escape(matchs[1].str(),false);
            renderer.inlink = true;
            if(matchs[0].str()[0] != '!'){
                out += renderer.link(href,title,parseInline(text));
            }else{
                out += renderer.image(href,title,text);
            }
            renderer.inlink = false;
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //reflink, nolink
        if (regex_search(src,matchs,rules.reflink_re)){
            string text;
            if(matchs[2].str().empty()){
                text = matchs[2].str();
            }else{
                text = matchs[1].str();
            }
            text = regex_replace(text, regex("\\s+"), " ");
            transform(text.begin(),text.end(),text.begin(),::tolower);
            if(links.find(text) == links.end() || links[text].href.empty()){
                out += matchs[0].str()[0];
                src = src.substr(matchs[0].str().size());
                src = matchs[0].str().substr(1) + src;
                continue;
            }
            string href = links[text].href;
            string title = links[text].title;
            renderer.inlink = true;
            if(matchs[0].str()[0] != '!'){
                out += renderer.link(href,title,parseInline(text));
            }else{
                out += renderer.image(href,title,text);
            }
            renderer.inlink = false;
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // inlinemath
        if (regex_search(src,matchs,rules.inlinemath_re)){
            out += renderer.inlinemath(matchs[0].str());
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //strong
        if (regex_search(src,matchs,rules.strong_re)){
            string text;
            if(!matchs[2].str().empty()){
                text = matchs[2].str();
            }else{
                text = matchs[1].str();
            }
            out += renderer.strong(parseInline(text));
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //em
        if (regex_search(src,matchs,rules.em_re)){
            string text;
            if(!matchs[2].str().empty()){
                text = matchs[2].str();
            }else{
                text = matchs[1].str();
            }
            out += renderer.em(parseInline(text));
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // icode
        if (regex_search(src,matchs,rules.icode_re)){
            out += renderer.codespan(escape(matchs[2].str(),true));
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // br
        if (regex_search(src,matchs,rules.br_re)){
            string text {matchs[0].str()};
            out += renderer.br();
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //del
        if (regex_search(src,matchs,rules.del_re)){
            string text {matchs[1].str()};
            out += renderer.del(parseInline(text));
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // itext
        if (regex_search(src,matchs,rules.itext_re)){
            out += renderer.text(escape(matchs[0].str()));
            src = src.substr(matchs[0].str().size());
            continue;
        }

        if(!src.empty()){
            std::cerr << "ParseInline infinite loop at:" << std::endl << src << std::endl;
        }

    }
    return out;
}