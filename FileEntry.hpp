#ifndef FILEENTRY_HPP
#define FILEENTRY_HPP

#include <string>
#include <utility>
#include <vector>
#include <iostream>

class FileEntry{
    private:
    std::string name, category, password, service = "-", login = "-";
    public:
    /**
     * @brief Construct a new File Entry object
     * 
     * @param name 
     * @param password 
     * @param category 
     */
    FileEntry(std::string name, std::string password): name(std::move(name)), password(std::move(password)){}
    /**
     * @brief way of writing to file
     * 
     * @param out 
     * @param fileEntry 
     * @return std::ostream& 
     */
    std::string toStringToEncrypt();
    /**
     * @brief search function
     * 
     * @param data
     * @param pattern
     * @return std::vector<FileEntry> 
     */
    static std::vector<FileEntry> searchFileEntries(const std::vector<FileEntry>& data, const std::string& pattern);
    /**
     * @brief Set entry's name
     * 
     * @param name 
     */
    void setName(std::string name);
    /**
     * @brief Set entry's password
     * 
     * @param password 
     */
    void setPassword(std::string password);
    /**
     * @brief Set entry's serivce
     * 
     * @param service 
     */
    void setService(const std::string& service);
    /**
     * @brief Set login of entry
     * 
     * @param login 
     */
    void setLogin(const std::string& login);
    /**
     * @brief Set category of entry
     * 
     * @param category 
     */
    void setCategory(std::string category);
    /**
     * @brief Get the Category of entry
     * 
     * @return std::string 
     */
    std::string getCategory();
    /**
     * @brief Get the Password of entry
     * 
     * @return std::string 
     */
    std::string getPassword();

    /**
     * @brief Get the Delimiter used in file formating
     * 
     * @return std::string 
     */
    static std::string getDelimiter();
    /**
     * @brief overrides operator << to make it easier to write to the console
     * 
     * @param out 
     * @param fileEntry 
     * @return std::ostream& 
     */
    friend std::ostream &operator<<(std::ostream& out, const FileEntry &fileEntry);
    /**
     * @brief Used in find std::remove function
     * 
     * @param fileEntry1 
     * @param fileEntry2 
     * @return true 
     * @return false 
     */
    friend bool operator==(const FileEntry &fileEntry1, const FileEntry &fileEntry2);
    /**
     * @brief struct consisting of function - comparator used to compare FileEntry objects using name
     * 
     */
    friend struct LessThanName;
    /**
     * @brief struct consisting of function - comparator used to compare FileEntry objects using category
     * 
     */
    friend struct LessThanCategory;
};

struct LessThanName{
    inline bool operator()(const FileEntry &fileEntry1, const FileEntry &fileEntry2){
        return (fileEntry1.name.compare(fileEntry2.name)) < 0;
    }
};

struct LessThanCategory{
    inline bool operator()(const FileEntry &fileEntry1, const FileEntry &fileEntry2){
        return (fileEntry1.category.compare(fileEntry2.category)) < 0;
    }
};  

#endif