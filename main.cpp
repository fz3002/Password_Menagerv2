#include <iostream>
#include "UI.hpp"
#if _WIN64 || _WIN32
    #define OS "Windows"
#elif __linux__
    #define __OS "Linux"
#endif


auto main()->int {

    UI ui = UI();

    UI::clearTerminal();

    std::cout << "                                          Welcome to the Password Manager!!!\n" << std::endl;
    ui.chooseFile();
    UI::clearTerminal();
    ui.enterFile();
    if(ui.getIncorrectPassword()) {
        return -1;
    }
    UI::clearTerminal();

    while(ui.getCommand() != 8){
        ui.dataPrint();
        ui.menu();
        UI::clearTerminal();

        switch(ui.getCommand()) {
            case 1:
                if (UI::confirmation("--ADD ENTRY")) {
                    ui.addEntry();
                }
                break;
            case 2:
                if (UI::confirmation("--DELETE ENTRY")) {
                    ui.deleteEntry();
                }
                break;
            case 3:
                if (UI::confirmation("--EDIT ENTRY")) {
                    ui.editEntry();
                }
                break;
            case 4:
                if (UI::confirmation("--SEARCH ENTRY")) {
                    ui.searchEntry();
                }
                break;
            case 5: if (UI::confirmation("--SORT ENTRIES")) {
                        ui.sortEntries();
                    }
                    break;
            case 6: if (UI::confirmation("--ADD CATEGORY")) {
                        ui.addCategory();
                    }
                    break;
            case 7: if (UI::confirmation("--DELETE CATEGORY")) {
                        ui.deleteCategory();
                    }
                    break;
            case 8: break;
            default: std::cout << "Invalid command!" << std::endl;
        }

        ui.writeToFile();
        UI::clearTerminal();
    }

    return 0;
}