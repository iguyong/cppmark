//
// Created by yong gu on 5/8/18.
//

#include "renderer.h"
#include <regex>
using namespace std;


std::string Renderer::code(std::string& code, std::string lang, bool escaped) {
    if(!escaped){
        code = escape(code,true);
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

std::string Renderer::heading(std::string& text, int level, std::string raw) {
    std::transform(raw.begin(),raw.end(),raw.begin(),::tolower);
    regex_replace(raw, regex("[^\\w]+"), "-");
    return "<h" + to_string(level) + " id=\"" + options.headerPrefix + raw + "\">" + text + "</h" + to_string(level) + ">\n";
}

std::string escape(const std::string &html, bool encode) {
    {
        std::string res(html);
        if (encode) {
            std::regex_replace(res,std::regex("&(?!#?\\w+;)"), "&amp;");
        }else{
            std::regex_replace(res,std::regex("&"), "&amp;");
        }
        std::regex_replace(res,std::regex("<"), "&lt;");
        std::regex_replace(res,std::regex(">"), "&gt;");
        std::regex_replace(res,std::regex("\""), "&quot;");
        std::regex_replace(res,std::regex("'"), "&#39;");
        return res;
    }
}
