#include <unordered_set>
#include <set>
#include "EncryptDecrypt.hpp"
#include "Categories.hpp"
#include "FileEntry.hpp"

std::string Categories::vectorToString(const std::set<std::string>& categories) {
    std::string result;

    for(const auto& e: categories){
        result += e;
        result += FileEntry::getDelimiter();
    }

    return result + "||";
}

std::set<std::string> Categories::stringToSet(std::string categories) {
    categories.pop_back();
    categories.pop_back();
    std::set<std::string> setSol;

    std::vector<std::string> categoriesVec =  EncryptDecrypt::split(categories, FileEntry::getDelimiter());
    for(const auto& el : categoriesVec){
        setSol.insert(el);
    }

    return setSol;
}
