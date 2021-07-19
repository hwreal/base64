//
//  base64.cpp
//  base64
//
//  Created by hwreal on 2021/7/19.
//

#include "base64.h"

#define u8(x) ((uint8_t)(x))

HWBase64::HWBase64(){
}

HWBase64::~HWBase64(){
    printf("\n~HWBase64 析构函数 ~\n");
    if (!m_arrEnc.empty()) {
        printf("** 清除编码数据\n");
        for (size_t i=0; i<m_arrEnc.size(); i++) {
            delete m_arrEnc[i];
        }
        m_arrEnc.clear();
    }
    if (!m_arrDec.empty()) {
        printf("** 清除解码数据\n");
        for (size_t i=0; i<m_arrEnc.size(); i++) {
            delete m_arrEnc[i];
        }
        m_arrEnc.clear();
    }
}


char HWBase64::getB64char(uint8_t nIndex) {
    
    static const char szTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    if (nIndex >= 0 && nIndex < 64)
    {
        return szTable[nIndex];
    }
    return '=';
}

uint8_t HWBase64::getB64Index(char c) {
    uint8_t index = -1;
    if (c >= 'A' && c <= 'Z') {
        index = c - 'A';
    }else if (c >= 'a' && c <= 'z'){
        index = c - 'a' + 26;
    }else if(c >= '0' && c <= '9'){
        index = c - '0' + 52;
    }else if(c == '+'){
        index = 62;
    }else if(c == '/'){
        index = 63;
    }
    return index;
}

const string &HWBase64::encode(const string strInput){
    
    string &encStr = *(new string);
    m_arrEnc.push_back(&encStr);
    
    // 三个字符为一组, 先编码整组的
    size_t countOfGroup = strInput.length() / 3;
    for (size_t i = 0; i < countOfGroup; i++) {
        char c1 = strInput[i*3+0];
        char c2 = strInput[i*3+1];
        char c3 = strInput[i*3+2];
        
        uint8_t index1 = u8(c1>>2);
        uint8_t index2 = u8(u8(c1<<6)>> 2) | u8(c2>>4);
        uint8_t index3 = u8(u8(c2 << 4)>>2) | u8(c3>>6);
        uint8_t index4 = u8(u8(c3<<2)>>2);
        
        encStr.push_back(getB64char(index1));
        encStr.push_back(getB64char(index2));
        encStr.push_back(getB64char(index3));
        encStr.push_back(getB64char(index4));
    }
    
    // 再编码剩余的,先计算剩余几个字符
    int countOfRestChars = strInput.length() % 3;
    if (countOfRestChars == 1) {
        char c1 = strInput[countOfGroup*3];
        
        uint8_t index1 = u8(c1>>2);
        uint8_t index2 = u8(u8(c1<<6)>>2);
        
        encStr.push_back(this->getB64char(index1));
        encStr.push_back(this->getB64char(index2));
        
        encStr.push_back('=');
        encStr.push_back('=');
    }
    
    if (countOfRestChars == 2) {
        char c1 = strInput[countOfGroup*3];
        char c2 = strInput[countOfGroup*3+1];
        
        uint8_t index1 = u8(c1>>2);
        uint8_t index2 = u8(u8(c1<<6)>> 2) | u8(c2>>4);
        uint8_t index3 = u8(u8(c2<<4)>>2);
        
        encStr.push_back(this->getB64char(index1));
        encStr.push_back(this->getB64char(index2));
        encStr.push_back(this->getB64char(index3));
        
        encStr.push_back('=');
    }
    
    return encStr;
}


const string &HWBase64::decode(const string strInput){
    
    string *pDecStr = new string;
    m_arrDec.push_back(pDecStr);
    string &decStr = *pDecStr;
    
    size_t strLen = strInput.length();
    
    if (strLen < 4) {
        return decStr;
    }
    
    size_t countOfGroup = strLen / 4;
    char lastChar = strInput[strLen - 1]; // 最后一个字符
    bool hasPading = lastChar == '=';
    
    if (hasPading) {
        countOfGroup -= 1;
    }
    
    for (int i = 0; i<countOfGroup; i++) {
        char c1 = strInput[i*4];
        char c2 = strInput[i*4+1];
        char c3 = strInput[i*4+2];
        char c4 = strInput[i*4+3];
        
        uint8_t index1 = this->getB64Index(c1);
        uint8_t index2 = this->getB64Index(c2);
        uint8_t index3 = this->getB64Index(c3);
        uint8_t index4 = this->getB64Index(c4);
        
        
        uint8_t b1 = u8(index1<<2) | u8(index2>>4);
        uint8_t b2 = u8(index2<<4) | u8(index3>>2);
        uint8_t b3 = u8(index3<<6) | index4;
        
        decStr.push_back(b1);
        decStr.push_back(b2);
        decStr.push_back(b3);
    }
    
    if (hasPading) {
        char lastNextChar = strInput[strLen - 2]; // 倒数第二个字符
        if (lastNextChar == '=') {
            // 最后一组两个字符和两个==
            char c1 = strInput[countOfGroup*4];
            char c2 = strInput[countOfGroup*4+1];
            
            uint8_t index1 = this->getB64Index(c1);
            uint8_t index2 = this->getB64Index(c2);
            
            uint8_t b1 = u8(index1<<2) | u8(index2>>4);
            decStr.push_back(b1);
            
        } else {
            // 最后一组三个字母和一个==
            char c1 = strInput[countOfGroup*4];
            char c2 = strInput[countOfGroup*4+1];
            char c3 = strInput[countOfGroup*4+2];
            
            uint8_t index1 = this->getB64Index(c1);
            uint8_t index2 = this->getB64Index(c2);
            uint8_t index3 = this->getB64Index(c3);
            
            uint8_t b1 = u8(index1<<2) | u8(index2>>4);
            uint8_t b2 = u8(index2<<4) | u8(index3>>2);
            
            decStr.push_back(b1);
            decStr.push_back(b2);
        }
    }
    
    return decStr;
}
