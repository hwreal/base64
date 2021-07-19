//
//  base64.h
//  base64
//
//  Created by hwreal on 2021/7/19.
//

#ifndef base64_h
#define base64_h

#include <vector>
#include <string>

using namespace std;

class HWBase64 {
public:
    HWBase64();
    ~HWBase64();
    
public:
    
    const string &encode(const string strInput);
    const string &decode(const string strInput);
    
private:
    char getB64char(uint8_t nIndex);
    uint8_t getB64Index(char c);
    
private:
    vector<string*> m_arrEnc;
    vector<string*> m_arrDec;
};


#endif /* base64_h */
