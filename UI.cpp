#include "UI.hpp"
#include <iostream>
#include <string>
#if _WIN64 || _WIN32
    #define OS "Windows"
#elif __linux__
    #define __OS "Linux"
#endif


std::string UI::stringToLowerCase(const std::string& str) {
    std::string result;

    for(char i : str){
        result += tolower(i);
    }
    return result;
}

void UI::printCategories() {
    int i = 0;

    for(const auto& e: this -> categories) {
        std::cout << ++i << ". " << e << std::endl;
    }
}

std::vector<std::string> UI::getTimeStamp(const int &hours, const int &minutes, const int &seconds){
    std::vector<std::string> timeStamp;
    std::string hString, minString, secString;

    if(std::to_string(hours).length() < 2) {
        hString = "0" + std::to_string(hours);
    }
    else {
        hString = std::to_string(hours);
    }
    if(std::to_string(minutes).length() < 2) {
        minString = "0" + std::to_string(minutes);
    }
    else {
        minString = std::to_string(minutes);
    }
    if(std::to_string(seconds).length() < 2) {
        secString = "0" + std::to_string(seconds);
    }
    else {
        secString = std::to_string(seconds);
    }

    timeStamp.push_back(hString);
    timeStamp.push_back(minString);
    timeStamp.push_back(secString);

    return timeStamp;
}

int UI::getCommand() const {
    return this->command;
}

bool UI::getIncorrectPassword() const {
    return this -> incorrectPassword;
}

void UI::clearTerminal() {
    if(OS == "Windows") {
        system("cls");
    }else {
        system("clear");
    }

}


//------------------------------------------------------MAIN FUNCTIONS


void UI::chooseFile() {
    if(OS == "Linux") {
        std::filesystem::current_path(std::filesystem::current_path().parent_path().parent_path());
    }
    else {
        std::filesystem::current_path(std::filesystem::current_path().parent_path());
    }

    std::cout << "Would you like to [1] create a new file/use file outside directory or [2] open an existing file?" << std::endl;
    std::filesystem::create_directory("data");

    filePath = UserInput::filePath();
}

void UI::enterFile() {
    std::string dataString, dataStringDec, phraseToCheck, line;
    file = std::fstream(filePath, std::ios::out | std::ios::in);

    //getting current time
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    hours = ltm -> tm_hour;
    minutes = ltm -> tm_min;
    seconds = ltm -> tm_sec;

    if(!file.is_open()) {                                                                                               //if file does not exist create it and choose new master password
        file = std::fstream(filePath, std::ios::out);
        file.close();
        std::string passwordConfirmation;

        std::cout << "New file created!" << std::endl;

        do {
            std::cout << "Please enter password: ";
            UserInput::getUserInputString(newMasterPassword);
            masterPassword = newMasterPassword;
            std::cout << "Confirm Password: ";
            UserInput::getUserInputString(newMasterPassword);
            passwordConfirmation = newMasterPassword;
            if( passwordConfirmation != masterPassword ){
                std::cout << "Passwords are not the same!!!\n";
            }
        }while(passwordConfirmation != masterPassword);

        writeToFile();                                                                                                  //write checkPhrase and default categories into file in case of sudden termination

    }else{                                                                                                              //file exists

        std::string changedTimeStamp;
        std::vector<std::string> timeStampFormatted = getTimeStamp(hours, minutes, seconds);

        while(getline(file, line)) {
            dataString += line;
        }

        file.close();
        file = std::fstream(filePath, std::ios::out | std::ios::in);

        std::string sbstring = dataString.substr(checkPhrase.length() + 4, dataString.length());
        sbstring.pop_back();                                                                                            //deleting 2 last characters cause of Time stamp of seconds
        sbstring.pop_back();
        dataString = dataString.substr(2, checkPhrase.length()) + sbstring;
        changedTimeStamp = timeStampFormatted[0] + dataString.substr(0, checkPhrase.length()) + timeStampFormatted[1] + sbstring + timeStampFormatted[2];
        file << changedTimeStamp;

        std::cout << "Enter password: ";
        UserInput::getUserInputString(masterPassword);

        dataStringDec = EncryptDecrypt::decrypt(masterPassword, dataString);

        phraseToCheck = dataStringDec.substr(0, dataStringDec.find("||", 0)+2);
        while(phraseToCheck != checkPhrase) {
            std::cout << "Invalid password" << std::endl;
            std::cout << "Enter password: ";
            UserInput::getUserInputString(masterPassword);

            dataStringDec = EncryptDecrypt::decrypt(masterPassword, dataString);
            phraseToCheck = dataStringDec.substr(0, dataStringDec.find("||", 0)+2);

            incorrectPasswordCount++;
            if(incorrectPasswordCount > 5){
                std::cout << "Too many incorrect attempts!!!" << std::endl;
                incorrectPassword = true;
                break;
            }
        }

        if(!incorrectPassword) {
            std::string categoriesFileString = dataStringDec.substr(phraseToCheck.length(), dataStringDec.find("||", phraseToCheck.length()) - phraseToCheck.length() + 2);
            std::string fileEntriesData = dataStringDec.substr(categoriesFileString.length() + phraseToCheck.length(), dataStringDec.length());
            categories = Categories::stringToSet(categoriesFileString);

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

void UI::dataPrint() {
    std::cout<<"------------------------------------------------------------------------------------------------------------------------"<<std::endl;
    std::cout << "Data: " << std::endl;

    int dataIndex = 0;

    for(const auto& e: data) {
        std::cout << ++dataIndex << ".\n" << e << std::endl;
    }

    std::cout<<"------------------------------------------------------------------------------------------------------------------------"<<std::endl;
}

void UI::menu() {
    std::cout << "Menu: \n "
                 "[1] - Add new password;\n "
                 "[2] - Delete password;\n "
                 "[3] - Edit password;\n "
                 "[4] - Search password;\n "
                 "[5] - Sort passwords;\n "
                 "[6] - Add category;\n "
                 "[7] - Delete category;\n "
                 "[8] - Change File;\n "
                 "[9] - Quit"
              << std::endl;

    while(!(std::cin >> command) || command < 1 || command > 9) {                                                       //ask until not an int or not int <1 and >8
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid command!" << std::endl;
    }
}

void UI::addEntry() {
    char commandLocal = ' ';
    std::string commandLocalLine, confirmation = " ";

    while(commandLocal != 'n' && commandLocal!= 'N') {
        std::cout << "--ADD ENTRY\n" << std::endl;
        bool cancel = false;
        FileEntry fileEntry = UserInput::getFileEntry(data, categories, cancel);
        clearTerminal();
        if (!cancel) {
            std::cout << fileEntry;
            while (confirmation[0] != 'n' && confirmation[0] != 'N' && confirmation[0] != 'y' &&
                   confirmation[0] != 'Y') {
                std::cout << "Are you sure you want to add this entry? [y/n]: ";
                UserInput::getUserInputString(confirmation);
            }
            if (confirmation[0] == 'n' || confirmation[0] == 'N') {                                                          //break if user doesn't want to add entry
                clearTerminal();
                break;
            }
            this->data.push_back(fileEntry);
            while (commandLocal != 'n' && commandLocal != 'N' && commandLocal != 'y' && commandLocal != 'Y') {
                std::cout << "Do you want to add another entry? [y/n]: ";
                UserInput::getUserInputString(commandLocalLine);
                commandLocal = commandLocalLine[0];
            }
        }
    }
}

void UI::deleteEntry() {
    dataPrint();
    
    int rangeBegin, rangeEnd;
    std::string deleteEntriesConfirmation;
    if (!data.empty()) {
        std::cout << "--DELETE ENTRY\n" << std::endl;
        while ((std::cout << "Enter range of entries to delete ([begin] [end]): " &&
                !(std::cin >> rangeBegin >> rangeEnd)) || rangeBegin < 1 || rangeEnd > data.size()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input!!!" << std::endl;
        }

        while (std::cout << "Are u sure you want to delete entries from " << rangeBegin << " to " << rangeEnd
                         << std::endl &&
               !(std::cin >> deleteEntriesConfirmation) &&
               tolower(deleteEntriesConfirmation[0]) != 'y' &&
               tolower(deleteEntriesConfirmation[0]) != 'n') {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input!!!" << std::endl;
        }
        if (tolower(deleteEntriesConfirmation[0]) == 'y') {
            this->data.erase(data.begin() + rangeBegin - 1, data.begin() + rangeEnd);
        }
    }else{
        std::cout << "Please enter some data first" <<std::endl;
    }
}

void UI::editEntry() {
    dataPrint();

    int index, chosenCategory, localCommand;
    std::string newName, newPassword, newService, newLogin;
    
    if(!data.empty()) {
        int indexCategory = 0;

        std::cout << "--EDIT ENTRY\n" << std::endl;
        while (std::cout << "Enter index of entry to edit: " && !(std::cin >> index) || index < 1 ||
               index > data.size()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input!!!" << std::endl;
        }

        FileEntry &fileEntry = this->data[index - 1];

        clearTerminal();

        std::cout << fileEntry << std::endl;
        std::cout << "Edit: "
                     "[1]Name; "
                     "[2]Login; "
                     "[3]Password; "
                     "[4]Category; "
                     "[5]Service; "
                     "[6]Cancel" << std::endl;
        while (!(std::cin >> localCommand) || localCommand < 1 || localCommand > 9) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input!!!" << std::endl;
        }
        switch (localCommand) {
            case 1:
                std::cout << "Edit Name to: ";
                UserInput::getUserInputString(newName);
                fileEntry.setName(newName);
                break;
            case 2:
                std::cout << "Edit Login to: ";
                UserInput::getUserInputString(newLogin);
                fileEntry.setLogin(newLogin);
                break;
            case 3:
                std::cout << "Edit Password to: ";
                UserInput::getUserInputString(newPassword);
                fileEntry.setPassword(newPassword);
                break;
            case 4:
                std::cout << "Edit Category to one listed below: ";
                for (const auto &e: categories) {
                    std::cout << "[" << ++indexCategory << "] "
                              << e << " ";
                }
                std::cout << "[" << categories.size() + 1 << "] Create new category" << std::endl;
                while (true) {
                    while (!(std::cin >> chosenCategory)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input!!!" << std::endl;
                    }
                    if (chosenCategory > 0 && chosenCategory <= categories.size()) {
                        fileEntry.setCategory(*next(categories.begin(), chosenCategory - 1));
                        break;
                    } else if (chosenCategory == categories.size() + 1) {
                        addCategory();
                        fileEntry.setCategory(*categories.rbegin());
                        break;
                    } else {
                        std::cout << "Invalid input!!!" << std::endl;
                    }
                }
                break;
            case 5:
                std::cout << "Edit Service to: ";
                UserInput::getUserInputString(newService);
                fileEntry.setService(newService);
                break;
            default:
                break;
        }
    }else{
        std::cout << "Please enter some data first!!! " << std::endl;
    }
}

void UI::searchEntry() {
    std::string searchPhrase;

    if(!data.empty()) {
        std::cout << "--SEARCH ENTRY\n" << std::endl;
        std::cout << "Search phrase: ";
        UserInput::getUserInputString(searchPhrase);

        std::vector<FileEntry> foundEntries = FileEntry::searchFileEntries(data, searchPhrase);

        if (foundEntries.empty()) {
            std::cout << "No entry found!" << std::endl;
        } else {
            std::cout << "Found entries: " << std::endl;
            int index = 0;
            for (const auto &e: foundEntries) {
                std::cout << ++index << ". " << e << std::endl;
            }
        }
    }else{
        std::cout << "Please enter some data" << std::endl;
    }
}

void UI::sortEntries(){
    std::string sortParameter;
    std::vector<FileEntry> sortedEntries = data;
    int dataIndex = 0;
    
    if(!data.empty()) {
        std::cout << "--SORT ENTRIES\n" << std::endl;
        while (stringToLowerCase(sortParameter) != "name" && stringToLowerCase(sortParameter) != "category") {
            std::cout << "Sorting parameter: (Name/Category) ";
            UserInput::getUserInputString(sortParameter);
        }

        if (stringToLowerCase(sortParameter) == "name") {
            std::sort(sortedEntries.begin(), sortedEntries.end(), LessThanName());
        } else {
            std::sort(sortedEntries.begin(), sortedEntries.end(), LessThanCategory());
        }

        std::cout << "Sorted entries: " << std::endl;
        for (const auto &e: sortedEntries) {
            std::cout << ++dataIndex << ". " << e << std::endl;
        }
    }else{
        std::cout << "Please enter some data" << std::endl;
    }
}

void UI::addCategory() {
    std::string newCategory;

    std::cout << "--ADD CATEGORY\n" << std::endl;
    printCategories();
    std::cout << "Please enter category name: ";
    UserInput::getUserInputString(newCategory);
    categories.insert(newCategory);

}

void UI::deleteCategory() {
    printCategories();

    std::string categoryName, deleteCategoryConfirmation;

    std::cout << "--DELETE CATEGORY\n" << std::endl;
    std::cout << "Please enter category name: ";
    UserInput::getUserInputString(categoryName);
    while(find(categories.begin(), categories.end(), categoryName) == categories.end()) {
        std::cout << "Invalid input!!!" << std::endl;
        std::cout << "Please enter category name: ";
        UserInput::getUserInputString(categoryName);
    }

    while(categoryName == "None" || categoryName == "Internet" || categoryName == "Banking"){
        std::cout << "Can't delete default 3 categories!!!" << std::endl;
        std::cout << "Please enter category name: ";
        UserInput::getUserInputString(categoryName);
    }

    while(std::cout << "Are u sure you want to delete category and all entries that belong to it? Category Name:" << categoryName << std::endl &&
          !(std::cin >> deleteCategoryConfirmation) &&
          tolower(deleteCategoryConfirmation[0]) != 'y' &&
          tolower(deleteCategoryConfirmation[0]) != 'n'){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input!!!" << std::endl;
    }

    if (tolower(deleteCategoryConfirmation[0]) != 'y') {
        categories.erase(categoryName);
    }

    for(auto e: data) {
        if(e.getCategory() == categoryName) {
            data.erase(std::remove(data.begin(), data.end(), e), data.end());
        }
    }
   }

void UI::changeFile() {
    std::string currentSave;

    std::cout << "Do you want to save current file? [y/n]:";
    UserInput::getUserInputString(currentSave);
    while(tolower(currentSave[0]) != 'y' && tolower(currentSave[0]) != 'n'){
        std::cout << "Invalid Input!!!" << std::endl;
        UserInput::getUserInputString(currentSave);
    }
    if(tolower(currentSave[0]) == 'y'){
        writeToFile();
    }
    data.clear();
    masterPassword = "";
    categories = categoriesDefault;
    incorrectPasswordCount = 0;
    UI::clearTerminal();
    std::cout << "Would you like to [1] create a new file/use file outside directory or [2] open an existing file?" << std::endl;
    filePath = UserInput::filePath();
    UI::clearTerminal();
    enterFile();
}

void UI::writeToFile() {
    std::vector<std::string> timeStamp = getTimeStamp(hours, minutes, seconds);
    std::string hString = timeStamp[0], minString = timeStamp[1], secString = timeStamp[2];
    std::string toEncrypt = checkPhrase + Categories::vectorToString(categories);
    std::string encryptedFileReady, encrypted;
    auto fileRefresh = std::fstream(filePath, std::ios::out | std::ios::binary);

    for(auto e: data){
        toEncrypt += e.toStringToEncrypt();
    }

    encrypted = EncryptDecrypt::encrypt(masterPassword, toEncrypt);
    encryptedFileReady = hString;
    encryptedFileReady += encrypted.substr(0, checkPhrase.length());
    encryptedFileReady += minString;
    encryptedFileReady += encrypted.substr(checkPhrase.length());
    encryptedFileReady += secString;

    fileRefresh << encryptedFileReady;
}

bool UI::confirmation(const std::string &functionName) {
    std::string confirmationInput;

    std::cout << functionName << "\nContinue? [y/n]: ";
    UserInput::getUserInputString(confirmationInput);
    while (tolower(confirmationInput[0]) != 'y' && tolower(confirmationInput[0]) != 'n'){
        std::cout << "Invalid Input" << std::endl;
        std::cout << "Continue? [y/n]: ";
        UserInput::getUserInputString(confirmationInput);
    }

    clearTerminal();

    return tolower(confirmationInput[0]) == 'y';
}


