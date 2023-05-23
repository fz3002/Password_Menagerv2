cmake_minimum_required(VERSION 3.24)
project(Password_Menager)

set(CMAKE_CXX_STANDARD 17)

add_executable(Password_Menager main.cpp EncryptDecrypt.cpp EncryptDecrypt.hpp FileEntry.cpp FileEntry.hpp Password.cpp Password.hpp UI.cpp UI.hpp Categories.hpp Categories.cpp UserInput.cpp UserInput.hpp)
