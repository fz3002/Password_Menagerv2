#ifndef UI_HPP
#define UI_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <filesystem>
#include <sys/stat.h>
#include <ctime>
#include "EncryptDecrypt.hpp"
#include "Password.hpp"
#include "FileEntry.hpp"
#include "Categories.hpp"
#include "UserInput.hpp"

class UI{
    private:
    int incorectPasswordCount = 0, hours, minutes, seconds;
    int command;
    bool incorectPassword = false;
    std::string filePath, newMasterPassword, checkPhrase = "1701__CheckPasswordCheck__1701||", masterPassword;
    std::vector<std::string> categoriesDefault = {"None", "Internet", "Banking"}, categories = categoriesDefault;
    std::vector<FileEntry> data;
    std::fstream file;

    /**
     * @brief changes string to lower case
     * 
     * @param str 
     * @return std::string 
     */
    static std::string stringToLowerCase(const std::string& str);
    /**
     * @brief prints categories vector
     * 
     */
    void printCategories();
    /**
     * @brief creates vector of 3 formated timestamps used in file
     * 
     * @param hours 
     * @param minutes 
     * @param seconds 
     * @return std::vector<std::string> 
     */
    static std::vector<std::string> getTimeStamp(const int &hours, const int &minutes, const int &seconds);
    public:
    /**
     * @brief UI element prompting user to choose a file
     * 
     */
    void chooseFile();
    /**
     * @brief UI element that gets masterPassword and gets passes decrypted data to data vector
     * 
     */
    void enterFile();
    /**
     * @brief prints data vector to console
     * 
     */
    void dataPrint();
    /**
     * @brief UI menu element that recieves command from user
     * 
     */
    void menu();
    /**
     * @brief adds entry to data vector
     * 
     */
    void addEntry();
    /**
     * @brief delets one or more entry from data vector
     * 
     */
    void deleteEntry();
    /**
     * @brief lets user edit one entry
     * 
     */
    void editEntry();
    /**
     * @brief writes to console data consisting of pattern given by user
     * 
     */
    void searchEntry();
    /**
     * @brief writes to console data sorted using parameter given by user (name or category)
     * 
     */
    void sortEntries();
    /**
     * @brief lets user add category to category vector
     * 
     */
    void addCategory();
    /**
     * @brief user delets chosen category as well as every entry in it
     * 
     */
    void deleteCategory();
    /**
     * @brief function writes formated data to file
     * 
     */
    void writeToFile();
    /**
     * @brief Get the Command given in menu function
     * 
     * @return int
     */
    int getCommand() const;
    /**
     * @brief returns if masterPassword was correct or user used incorrect password to many times
     * 
     * @return true 
     * @return false 
     */
    bool getIncorrectPassword() const;

    static void clearTerminal();
};

#endif