#ifndef USERINPUT_HPP
#define USERINPUT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <filesystem>
#include <sys/stat.h>
#include "FileEntry.hpp"
#include "Password.hpp"


namespace UserInput{
    /**
     * @brief determines if password is used
     * @param password
     * @param data
     * @return true
     * @return false
     */
    bool isPasswordUsed(const std::string& password, const std::vector<FileEntry> &data);
    /**
     * @brief determines path to work file asking user
     * 
     * @return std::string 
     */
    std::string filePath();
    /**
     * @brief fetches fileEntry with values given by user
     * @param data
     * @param categories
     * @return FileEntry
     */
    FileEntry getFileEntry(const std::vector<FileEntry> &data, std::vector<std::string> &categories);
}

#endif