#include "Password.hpp"
#include <iostream>
#include <random>

std::string password::generatePassword(int length, bool upperCase, bool specialSymbols){
    std::string upperCaseLetters = "QWERTYUIOPASDFGHJKLZXCVBNM";
    std::string lowerCaseLetters = "qwertyuiopasdfghjklzxcvbnm";
    std::string specialCharacters = "!@#$%^&*()_-+=~`{[}]:<,>.?";
    std::default_random_engine defaultRandomEngine;

    std::string password;
    bool specialUsed = false, upperCaseUsed = false;
    std::uniform_int_distribution<int> intDisto(1, 3),
                                        intSpecialDisto(2, 3),
                                        intUpperDisto(1, 2),
                                        randomIndexDist(1, 2),
                                        specialDist (0,specialCharacters.length()),
                                        upperCaseDist (0,upperCaseLetters.length()),
                                        lowerCaseDist (0,lowerCaseLetters.length());

    for(int i = 0; i < length; i++){
        //randomize upperCase lowerCase or Special
        int random = intDisto(defaultRandomEngine);
        if (upperCase && !specialSymbols){
            random = intSpecialDisto(defaultRandomEngine);
        }else if (!upperCase && specialSymbols){
            random = intSpecialDisto(defaultRandomEngine);
        }
        else if (!upperCase && !specialSymbols){
            random = 2;
        }
        //choosing upperCase letters or lowerCase or Special Chars
        if (random == 1){
            password += upperCaseLetters[upperCaseDist(defaultRandomEngine)];
            upperCaseUsed = true;
        }else if(random == 2){
            password += lowerCaseLetters[lowerCaseDist(defaultRandomEngine)];
        }else if(random == 3){
            password += specialCharacters[specialDist(defaultRandomEngine)];
            specialUsed = true;
        }
    }

    //choosing random index that is lowerCase

    int randomIndex = randomIndexDist(defaultRandomEngine)%password.size();
    while(password[randomIndex] > 'z' || password[randomIndex] < 'a') {
        randomIndex = randomIndexDist(defaultRandomEngine)%password.size();
    }

    //Swapping to meet requirements

    if(!specialUsed && (upperCaseUsed || !upperCase) && specialSymbols){
         //if no special symbols but requested
        password += specialCharacters[specialDist(defaultRandomEngine)];
    }
    else if((specialUsed || !specialSymbols) && !upperCaseUsed && upperCase){
        //if no upperCase but requested
        password += upperCaseLetters[upperCaseDist(defaultRandomEngine)];
    }
    else if(!specialUsed && !upperCaseUsed && specialSymbols && upperCase) { //neither symbol present both requested
        int randomIndex2 = randomIndexDist(defaultRandomEngine)%password.size();
        while (randomIndex2 ==  randomIndex){
            randomIndex2 = randomIndexDist(defaultRandomEngine)%password.size();
        }
        password[randomIndex] = specialCharacters[specialDist(defaultRandomEngine)];
        password[randomIndex2] = upperCaseLetters[upperCaseDist(defaultRandomEngine)];
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