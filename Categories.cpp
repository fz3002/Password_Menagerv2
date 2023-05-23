#include "EncryptDecrypt.hpp"
#include "Categories.hpp"
#include "FileEntry.hpp"

std::string Categories::vectorToString(const std::vector<std::string>& categories){
    std::string result;
    for(const auto& e: categories){
        result += e;
        result += FileEntry::getDelimiter();
    }
    return result + "||";
}

std::vector<std::string> Categories::stringToVector(std::string categories){
    categories.pop_back();
    categories.pop_back();
    return EncryptDecrypt::split(categories, FileEntry::getDelimiter());
}
