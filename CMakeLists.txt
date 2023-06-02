cmake_minimum_required(VERSION 3.24)
project(Password_Manager)

set(CMAKE_CXX_STANDARD 23)

add_executable(Password_Manager main.cpp EncryptDecrypt.cpp EncryptDecrypt.hpp FileEntry.cpp FileEntry.hpp Password.cpp Password.hpp UI.cpp UI.hpp Categories.hpp Categories.cpp UserInput.cpp UserInput.hpp)
