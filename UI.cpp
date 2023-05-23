#include "UI.hpp"
#include <iostream>
#include <string>
#if _WIN64 || _WIN32
    #define OS "Windows"
#elif __linux__
    #define __OS "Linux"
#endif


std::string UI::stringToLowerCase(const std::string& str){
    std::string result;
    for(char i : str){
        result += tolower(i);
    }
    return result;
}

void UI::printCategories(){
    int i = 0;
    for(const auto& e: this -> categories) {
        std::cout << ++i << ". " << e << std::endl;
    }
}

std::vector<std::string> UI::getTimeStamp(const int &hours, const int &minutes, const int &seconds){
    std::vector<std::string> timeStamp;
    std::string hString, minString, secString;
    if(std::to_string(hours).length() < 2){ 
        hString = "0" + std::to_string(hours);
    }
    else{
        hString = std::to_string(hours);
    }
    if(std::to_string(minutes).length() < 2){
        minString = "0" + std::to_string(minutes);
    }
    else{
        minString = std::to_string(minutes);
    }
    if(std::to_string(seconds).length() < 2){
        secString = "0" + std::to_string(seconds);
    }
    else{
        secString = std::to_string(seconds);
    }
    timeStamp.push_back(hString);
    timeStamp.push_back(minString);
    timeStamp.push_back(secString);
    return timeStamp;
}


void UI::chooseFile(){
    if(OS == "Linux") std::filesystem::current_path(std::filesystem::current_path().parent_path().parent_path()); //TODO: check file path on linux
    else std::filesystem::current_path(std::filesystem::current_path().parent_path());
    std::cout << "Would you like to [1] create a new file/use file outside directory or [2] open an existing file?" << std::endl;
    std::filesystem::create_directory("data");
    filePath = UserInput::filePath();
}

void UI::enterFile(){
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    std::string dataString, dataStringDec, phraseToCheck, line;
    file = std::fstream(filePath, std::ios::out | std::ios::in);
    hours = ltm -> tm_hour;
    minutes = ltm -> tm_min;
    seconds = ltm -> tm_sec;

    if(!file.is_open()) { //if file does not exist create it and choose new master password
        file = std::fstream(filePath, std::ios::out);
        file.close();
        std::cout << "New file created!" << std::endl;
        std::cout << "Please enter password: ";
        ws(std::cin);
        getline(std::cin, newMasterPassword);
        masterPassword = newMasterPassword;
        writeToFile(); //wrtie checkPhrase and default categories into file in case of sudden termination
    }else{ 
        //file exists
        std::string changedTimeStamp;
        std::vector<std::string> timeStampFormated = getTimeStamp(hours, minutes, seconds);

        while(getline(file, line)) {
            dataString += line;
        }
        file.close();
        file = std::fstream(filePath, std::ios::out | std::ios::in);
        std::string sbstring = dataString.substr(checkPhrase.length() + 4, dataString.length());
        sbstring.pop_back(); //deleting 2 last characters cause of Time stamp of seconds
        sbstring.pop_back();
        dataString = dataString.substr(2, checkPhrase.length()) + sbstring;
        changedTimeStamp = timeStampFormated[0] + dataString.substr(0, checkPhrase.length()) + timeStampFormated[1] + sbstring + timeStampFormated[2];
        file << changedTimeStamp;
        std::cout << "Enter password: ";
        ws(std::cin);
        getline(std::cin, masterPassword);
        dataStringDec = EncryptDecrypt::decrypt(masterPassword, dataString);
        phraseToCheck = dataStringDec.substr(0, dataStringDec.find("||", 0)+2);
        while(phraseToCheck != checkPhrase){
            std::cout << "Invalid password" << std::endl;
            std::cout << "Enter password: ";
            ws(std::cin);
            getline(std::cin, masterPassword);
            incorectPasswordCount++;
            if(incorectPasswordCount > 5){
                std::cout << "Too many incorrect attempts!!!" << std::endl;
                incorectPassword = true;
                break;
            }
        }
        if(!incorectPassword){
            std::string categoriesFileString = dataStringDec.substr(phraseToCheck.length(), dataStringDec.find("||", phraseToCheck.length()) - phraseToCheck.length() + 2);
            std::string fileEntriesData = dataStringDec.substr(categoriesFileString.length() + phraseToCheck.length(), dataStringDec.length());
            categories = Categories::stringToVector(categoriesFileString);
            if(fileEntriesData.length() > 0){
                std::vector<std::string> fileEntriesStrings = EncryptDecrypt::split(fileEntriesData, "||");
                for(auto fileEntry : fileEntriesStrings){
                    std::vector<std::string> fileEntryMembers = EncryptDecrypt::split(fileEntry, ";;");
                    FileEntry readFileEntry = FileEntry(fileEntryMembers[0], fileEntryMembers[1]);
                    readFileEntry.setCategory(fileEntryMembers[2]);
                    if(fileEntryMembers[3].length() > 0){
                        readFileEntry.setLogin(fileEntryMembers[3]);
                    }
                    if(fileEntryMembers[4].length() > 0){
                        readFileEntry.setService(fileEntryMembers[4]);
                    }
                    data.push_back(readFileEntry);
                }
            }
        }
    }
    file.close();
}

void UI::dataPrint(){
    std::cout << "Data: " << std::endl;
    int dataIndex = 0;
    for(const auto& e: data) {
        std::cout << ++dataIndex << ". " << e << std::endl;
    }
}

void UI::menu(){
    std::cout << "Menu: \n"
                 "[1] - Add new password; "
                 "[2] - Delete password; "
                 "[3] - Edit password; "
                 "[4] - Search password; "
                 "[5] - Sort passwords; "
                 "[6] - Add category; "
                 "[7] - Delete category; "
                 "[8] - Quit"
              << std::endl;
    while(!(std::cin >> command) || command < 1 || command > 8) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid command!" << std::endl;
    }
}

void UI::addEntry(){
    char commandLocal = ' ';
    std::string commandLocalLine;
    if(OS == "Windows") system("cls");
    else system("clear");
    while(commandLocal != 'n' && commandLocal!= 'N') {
        FileEntry fileEntry = UserInput::getFileEntry(data, categories);
        this -> data.push_back(fileEntry);
        std::cout << "Do you want to add another entry? (Y/N): ";
        std::cin >> commandLocalLine;
        commandLocal = commandLocalLine[0];
    }
}

void UI::deleteEntry(){
    dataPrint();
    int rangeBegin, rangeEnd;
    while(std::cout << "Enter range of entries to delete (2 Numbers): " && !(std::cin >> rangeBegin >> rangeEnd)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input!!!" << std::endl;
    }
    this -> data.erase(data.begin() + rangeBegin - 1, data.begin() + rangeEnd);
}

void UI::editEntry(){
    dataPrint();
    int index, categoryIndex;
    std::string newName, newPassword, newService, newLogin;
    while (std::cout << "Enter index of entry to edit: " &&!(std::cin >> index)){
        std::cout << "Invalid input!!!" << std::endl;
    }
    int localCommand;
    FileEntry &fileEntry = this -> data[index - 1];
    system("cls");
    std::cout << fileEntry << std::endl;
    std::cout << "Edit: "
                 "[1]Name; "
                 "[2]Login; "
                 "[3]Password; "
                 "[4]Category; "
                 "[5]Service; " << std::endl;
    std::cin >> localCommand;
    switch(localCommand){
        case 1: std::cout << "Edit Name to: ";
                ws(std::cin);
                getline(std::cin, newName);
                //fileEntry.setName(newName);
                fileEntry.setName(newName);
                break;
        case 2: std::cout << "Edit Login to: ";
                ws(std::cin);
                getline(std::cin, newLogin);
                fileEntry.setLogin(newLogin);
                break;
        case 3: std::cout << "Edit Password to: ";
                ws(std::cin);
                getline(std::cin, newPassword);
                fileEntry.setPassword(newPassword);
                break;
        case 4: std::cout << "Edit Category to one listed below: ";
                for(const auto& e: categories) {
                std::cout << "[" <<find(categories.begin(), categories.end(), e) - categories.begin() + 1 << "] " << e << " ";
                }
                std::cout << "[" <<categories.size() + 1 << "] Create new category"<< std::endl;
                while(true){
                    std::cin >> categoryIndex;
                    if(categoryIndex > 0 && categoryIndex <= categories.size()){
                        fileEntry.setCategory(categories[categoryIndex-1]);
                        break;
                    }else if(categoryIndex == categories.size() + 1){
                        addCategory();
                        fileEntry.setCategory(categories[categories.size() - 1]);
                        break;
                    }else{
                        std::cout << "Invalid input!!!" << std::endl;
                    }
                }
                break;
        case 5: std::cout << "Edit Service to: ";
                ws(std::cin);
                getline(std::cin, newService);
                fileEntry.setService(newService);
                break;
        default: std::cout << "Invalid input!!!";
                break;
    }
}

void UI::searchEntry(){
    std::string searchPhrase;
    std::cout << "Search phrase: ";
    ws(std::cin);
    getline(std::cin, searchPhrase);
    std::vector<FileEntry> foundEntries = FileEntry::searchFileEntries(data, searchPhrase);
    if(foundEntries.empty()){
         std::cout << "No entry found!" << std::endl;
    }else{
        int index = 0;
        for(const auto& e: foundEntries) {
            std::cout << ++index << ". " << e << std::endl;
        }
    }
}

void UI::sortEntries(){
    std::string sortParameter;
    std::vector<FileEntry> sortedEntries = data;
    while(stringToLowerCase(sortParameter) != "name" && stringToLowerCase(sortParameter) != "category") {
        std::cout << "Sorting parameter: (Name/Category) ";
        std::cin >> sortParameter;
    }
    if(stringToLowerCase(sortParameter) == "name") {
        std::sort(sortedEntries.begin(), sortedEntries.end(), LessThanName());
    }else{
        std::sort(sortedEntries.begin(), sortedEntries.end(), LessThanCategory());
    }
    std::cout << "Sorted entries: " << std::endl;
    int dataIndex = 0;
    for(const auto& e: sortedEntries){
        std::cout << ++dataIndex << ". " << e << std::endl;
    }
}

void UI::addCategory(){
    printCategories();
    std::string newCategory;
    std::cout << "Please enter category name: ";
    ws(std::cin);
    getline(std::cin, newCategory);
    categories.push_back(newCategory);
}

void UI::deleteCategory(){
    printCategories();
    std::string categoryName;
    std::cout << "Please enter category name: ";
    ws(std::cin);
    getline(std::cin, categoryName);
    while(find(categories.begin(), categories.end(), categoryName) == categories.end()){
        std::cout << "Invalid input!!!" << std::endl;
        ws(std::cin);
        getline(std::cin, categoryName);
    }
    categories.erase(std::remove(categories.begin(), categories.end(), categoryName), categories.end());
    for(auto e: data) {
        if(e.getCategory() == categoryName) {
            data.erase(std::remove(data.begin(), data.end(), e), data.end());
        }
    }
}

void UI::writeToFile(){
    std::vector<std::string> timeStamp = getTimeStamp(hours, minutes, seconds);
    std::string hString = timeStamp[0], minString = timeStamp[1], secString = timeStamp[2];
    auto fileRefresh = std::fstream(filePath, std::ios::out | std::ios::binary);
    std::string toEncrypt = checkPhrase + Categories::vectorToString(categories);
    for(auto e: data){
        toEncrypt += e.toStringToEncrypt();
    }
    std::string encrypted = EncryptDecrypt::encrypt(masterPassword, toEncrypt);
    encrypted = hString + encrypted.substr(0, checkPhrase.length()) + minString + encrypted.substr(checkPhrase.length(), encrypted.length()) + secString;
    fileRefresh << encrypted;
}

int UI::getCommand() const{
    return this->command;
}

bool UI::getIncorectPassword() const{
    return this -> incorectPassword;
}

