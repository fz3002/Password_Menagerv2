#include <iostream>
#include <string>
#include "UI.hpp"
#if _WIN64 || _WIN32
    #define OS "Windows"
#elif __linux__
    #define __OS "Linux"
#endif

auto main()->int{
    if(OS == "Windows") system("cls");
    else system("clear");
    std::cout << "Welcome to the Pasword Manager!" << std::endl;
    UI ui = UI();
    ui.chooseFile();
    if(OS == "Windows") system("cls");
    else system("clear");
    ui.enterFile();
    if(ui.getIncorectPassword()) return -1;
    /*
    if(__OS == "Windows") system("cls");
    else system("clear");
    */
    while(ui.getCommand() != 8){
        ui.dataPrint();
        ui.menu();
        if(OS == "Windows") system("cls");
        else system("clear");
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
    return 0;
}