#ifndef PASSWORD_HPP
#define PASSWORD_HPP

#include <iostream>

namespace password{
    /**
     * @brief function to generate password with given length and asspects chosen by user passed in parameters
     * 
     * @param length 
     * @param upperCase 
     * @param lowerCase 
     * @param specialSymbols 
     * @return std::string 
     */
    std::string generatePassword(int length, bool upperCase, bool specialSymbols);
    /**
     * @brief funcrion checsk how strong a password passed as parameter is
     * 
     * @param password 
     * @return std::string 
     */
    std::string passwordSafety(const std::string &password); //TODO: change scoring scheme
}

#endif