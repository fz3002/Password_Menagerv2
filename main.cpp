#include <iostream>
#include <Windows.h>
#include "UI.hpp"
#if _WIN64 || _WIN32
    #define OS "Windows"
#elif __linux__
    #define __OS "Linux"
#endif


auto main()->int {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

    UI ui = UI();

    UI::clearTerminal();

    std::cout << "                                          Welcome to the Pasword Manager!!!\n" << std::endl;
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

        switch(ui.getCommand()){
            case 1: ui.addEntry();
                    break;
            case 2: ui.deleteEntry();
                    break;
            case 3: ui.editEntry();
                    break;
            case 4: ui.searchEntry();
                    break;
            case 5: ui.sortEntries();
                    break;
            case 6: ui.addCategory();
                    break;
            case 7: ui.deleteCategory();
                    break;
            case 8: break;
            default: std::cout << "Invalid command!" << std::endl;
        }

        ui.writeToFile();
    }

    SetConsoleMode(hStdin, mode);
    return 0;
}