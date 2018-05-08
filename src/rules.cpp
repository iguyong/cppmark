//
// Created by yong gu on 5/3/18.
//

#include "rules.h"

using namespace std;


// consider using only one regex_replace call instead ?
void replace(string &source, const string &name, const string& val) {
    string newVal = regex_replace(val, std::regex("(^|[^\\[])\\^"), "$1");   // remove begin mark ^
    for (string::size_type i = 0; (i = source.find(name, i)) != string::npos;) {
        source.replace(i, name.length(), newVal);
        i += val.length();
    }
}

Rules::Rules() {
    newline_re = regex(newline);
    code_re = regex(code);
    fences_re = regex(fences);
    hr_re = regex(hr);
    heading_re = regex(heading);
    lheading_re = regex(lheading);
    displaymath_re = regex(displaymath);
    def_re = regex(def);
    text_re = regex(text);
    checkbox_re = regex(checkbox);

    replace(item, "bull", bullet);
    item_re = regex(item);

    string _hr{"\\n+(?=\\1?(?:[-*_] *){3,}(?:\\n+|$))"};
    string _heading = "\\n+(?=" + heading + ")";
    string _lheading = "\\n+(?=" + lheading + ")";
    replace(list, "bull", bullet);
    replace(list, "hr", _hr);
    string _def = "\\n+(?=" + def + ")";
    replace(list, "def", _def);
//    replace(list,"heading",_heading);
//    replace(list,"lheading",_lheading);
    list_re = regex(list);

    replace(blockquote, "def", def);
    blockquote_re = regex(blockquote);

    string _tag{"(?!(?:a|em|strong|small|s|cite|q|dfn|abbr|data|time|code"
                        "|var|samp|kbd|sub|sup|i|b|u|mark|ruby|rt|rp|bdi|bdo"
                        "|span|br|wbr|ins|del|img)\\b)\\w+(?!:/|[^\\w\\s@]*@)\\b"};
    string _attribute{" +[a-zA-Z:_][\\w.:-]*(?: *= *\"[^\"\\n]*\"| *= *'[^'\\n]*'| *= *[^\\s\"'=<>`]+)?"};
    string _comment{"<!--(?!-?>)[\\s\\S]*?-->"};
    replace(html, "comment", _comment);
    replace(html, "closed", "<(tag)[\\s\\S]+?<\\/\\1>");
    replace(html, "closing", "<tag(?:\"[^\"]*\"|'[^']*'|[^'\">])*?>");
    replace(html, "tag", _tag);
    html_re = regex(html);

    replace(paragraph, "hr", hr);
    replace(paragraph, "heading", heading);
    replace(paragraph, "lheading", lheading);
    replace(paragraph, "blockquote", blockquote);
    replace(paragraph, "tag", "<" + _tag);
    replace(paragraph, "def", def);
    replace(paragraph, "displaymath", displaymath);


    nptable = "^ *([^|\\n ].*\\|.*)\\n *([-:]+ *\\|[-| :]*)(?:\\n((?:.*[^>\\n ].*(?:\\n|$))*)\\n*|$)";
    table = "^ *\\|(.+)\\n *\\|?( *[-:]+[-| :]*)(?:\\n((?: *[^>\\n ].*(?:\\n|$))*)\\n*|$)";
    string _fences{fences};
    replace(_fences, "\\1", "\\2");
    string _list{list};
    replace(_list, "\\1", "\\3");
    replace(paragraph, "(?!", "(?!" + _fences + "|" + _list + "|");

    paragraph_re = regex(paragraph);


    //inline rules
    string _inside {"(?:\\[[^\\]]*\\]|[^\\[\\]]|\\](?=[^\\[]*\\]))*"};
    string _href {"\\s*<?([\\s\\S]*?)>?(?:\\s+['\"]([\\s\\S]*?)['\"])?\\s*"};
    replace(link,"inside",_inside);
    replace(link,"href",_href);
    link_re = regex(link);
    replace(reflink,"inside",_inside);
    reflink_re = regex(reflink);
}