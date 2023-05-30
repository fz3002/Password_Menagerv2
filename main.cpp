#include <iostream>
#include <string>
#include <Windows.h>
#include "UI.hpp"
#if _WIN64 || _WIN32
    #define OS "Windows"
#elif __linux__
    #define __OS "Linux"
#endif

auto main()->int {

    //MSG msg;
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

    /* TODO: Think about creating hotkey in a separate thread
    if(!(RegisterHotKey(nullptr, 1, MOD_CONTROL | MOD_SHIFT, VK_F1))) {
        std::cout << "Failed to register hotkey!" <<std::endl;
        return 1;
    }
    while(GetMessage(&msg, nullptr, 0, 0) != 0) {
        if(msg.message == WM_HOTKEY){
            if(msg.wParam == 1){
                ui.dataPrint();
                ui.menu();
                UI::clearTerminal();
            }
        }
    }
     */

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

    UnregisterHotKey(NULL, 1);
    return 0;
}