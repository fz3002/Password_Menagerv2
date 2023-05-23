#include "Password.hpp"
#include <iostream>
#include <random>

std::string password::generatePassword(int length, bool upperCase, bool specialSymbols){
   std::default_random_engine defaultRandomEngine;
    std::uniform_int_distribution<int> intDisto(0, 10000000);
    std::string password;
    bool specialUsed = false, upperCaseUsed = false;
    for(int i = 0; i < length; i++){
        if(upperCase && intDisto(defaultRandomEngine)%3 == 0){
            password += intDisto(defaultRandomEngine)%26 + 'A';
            upperCaseUsed = true;
        }else if(specialSymbols && intDisto(defaultRandomEngine)%7 == 0){
            password += intDisto(defaultRandomEngine)%15 + '!';
            specialUsed = true;
        }else{
            password += intDisto(defaultRandomEngine)%26 + 'a';
        }
    }
    int randomIndex = intDisto(defaultRandomEngine)%password.size();
    while(password[randomIndex] > 'z' || password[randomIndex] < 'a') randomIndex = intDisto(defaultRandomEngine)%password.size();
    if(!specialUsed && (upperCaseUsed || !upperCase) && specialSymbols) password[randomIndex] = intDisto(defaultRandomEngine)%15 + '!'; //if no special symbols but requested
    else if((specialUsed || !specialSymbols) && !upperCaseUsed && upperCase) password[randomIndex] = intDisto(defaultRandomEngine)%26 + 'A'; //if no upperCase but requested
    else if(!specialUsed && !upperCaseUsed && specialSymbols && upperCase) {
        int randomIndex2 = intDisto(defaultRandomEngine)%password.size();
        while (randomIndex2 ==  randomIndex){
            randomIndex2 = intDisto(defaultRandomEngine)%password.size();
        }
        password[randomIndex] = intDisto(defaultRandomEngine)%26 + 'A';
        password[randomIndex2] = intDisto(defaultRandomEngine)%15 + '!';
    }
    return password;
}

std::string password::passwordSafety(const std::string &password){
    int upperCaseLetters=0, lowerCaseLetters=0, specialSymbols=0, score;
    auto itPassword = password.begin();
    while(itPassword!= password.end()){
        if(*itPassword >= 'a' && *itPassword <= 'z'){
            upperCaseLetters++;
        }else if(*itPassword >= 'A' && *itPassword <= 'Z'){
            lowerCaseLetters++;
        }else{
            specialSymbols++;
        }
        itPassword++;
    }
    score = (lowerCaseLetters + (upperCaseLetters*2) + (specialSymbols*4))*password.length();
    std::cout << "score: " << score << std::endl;
    //*determines password safety level
    if(score < 100) return "very weak";
    else if(score < 200) return "weak";
    else if(score < 300) return "mid";
    else if(score < 400) return "strong";
    return "very strong";
}