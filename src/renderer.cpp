//
// Created by yong gu on 5/8/18.
//

#include "renderer.h"
#include <regex>
#include <utility>
using namespace std;


std::string Renderer::code(const std::string& text, const std::string& lang, bool escaped) {
    string code {text};
    if(!escaped){
        code = escape(text,true);
    }
    if(!options.codeTags) {
        return code;
    }
    if(lang.empty()){
        return "<pre><code>" + code + "</code></pre>";
    }else{
        return "<pre><code class=\"" + options.langPrefix + escape(lang,true) +"\">" + code + "</code></pre>";
    }
}

std::string Renderer::heading(const std::string& text, int level, const std::string& raw) {
    string raw_id {raw};
    std::transform(raw_id.begin(),raw_id.end(),raw_id.begin(),::tolower);
    raw_id = regex_replace(raw, regex("[^\\w]+"), "-");
    return "<h" + to_string(level) + " id=\"" + options.headerPrefix + raw_id + "\">" + text + "</h" + to_string(level) + ">\n";
}

std::string escape(const std::string &html, bool encode) {
    {
        std::string res {""};
        if (html.empty()){
            return res;
        }
        if (encode) {
            res = std::regex_replace(html,std::regex("&(?!#?\\w+;)"), "&amp;");
        }else{
            res = std::regex_replace(html,std::regex("&"), "&amp;");
        }
        res = std::regex_replace(res,std::regex("<"), "&lt;");
        res = std::regex_replace(res,std::regex(">"), "&gt;");
        res = std::regex_replace(res,std::regex("\""), "&quot;");
        res = std::regex_replace(res,std::regex("'"), "&#39;");
        return res;
    }
}

std::string unescape(const std::string &html) {
    string res {""};
    smatch matchs;
    regex_search(html,matchs,regex("&([#\\w]+);"));
    string n = matchs[1].str();
    string r{""};
    std::transform(n.begin(),n.end(),n.begin(),::tolower);
    if (n == "colon"){
        r = ":";
    }else if (n[0] == '#'){
        r = n[1] == 'x' ? std::to_string(std::stol(n.substr(2),nullptr,16)) : n.substr(1);
    }
    res = regex_replace(html,regex("&([#\\w]+);"),r);
    return res;
}
