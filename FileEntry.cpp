#include "FileEntry.hpp"
#include <iostream>
#include <utility>


std::string FileEntry::toStringToEncrypt() {
        return this -> name + getDelimiter() + this -> password + getDelimiter() + this -> category + getDelimiter() + this -> service + getDelimiter() + this -> login + getDelimiter() +"||";
}

std::vector<FileEntry> FileEntry::searchFileEntries(const std::vector<FileEntry>& data, const std::string& pattern) {
    std::vector<FileEntry> result;

    for(const auto& e: data) {
        if(e.name.find(pattern)!= std::string::npos || e.service.find(pattern)!= std::string::npos || e.login.find(pattern)!= std::string::npos || e.category.find(pattern)!= std::string::npos) {
            result.push_back(e);
        }
    }

    return result;
}

void FileEntry::setName(std::string newName) {
    this->name = std::move(newName);
}

void FileEntry::setPassword(std::string newPassword) {
    this->password = std::move(newPassword);
}

void FileEntry::setService(const std::string& newService) {
    this->service = newService;
}

void FileEntry::setLogin(const std::string& newLogin) {
    this->login = newLogin;
}

void FileEntry::setCategory(std::string newCategory) {
    this->category = std::move(newCategory);
}

std::string FileEntry::getCategory() {
    return this->category;
}

std::string FileEntry::getPassword() {
    return this->password;
}

std::string FileEntry::getDelimiter() {
    return ";;";
}

std::ostream &operator<<(std::ostream &out, const FileEntry &fileEntry) {
    return out << "name: " << fileEntry.name << "; login: " << fileEntry.login << "; password: " << fileEntry.password <<  "; category: " << fileEntry.category << "; service: " << fileEntry.service;
}

bool operator==(const FileEntry &fileEntry1, const FileEntry &fileEntry2) {
    return fileEntry1.name == fileEntry2.name && fileEntry1.category == fileEntry2.category && fileEntry1.login == fileEntry2.login && fileEntry1.password == fileEntry2.password && fileEntry1.service == fileEntry2.service;
}
