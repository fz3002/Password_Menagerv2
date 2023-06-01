#include "Password.hpp"
#include <iostream>
#include <random>

std::string password::generatePassword(int length, bool upperCase, bool specialSymbols){
    std::string upperCaseLetters = "QWERTYUIOPASDFGHJKLZXCVBNM";
    std::string lowerCaseLetters = "qwertyuiopasdfghjklzxcvbnm";
    std::string specialCharacters = "!@#$%^&*()_-+=~`{[}]:<,>.?";
    std::string digits = "0123456789";
    std::random_device dev;
    std::mt19937 rng(dev());
    std::string password;
    std::uniform_int_distribution<std::mt19937::result_type> intDisto(1, 4),
                                        intSpecialDisto(2, 4),
                                        intUpperDisto(1, 3),
                                        randomIndexDist(1, 2),
                                        specialDist (0,specialCharacters.length()),
                                        upperCaseDist (0,upperCaseLetters.length()),
                                        lowerCaseDist (0,lowerCaseLetters.length()),
                                        digitsDist (0,digits.length());

    for(int i = 0; i < length; i++) {//randomize upperCase lowerCase or Special
        unsigned int random;

        if (upperCase && !specialSymbols) {
            random = intUpperDisto(rng);
        }else if (!upperCase && specialSymbols) {
            random = intSpecialDisto(rng);
        }else if (!upperCase) {
            random = 2;
        }else{
            random = intDisto(rng);
        }
        //choosing upperCase letters or lowerCase or Special Chars
        if (random == 1) {
            password += upperCaseLetters[upperCaseDist(rng)];
        }else if(random == 2) {
            password += lowerCaseLetters[lowerCaseDist(rng)];
        }else if(random == 4) {
            password += specialCharacters[specialDist(rng)];
        }else{
            password += digits[digitsDist(rng)];
        }
    }

    return password;
}

std::string password::passwordSafety(const std::string &password) {
    int upperCaseLetters=0, lowerCaseLetters=0, specialSymbols=0, score;
    auto itPassword = password.begin();

    while(itPassword!= password.end()) {
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
    std::cout << "score: " << score << std::endl;                                                                       //determines password safety level
    if(score < 100) return "very weak";
    else if(score < 200) return "weak";
    else if(score < 300) return "mid";
    else if(score < 400) return "strong";
    return "very strong";
}