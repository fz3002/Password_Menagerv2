#ifndef CATEGORIES_HPP
#define CATEGORIES_HPP

#include <iostream>
#include <vector>

namespace Categories{
    /**
     * @brief returns string of categoires to encryption 
     * 
     * @param categories 
     * @return std::string 
     */
    std::string vectorToString(const std::vector<std::string>& categories);
    /**
     * @brief returns vector of cateogires from decrypted string
     * 
     * @param categories 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> stringToVector(std::string categories);
}

#endif