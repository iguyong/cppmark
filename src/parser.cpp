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
//    for(int i = 0;i<src.length();i++){
//        cout<<i<<": "<<src[i]<<endl;
//    }
//    cout<<"before: "<<src<<endl;
    src = regex_replace(src, regex("\\r\\n|\\r"), "\n");
    src = regex_replace(src, regex("\\t"), "    ");
    src = regex_replace(src, regex("\u00a0"), " ");   // do not use "\\u00a0"
    src = regex_replace(src, regex("\u2424"), "\n");
    src = regex_replace(src, regex("(^|\\n) +(?=\\n|$)"), "$1");   //blank line
//    cout<<endl;
//    for(int i = 0;i<src.length();i++){
//        cout<<i<<": "<<src[i]<<endl;
//    }
//    cout<<"after"<<src<<endl;
//    return;
    //
    smatch matchs;
    Token t;
    while (!src.empty()) {
        t.reset();     // t can be reused because vector.push_back() would copy it
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
        if (regex_search(src, matchs, rules.code_re)) {
            t.type = "code";
            string text = regex_replace(matchs[0].str(), regex("(\\n*) {4}"), "$1");   // mimic / {4}/gm
            t.text = regex_replace(text, regex("\\n*$"),"");     //striping trailing blank lines
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
            tokens.push_back(t);
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
        if (regex_search(src, matchs, rules.displaymath_re)){
            t.type = "displaymath";
            t.text = matchs[1].str();
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());
        }

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
                    space -= item.length();
                    item = regex_replace(item, regex(string("(^|\\n) {1,") + std::to_string(space) +"}"), "$1");
                }
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
            t.pre = (tmp == "pre" || tmp == "script" || tmp == "style"); // preformatted
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
        // text (i.e. non top-levl paragraph)
        if (regex_search(src, matchs, rules.text_re)) {
            // Top-level should never reach here.
            t.type = "text";
            t.text = matchs[0].str();
            tokens.push_back(t);
            src = src.substr(matchs[0].str().size());
            continue;
        }
        if (!src.empty()) {
            std::cerr << "TokenBlock: Infinite loop at:" << std::endl << src << std::endl;
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
        return renderer.heading(parseInline(token.text),token.level,token.text);
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
        string body {""};
        while(next() && token.type!= "blockquote_end"){
            body += parseToken();
        }
        return renderer.blockquote(body);
    }

    //list_start
    if (type == "list_start"){
        string body {""};
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
        string body{""};
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
        string body {""};
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

// concatenate consecutive block text
std::string Parser::parseText(){
    string body = token.text;
    Token nToken;
    while (peek(nToken) && nToken.type == "text"){
        next();
        body += "\n" + token.text;
    }
    return parseInline(body);
}


std::string Parser::parseInline(const std::string& text){
    string src {text};
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
            string matchStr {matchs[1].str()};
            if(matchs[2].str() == "@"){
                if (matchs[1].str()[6] == ':'){
                    text = mangle(matchStr.substr(7));
                }else{
                    text = mangle(matchStr);
                }
                href = mangle("mailto:") + text;
            }else{
                text = escape(matchStr,false);
                href = text;
            }
            out += renderer.link(href, "", text);  //no title
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //url
        if (!renderer.inlink && regex_search(src,matchs,rules.url_re)){
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
            }else if(renderer.inlink && regex_search(matchs[0].str(),regex("^<\\/a> ", std::regex::icase))){
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
        if (regex_search(src,matchs,rules.reflink_re) || regex_search(src,matchs,rules.nolink_re)){
            string text;
            if(!matchs[2].str().empty()){
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
                out += renderer.image(href,title,escape(text,false));
            }
            renderer.inlink = false;
            src = src.substr(matchs[0].str().size());
            continue;
        }

        // inlinemath
        if (regex_search(src,matchs,rules.inlinemath_re)){
            out += renderer.inlinemath(matchs[1].str());
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
            out += renderer.br();
            src = src.substr(matchs[0].str().size());
            continue;
        }

        //del
        if (regex_search(src,matchs,rules.del_re)){
            out += renderer.del(parseInline(matchs[1].str()));
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