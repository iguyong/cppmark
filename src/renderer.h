//
// Created by yong gu on 5/8/18.
//

#ifndef CPPMARK_RENDERER_H
#define CPPMARK_RENDERER_H

#include <string>
#include <algorithm>
#include <regex>
#include "options.h"

std::string escape(const std::string& html, bool encode = false);

class Renderer {
public:
    Renderer(){}
    Renderer(const Options &options) : options(options) {}

    Options options;
    bool inlink = false;

    //code and fences
    std::string code(std::string &code, std::string lang, bool escaped);

    std::string blockquote(std::string quote) {
        return "<blockquote>\n" + quote + "</blockquote>\n";
    }

    std::string html(std::string html) { return html; }

    std::string heading(std::string& text, int level, std::string raw);

    std::string math(std::string text, bool display) {
        if (display) {
            return "<div class=\"displaymath\">" + escape(text, true) + "\n</div>";
        } else {
            return "<span class=\"inlinemath\">" + escape(text, true) + "\n</span>";
        }
    }

    std::string displaymath(std::string text) {
        return math(text, true);
    }

    std::string hr() {
        return "<hr>\n";
    }

    std::string list(std::string body, bool ordered, bool taskList) {
        std::string type(ordered ? "ol" : "ul");
        std::string classes(taskList ? " class=\"task-list\"" : "");
        return "<" + type + classes + ">\n" + body + "</" + type + ">\n";
    }

    std::string listitem(std::string text, int checked = -1) {
        if (checked < 0) {
            return "<li>" + text + "</li>\n";
        }
        return "<li class=\"task-list-item\"> <input type=\"checkbox\" class=\"task-list-item-checkbox\" disabled" +
               std::string(checked > 0 ? " checked" : "") + ">" + text + "</li>\n";
    }

    std::string paragraph(std::string text){
        return "<p>" + text + "</p>\n";
    }

    std::string strong(std::string text) {
        return "<strong>" + text + "</strong>";
    }

    std::string em(std::string text) {
        return "<em>" + text + "</em>";
    }

    std::string codespan(std::string text) {
        return "<code>" + text + "</code>";
    }

    std::string br() {
        return "<br>";
    }

    std::string del(std::string text) {
        return "<del>" + text + "</del>";
    }

    std::string link(std::string href, std::string title, std::string text) {
        std::string out = "<a href=\"" + href + "\"";
        if(!title.empty()){
            out += " title=\"" + title + "\"";
        }
        out += ">" + text + "</a>";
        return out;
    }

    std::string inlinemath(std::string text) {
        return math(text,false);
    }

    std::string image(std::string href, std::string title, std::string text) {
        std::string out = "<img src=\"" + href + "\" alt=\"" + text + "\"";
        if(!title.empty()){
            out += " title=\"" + title + "\"";
        }
        out += ">";
        return out;
    }

    std::string text(std::string text){
        return text;
    }
};


#endif //CPPMARK_RENDERER_H
