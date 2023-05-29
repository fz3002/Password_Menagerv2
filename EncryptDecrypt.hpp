#ifndef ENCRYPTDECRYPT_HPP
#define ENCRYPTDECRYPT_HPP
#include <iostream>
#include <string>
#include <vector>
#include "FileEntry.hpp"

namespace EncryptDecrypt{
    /**
     * @brief splits string into vector of strings using given delimiter
     * 
     * @param str 
     * @param delimiter 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> split(std::string &str, const std::string& delimiter);
    /**
     * @brief 
     * 
     * @param password 
     * @param plainText 
     * @return std::string 
     */
    std::string encrypt(std::string password, std::string plainText);
    /**
     * @brief decrypts given string into plainText
     * 
     * @param password 
     * @param plainText
     * @return std::string 
     */
    std::string decrypt(std::string password, std::string encryptedText);
}
#endif