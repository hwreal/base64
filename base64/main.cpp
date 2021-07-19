//
//  main.cpp
//  base64
//
//  Created by hwreal on 2021/7/19.
//


#include <iostream>
#include "base64.h"

int main(int argc, const char * argv[]) {
    
    HWBase64 b64 = HWBase64();
    
    string ret1 = b64.encode("Man");
    cout<<"ret1: "<<ret1<<endl;
 
    string ret2 = b64.decode(ret1);
    cout<<"ret2: "<<ret2<<endl;
    
    return 0;
}
