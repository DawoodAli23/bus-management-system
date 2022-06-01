#pragma once
#include<iostream>
#include<string>
#include<vector>
void mainMenu();
void loginForAdmin();
void adminPanel();
void addNewBus();
void saveBusDetail(std::string fromWhere, std::string toWhere, int totalNoOfseats, std::string noOfStays, int timeOfdeparture, int timeOfArrival, std::string price);
void editForAdminPanel();
void createEditedFile(int toA, int toD, std::string noOfStays, std::string newPrice, std::string ID);
void changeAdminPassword();
void showAllBus();
void createFolder();
bool compareValues(std::string& first, std::string& second);
void takeInput(std::string query, std::string& target);
void createFile();
bool isEmpty(std::ifstream& file);
void selection(int& num, int min, int max, std::string query);
bool isNumber(std::string str);
void filterByOrigin();
void filterByDestination();
void filterByBusID();
void toLoopFunc(void (*arr[])());
void printVecData(std::vector<std::string> vec);
void showAllBusEDIT();
void bookTicket();
bool decrementSeatCounter(std::string ID);
void getData(std::string query);
void generateTicket(std::string name, std::string cnic, std::string age, std::string ID);
void cnicValidation(std::string& CNIC);
void regenrateTicket();
void viewUserDetails();
void mainMenuBusShow();
void exit();