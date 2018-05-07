//
// Created by yong gu on 5/3/18.
//

#include "parser.h"

#include <iostream>
using namespace std;

void replace(string& source, string const& name, string val)
{
    val = regex_replace(val,std::regex("(^|[^\\[])\\^"),"$1");   // remove begin mark ^
    for(string::size_type i = 0; (i = source.find(name, i)) != string::npos;)
    {
        source.replace(i, name.length(), val);
        i += val.length();
    }
}



Parser::Parser(string flavor) {
    newline_re = regex(newline);
    code_re = regex(code);
    fences_re = regex(fences);
    hr_re = regex(hr);
    heading_re = regex(heading);
    lheading_re = regex(lheading);
    displaymath_re = regex(displaymath);
    def_re = regex(def);
    text_re = regex(text);

    replace(item,"bull",bullet);
    item_re = regex(item);

    string _hr {"\\n+(?=\\1?(?:[-*_] *){3,}(?:\\n+|$))"};
    string _heading = "\\n+(?="+heading+")";
    string _lheading = "\\n+(?="+lheading+")";
    replace(list,"bull",bullet);
    replace(list,"hr",_hr);
    string _def = "\\n+(?="+def+")";
    replace(list,"def",_def);
//    replace(list,"heading",_heading);
//    replace(list,"lheading",_lheading);
    list_re = regex(list);

    replace(blockquote,"def",def);
    blockquote_re = regex(blockquote);

    string _tag {"(?!(?:a|em|strong|small|s|cite|q|dfn|abbr|data|time|code"
                         "|var|samp|kbd|sub|sup|i|b|u|mark|ruby|rt|rp|bdi|bdo"
                         "|span|br|wbr|ins|del|img)\\b)\\w+(?!:/|[^\\w\\s@]*@)\\b"};
    string _attribute {" +[a-zA-Z:_][\\w.:-]*(?: *= *\"[^\"\\n]*\"| *= *'[^'\\n]*'| *= *[^\\s\"'=<>`]+)?"};
    string _comment {"<!--(?!-?>)[\\s\\S]*?-->"};
    replace(html,"comment",_comment);
    replace(html,"closed","<(tag)[\\s\\S]+?<\\/\\1>");
    replace(html,"closing","<tag(?:\"[^\"]*\"|'[^']*'|[^'\">])*?>");
    replace(html,"tag",_tag);
    html_re = regex(html);

    replace(paragraph,"hr",hr);
    replace(paragraph,"heading",heading);
    replace(paragraph,"lheading",lheading);
    replace(paragraph,"blockquote",blockquote);
    replace(paragraph,"tag","<"+_tag);
    replace(paragraph,"def",def);
    replace(paragraph,"displaymath",displaymath);


    if (flavor == "gfm"){
        nptable = "^ *([^|\\n ].*\\|.*)\\n *([-:]+ *\\|[-| :]*)(?:\\n((?:.*[^>\\n ].*(?:\\n|$))*)\\n*|$)";
        table = "^ *\\|(.+)\\n *\\|?( *[-:]+[-| :]*)(?:\\n((?: *[^>\\n ].*(?:\\n|$))*)\\n*|$)";
        string _fences {fences};
        replace(_fences,"\\1","\\2");
        string _list {list};
        replace(_list,"\\1","\\3");
        replace(paragraph,"(?!","(?!"+_fences+"|"+_list+"|");
    }
    paragraph_re = regex(paragraph);
}

void Parser::token(Doc &doc, bool top) {
    string& src {doc.src};
    
    //normalize
    src = regex_replace(src, std::regex("\\r\\n|\\r"),"\n");
    src = regex_replace(doc.src, std::regex("\\t"), "    ");
    src = regex_replace(src, std::regex("\\u00a0"), " ");
    src = regex_replace(src, std::regex("\\u2424"), "\n");
    src = regex_replace(src, std::regex("(^|\n) +(?=\n|$)"),"$1");   //blank line

    //
    smatch matchs;
    Token t;
    while(!src.empty()){
        // newline
        if(std::regex_search(src, matchs, newline_re)){
            src = src.substr(matchs[0].str().size());
            if(matchs[1].str().length() > 1) {
                t.type = "space";
                doc.tokens.push_back(t);
            }
        }
        // code
        if(regex_search(src,matchs,code_re)){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type = "code";
            doc.tokens.push_back(t);
//            cout<<"matched: code"<<endl;
            continue;
        }
        // fences

        if(regex_search(src,matchs,fences_re)){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type="fences";
            doc.tokens.push_back(t);
//            cout<<"matched: fence"<<endl;
            continue;
        }

        // heading
        if(regex_search(src,matchs,heading_re)){
//            std::cout<<matchs[0]<<std::endl;
            src = src.substr(matchs[0].str().size());
            t.type="heading";
            t.level=matchs[1].str().size();
            t.content = matchs[2].str();
            doc.tokens.push_back(t);    // copy (not move)
//            cout<<"matched: heading"<<endl;
            continue;
        }

        // lheading
        if(regex_search(src,matchs,lheading_re)){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type = "lheading";
            doc.tokens.push_back(t);
            continue;
        }

        //displaymath

        // hr
        if(regex_search(src,matchs,hr_re)){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type = "hr";
            doc.tokens.push_back(t);
//            cout<<"matched: hr"<<endl;
            continue;
        }
        // blockquote
        if(regex_search(src,matchs,blockquote_re)){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type="blockquote";
            doc.tokens.push_back(t);
//            cout<<"matched: blockquote"<<endl;
            continue;
        }
        // list
        if(regex_search(src,matchs,list_re)){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type="list";
            doc.tokens.push_back(t);
//            cout<<"matched: list"<<endl;
            continue;
        }

        // html
        if(regex_search(src,matchs,html_re)){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type = "html";
            doc.tokens.push_back(t);
            continue;
        }
        // def
        if(top && regex_search(src,matchs,def_re)){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type = "def";
            doc.tokens.push_back(t);
            continue;
        }

        // top-level paragraph
        if(top && regex_search(src,matchs,paragraph_re)){
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type="paragraph";
            doc.tokens.push_back(t);
//            cout<<"matched: paragraph"<<endl;
            continue;
        }
        // text
        if(regex_search(src,matchs,text_re)){
            // Top-level should never reach here.
            src = src.substr(matchs[0].str().size());
//            regex_replace(matchs[0].str(),std::regex("^ {4}"), "");
            t.type="text";
            doc.tokens.push_back(t);
//            cout<<"matched: text"<<endl;
            continue;
        }
        if(!src.empty()){
            std::cerr<<"Infinite loop at:"<<std::endl<<src<<std::endl;
        }
    }
}
