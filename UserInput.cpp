#if _WIN64 || _WIN32
#define OS "Windows"
#elif __linux__
#define __OS "Linux"
#endif
#include "UserInput.hpp"

bool UserInput::isPasswordUsed(const std::string& password, const std::vector<FileEntry> &data){
    for(auto e: data){
        if(e.getPassword() == password){
            return true;
        }
    }
    return false;
}

std::string UserInput::filePath(){
    std::string filePath, userInput;
    int newOrExistingFile = 0;

    if(OS == "Linux") std::filesystem::current_path(std::filesystem::current_path().parent_path().parent_path());

    while (newOrExistingFile != 1 && newOrExistingFile != 2) {
        getUserInputString(userInput);
        newOrExistingFile = int(userInput[0]) - '0';
        if(newOrExistingFile == 1){                                                                                     //creating new file
            std::cout << "Please enter absolute path to the file: " << std::endl;
            getUserInputString(filePath);
            correctFilePath(filePath);
        }else if(newOrExistingFile == 2) {                                                                              //choose file from data directory
            char newFileCreation;
            std::string userInputNewFile;
            int chooseDir;
            while(true) {
                int index = 0;
                std::cout
                        << "------------------------------------------------------------------------------------------------------------------------"
                        << std::endl;
                for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(
                        std::filesystem::canonical("data"))) { // print out files in data directory
                    std::cout << ++index << ". " << std::filesystem::canonical(dirEntry.path()) << std::endl;
                }
                std::cout
                        << "------------------------------------------------------------------------------------------------------------------------"
                        << std::endl;
                if (index == 0) {                                                                                       //there are no file in data directory
                    std::cout << "No files in directory!" << std::endl;
                    std::cout << "Please enter absolute path to the file: " << std::endl;
                    getUserInputString(filePath);
                    correctFilePath(filePath);
                } else {
                    while (std::cout << "Choose file number from the ones listed above: " && !(std::cin >> chooseDir) ||
                           chooseDir < 1 || chooseDir > index) {                                                        //choose from given files TODO:check input error catch
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input!" << std::endl;
                    }
                    int i = 1;
                    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(
                            std::filesystem::canonical("data"))) {
                        if (i++ == chooseDir) {
                            filePath = std::filesystem::canonical(dirEntry.path()).string();
                            break;
                        }
                    }
                    if (!std::filesystem::exists(std::filesystem::path(filePath)) || std::filesystem::is_directory(std::filesystem::path(filePath))) {                        //chosen number out of range or chosen number leads to a directory
                        std::cout << "File does not exist!!!" << std::endl;
                        while (newFileCreation != 'y' && newFileCreation != 'Y' && newFileCreation != 'n' &&
                               newFileCreation != 'N') {
                            std::cout << "Would you like to create new file? [y/n]:" << std::endl;
                            getUserInputString(userInputNewFile);
                            newFileCreation = userInputNewFile[0];
                            if (newFileCreation != 'y' && newFileCreation != 'Y' && newFileCreation != 'n' &&
                                newFileCreation != 'N') {
                                std::cout << "Invalid input!!!" << std::endl;
                            }
                        }
                        if (newFileCreation == 'y' || newFileCreation == 'Y') {                                                            //if user wants to create new file he has to enter the absolute path to the file if not loops goes to the beginning of the loop of choosing existing files
                            std::cout << "Please enter absolute path to the file: " << std::endl;
                            getUserInputString(filePath);
                            correctFilePath(filePath);
                            break;
                        }
                    } else break;                                                                                       //if file exists and is not a directory loop ends
                }
            }
         }else{
            std::cout << "Invalid input!" << std::endl;
        }
    }
    return filePath;
}

FileEntry UserInput::getFileEntry(const std::vector<FileEntry> &data, std::set<std::string> &categories, bool &cancel) {
    std::string ifLogin, ifService;
    std::string name, entryPassword, service, login, commandLocalLine;
    int chosenCategory, commandLocalPassword, categoryIndex = 0;

    std::cout << "Enter Data" << std::endl;
    std::cout << "Name: ";
    getUserInputString(name);
    if(name[0] == 27) std::cout << "Test" << std::endl;
    std::cout << "[1]Use your own password; "
                 "[2]Generate password;"
                 "[3]Cancel " << std::endl;
    while( !(std::cin >> commandLocalPassword) || commandLocalPassword < 1){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input!" << std::endl;
    }
    if(commandLocalPassword == 1) {                                                                                     //Own Password
        char commandLocalOwnPassword = ' ';
        std::string commandLocalOwnPasswordInput;

        while(commandLocalOwnPassword != 'y' && commandLocalOwnPassword!= 'Y') {
            commandLocalOwnPassword = ' ';
            std::cout << "Password: ";
            getUserInputString(entryPassword);
            if(isPasswordUsed(entryPassword, data)) {
                std::cout << "Password already used" << std::endl;
                continue;
            }
            std::cout << password::passwordSafety(entryPassword) << std::endl;
            while(commandLocalOwnPassword!= 'n' && commandLocalOwnPassword!= 'N' && commandLocalOwnPassword != 'y' && commandLocalOwnPassword!= 'Y') {
                std::cout << "Would you like to use your this password? [y/n]:";
                getUserInputString(commandLocalOwnPasswordInput);
                commandLocalOwnPassword = commandLocalOwnPasswordInput[0];
            }
        }
    }else if (commandLocalPassword == 2) { //Generate Password
        int gPasswordLength;
        std::string choiceUpperCase = " ", choiceSpecialSymbols =" ";
        bool gUpperCase, gSpecialSymbols;
        std::cout << "Password length: ";
        while( !(std::cin >> gPasswordLength) || gPasswordLength < 1){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input!" << std::endl;
        }
        while(choiceUpperCase[0] != 'n' && choiceUpperCase[0] != 'N' && choiceUpperCase[0] != 'y' && choiceUpperCase[0] != 'Y'){
            std::cout << "Would you like to use uppercase letters? [y/n]:";
            getUserInputString(choiceUpperCase);
        }
        while(choiceSpecialSymbols[0] != 'n' && choiceSpecialSymbols[0] != 'N' && choiceSpecialSymbols[0] != 'y' && choiceSpecialSymbols[0] != 'Y'){
            std::cout << "Would you like to use special symbols? [y/n]:";
            getUserInputString(choiceSpecialSymbols);
        }
        gUpperCase = choiceUpperCase[0] == 'y' || choiceUpperCase[0] == 'Y';
        gSpecialSymbols = choiceSpecialSymbols[0] == 'y' || choiceSpecialSymbols[0] == 'Y';
        entryPassword = password::generatePassword(gPasswordLength, gUpperCase, gSpecialSymbols);

        std::cout << "Password generated: " << entryPassword << std::endl;
    }else{
        cancel = true;
    }
    FileEntry fileEntry(name, entryPassword);
    if(!cancel) {
        std::cout << "Category: ";
        for (const auto &e: categories) {
            std::cout << "[" << ++categoryIndex << "] " << e
                      << " ";
        }
        std::cout << "[" << categories.size() + 1 << "] Create new category" << std::endl;
        std::string newCategory;
        while (true) {
            while (!(std::cin >> chosenCategory)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input!" << std::endl;
            }
            if (chosenCategory > 0 && chosenCategory <= categories.size()) {
                fileEntry.setCategory(*next(categories.begin(), chosenCategory-1));
                break;
            } else if (chosenCategory == categories.size() + 1) {
                int index = 0;
                for (const auto &e: categories) {
                    std::cout << ++index << ". " << e << std::endl;
                }
                std::cout << "Please enter category name: ";
                getUserInputString(newCategory);
                categories.insert(newCategory);
                fileEntry.setCategory(*categories.rbegin());
                break;
            } else {
                std::cout << "Invalid input!!!" << std::endl;
            }
        }
        std::cout << "Do you want to add service? [y/n]: ";
        do {
            getUserInputString(ifService);
        } while (toupper(ifService[0]) != 'Y' && toupper(ifService[0]) != 'N');

        if (toupper(ifService[0]) == 'Y') {
            std::cout << "Service: ";
            getUserInputString(service);
            fileEntry.setService(service);
        }
        std::cout << "Do you want to add login? [y/n]: ";
        do {
            getUserInputString(ifLogin);
        } while (toupper(ifLogin[0]) != 'Y' && toupper(ifLogin[0]) != 'N');
        if (toupper(ifLogin[0]) == 'Y') {
            std::cout << "Login: ";
            getUserInputString(login);
            fileEntry.setLogin(login);
        }
    }
    return fileEntry;
}

void UserInput::getUserInputString(std::string &arg) {
    ws(std::cin);
    getline(std::cin, arg);
}

void UserInput::correctFilePath(std::string &filePath) {
    while(!std::filesystem::exists(std::filesystem::path(filePath).parent_path())){
        std::cout << "Path doesn't exists, Enter new one: " <<std::endl;
        getUserInputString(filePath);
    }
}


