#ifndef CATEGORIES_HPP
#define CATEGORIES_HPP

#include <iostream>
#include <vector>
#include <set>

namespace Categories{
    /**
     * @brief returns string of categoires to encryption 
     * 
     * @param categories 
     * @return std::string 
     */
    std::string vectorToString(const std::set<std::string>& categories);
    /**
     * @brief returns vector of cateogires from decrypted string
     * 
     * @param categories 
     * @return std::set<std::string>
     */
    std::set<std::string> stringToSet(std::string categories);

}

#endif