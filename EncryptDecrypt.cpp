#include "EncryptDecrypt.hpp"
#include <string>
#include <vector>
#include <bits/stdc++.h>


std::vector<std::string> EncryptDecrypt::split(std::string &str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0, end;

    while(end = str.find(delimiter, start), end!= std::string::npos){
        token = str.substr(start, end-start);
        tokens.push_back(token);
        start = end + delimiter.length();
    }

    return tokens;
}

std::string EncryptDecrypt::encrypt(std::string password, std::string plainText) {
    std::string encrypted;
    auto passwordIndex = 0;
    char character;

    for(int i = 0; i < plainText.length();) {
        if(passwordIndex < password.length()) {
            character = char((plainText[i]^password[passwordIndex]))+33;
            encrypted += character;
            passwordIndex++;
            i++;
        }else {
            passwordIndex = 0;
        }
    }

    return encrypted;
}
    
std::string EncryptDecrypt::decrypt(std::string password, std::string encryptedText) {
    std::string decrypted;
    int passwordIndex = 0;

    for(int i = 0; i < encryptedText.length();) {
        if(passwordIndex < password.length()) {
            decrypted += char(((encryptedText[i])-33)^password[passwordIndex]);
            passwordIndex++;
            i++;
        }else {
            passwordIndex = 0;
        }
    }

    return decrypted;
}


