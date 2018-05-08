//
// Created by yong gu on 5/3/18.
//

#include "token.h"
#include <iostream>
using namespace std;

void Token::print() const {
    cout<<type;
    if (type=="fences" || type=="code"){
        cout<<": "<<language<<endl<<text;
    }else if(type=="heading" || type=="lheading"){
        cout<<level<<": "<<text;
    }
    cout<<endl;
}
