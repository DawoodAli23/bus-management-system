#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<direct.h>
#include<filesystem>
#include<vector>
#include<stdlib.h>
#include "Header.h"
using namespace std::experimental::filesystem::v1;
void mainMenu()
{
	system("cls");
	std::cout << "Main Menu\n";
	std::cout << "[1]-Book a ticket\n";
	std::cout << "[2]-Admin Panel\n";
	std::cout << "[3]-Regenrate Ticket\n";
	std::cout << "[4]-exit\n";
	void (*selectionInput[])() = { bookTicket,loginForAdmin,regenrateTicket,exit };
	int choice;
	selection(choice, 1, 4, "Enter your option here : ");
	(*selectionInput[choice - 1])();
}

void regenrateTicket() {
	system("cls");
	std::string ID, fID, str;
	std::vector<std::string> vec;
	std::ifstream info;
	info.open("schedule\\userData.txt", std::ios::in);
	std::ifstream ticketTemplate;
	ticketTemplate.open("ticket\\index.html", std::ios::in);
	std::fstream temperoryFile;
	temperoryFile.open("schedule\\temp.txt", std::ios::out);
	temperoryFile.close();
	std::ofstream tempFile;
	bool flag = false;
	tempFile.open("schedule\\temp.txt", std::ios::out);
	if (!isEmpty(info)) {
		std::cout << "Enter the ID for you want to regenrate the ticket : ";
		std::getline(std::cin, ID);
		while (std::getline(info, str)) {
			std::stringstream ss(str);
			ss >> fID;
			if (ID == fID) {
				flag = true;
				tempFile << str << std::endl;
				info.close();
				tempFile.close();
				break;
			}
		}
	}
	else {
		std::cout << "Nobody has booked a ticket yet\n";
		system("pause");
		mainMenu();
	}
	if (flag) {
		std::ifstream tempF;
		tempF.open("schedule\\temp.txt", std::ios::in);
		while (std::getline(tempF, str, '\t'))
		{
			vec.push_back(str);
		}
		_mkdir(vec.at(2).c_str());
		std::fstream generateTicket;
		generateTicket.open((vec.at(2) + "\\index.html").c_str(), std::ios::out);
		while (std::getline(ticketTemplate, str))
		{
			if (str == "pNo") {
				generateTicket << vec.at(4) << std::endl;
			}
			else if (str == "pTo") {
				generateTicket << vec.at(5) << std::endl;
			}
			else if (str == "pFrom") {
				generateTicket << vec.at(6) << std::endl;
			}
			else if (str == "pPrice") {
				generateTicket << vec.at(7) << std::endl;
			}
			else {
				generateTicket << str << std::endl;
			}
		}
		ticketTemplate.close();
		generateTicket.close();
		std::cout << "Ticket Generated succesfully\n";
	}
	else {
		std::cout << "User is not available\n";
	}
	
	system("pause");
	mainMenu();
}

void loginForAdmin()
{
	system("cls");
	std::string username, password;
	std::string fUsername, fPassword, temp;
	std::ifstream info;
	info.open("Data\\admininfo.txt", std::ios::in);
	std::getline(info, temp);
	info.close();
	std::stringstream ss(temp);
	ss >> fUsername >> fPassword;
	takeInput("Enter your username : ", username);
	takeInput("Enter your password : ", password);
	if (compareValues(username, fUsername) && compareValues(password, fPassword)) {
		adminPanel();
	}
	else {
		std::cout << "Wrong username or E-mail\nTry Again!\n";
		system("pause");
		system("cls");
		loginForAdmin();
	}
}

void adminPanel()
{
	system("cls");
	std::cout << "Admin Panel\n";
	std::cout << "[1]-Add a new bus in schedule\n";
	std::cout << "[2]-View details\n";
	std::cout << "[3]-View user's details\n";
	std::cout << "[4]-Edit\n";
	std::cout << "[5]-Chnage password\n";
	std::cout << "[6]-Main menu\n";
	void (*selectionInput[])() = { addNewBus,showAllBus,viewUserDetails,editForAdminPanel,changeAdminPassword,mainMenu };
	int choice;
	selection(choice, 1, 6, "Enter your option here : ");
	(*selectionInput[choice - 1])();
}
void viewUserDetails() {
	system("cls");
	std::ifstream info;
	info.open("schedule\\userData.txt", std::ios::in);
	std::string str;
	if (info.is_open()) {
		while (std::getline(info, str))
		{
			if (str.size() == 0) {
				continue;
			}
			else {
				std::cout << str << std::endl;
			}
		}
	}
	else {
		std::cout << "No user yet\n";
	}
	
	info.close();
	system("pause");
	adminPanel();
}
void bookTicket() {
	system("cls");
	std::cout << "Booking Panel\n";
	mainMenuBusShow();
	getData("Enter the ID of the bus you wanna book : ");
	std::string name, age, cnic, ID, uID;
	std::cin.ignore();
	takeInput("Enter your full name : ", name);
	cnicValidation(cnic);
	std::cin.ignore();
	bool flag = false;
	do {
		takeInput("Enter your age : ", age);
		flag = isNumber(age);
	} while (!flag);
	std::ifstream info;
	info.open("schedule\\userTemp.txt");
	std::getline(info, ID, '\t');
	if (decrementSeatCounter(ID)) {
		std::ifstream userID;
		userID.open("schedule\\userID.txt", std::ios::in);
		if (!userID.is_open()) {
			userID.close();
			std::fstream newUserID;
			newUserID.open("schedule\\userID.txt", std::ios::out);
			newUserID << "1" << std::endl;
			uID = "1";
			newUserID.close();
			generateTicket(name, cnic, age, uID);
		}
		else {
			std::getline(userID, uID);
			userID.close();
			remove("schedule\\userID.txt");
			std::fstream newFile;
			newFile.open("schedule\\userID.txt", std::ios::out);
			newFile << stoi(uID) + 1 << std::endl;
			newFile.close();
			uID = std::to_string(std::stoi(uID) + 1);
			generateTicket(name, cnic, age, uID);
		}
	}
	else {
		std::cout << "No tciket available\n";
		void (*selectionInput[])() = { bookTicket,mainMenu };
		toLoopFunc(selectionInput);
	}

}

void generateTicket(std::string name, std::string cnic, std::string age, std::string ID) {
	std::ofstream data;
	data.open("schedule\\userData.txt", std::ios::out | std::ios::app);
	std::ifstream info;
	info.open("schedule\\userTemp.txt", std::ios::in);
	std::ifstream ticketTemplate;
	ticketTemplate.open("ticket\\index.html", std::ios::in);
	_mkdir(name.c_str());
	std::fstream generateTicket;
	generateTicket.open((name + "\\index.html").c_str(), std::ios::out);
	std::string str;
	std::vector<std::string> vec;
	while (std::getline(info, str, '\t'))
	{
		vec.push_back(str);
	}
	info.close();
	while (std::getline(ticketTemplate, str))
	{
		if (str == "pNo") {
			generateTicket << vec.at(8) << std::endl;
		}
		else if (str == "pTo") {
			generateTicket << vec.at(2) << std::endl;
		}
		else if (str == "pFrom") {
			generateTicket << vec.at(1) << std::endl;
		}
		else if (str == "pPrice") {
			generateTicket << vec.at(7) << std::endl;
		}
		else {
			generateTicket << str << std::endl;
		}
	}
	ticketTemplate.close();
	generateTicket.close();
	data << ID << "\t" << cnic << "\t" << name << "\t" << age << "\t" << vec.at(8) << "\t" << vec.at(2) << "\t" << vec.at(1) << "\t" << vec.at(7) << std::endl;
	data.close();
	std::cout << "Your ticket generated successfully\n";
	system("pause");
	mainMenu();
}
bool decrementSeatCounter(std::string ID)
{
	std::string temp, checkID;
	std::vector<std::string> vec;
	std::ifstream info;
	info.open("schedule\\record.txt", std::ios::in);
	std::fstream writeTo;
	writeTo.open("schedule\\temp.txt", std::ios::out);
	bool flag = false;
	if (info.is_open()) {
		while (std::getline(info, temp))
		{
			std::stringstream ss(temp);
			ss >> checkID;
			if (ID == checkID) {
				std::fstream tab;
				tab.open("schedule\\tab.txt", std::ios::out);
				tab << temp << std::endl;
				tab.close();
				std::ifstream n;
				n.open("schedule\\tab.txt", std::ios::in);
				std::string str;
				while (std::getline(n, str, '\t'))
				{
					vec.push_back(str);
				}
				n.close();
				if (stoi(vec.at(8)) > 0) {
					flag = true;
					writeTo << vec.at(0) << "\t" << vec.at(1) << "\t" << vec.at(2) << "\t" << vec.at(3) << "\t" << vec.at(4) << "\t" << vec.at(5) << "\t" << vec.at(6) << "\t" << vec.at(7) << "\t" << stoi(vec.at(8)) - 1 << "\n";
				}
				else {
					writeTo << "\n" << vec.at(0) << "\t" << vec.at(1) << "\t" << vec.at(2) << "\t" << vec.at(3) << "\t" << vec.at(4) << "\t" << vec.at(5) << "\t" << vec.at(6) << "\t" << vec.at(7) << "\t" << stoi(vec.at(8)) << "\n";
				}
				
			}
			else {
				writeTo << temp << std::endl;
			}
		}
	}
	info.close();
	writeTo.close();
	remove("schedule\\tab.txt");
	remove("schedule\\record.txt");
	rename("schedule\\temp.txt", "schedule\\record.txt");
	if (!flag) {
		std::cout << "No seat available\n";
		return false;
	}
	else {
		return true;
	}
	system("pause");
	mainMenu();
}

void cnicValidation(std::string& CNIC) {
	bool condition = false;
	int countA = 0, countN = 0;
	while (countN != 13 || condition == false) {
		countA = 0; //for counting alphabets
		countN = 0; //for counting integers
		std::cout << "Enter your CNIC number : ";
		std::cin >> CNIC;
		if (CNIC.length() == 13 && CNIC.at(0) != '0' && CNIC.at(1) != '0') {
			for (int i = 0; i < 13; i++) {
				if (CNIC[i] == '0' || CNIC[i] == '1' || CNIC[i] == '2' || CNIC[i] == '3' || CNIC[i] == '4' || CNIC[i] == '5' || CNIC[i] == '6' || CNIC[i] == '7' || CNIC[i] == '8' || CNIC[i] == '9') {
					countN++;
				}
				else {
					countA++;
				}
			}
			if (countA <= 0 && countN >= 13) {
				condition = true;
			}
			else {
				std::cout << "You entered alaphabet(s), only numbers allowed" << std::endl;
			}
		}
		else {
			std::cout << "Invalid input, try again." << std::endl;
		}
	}
}

void getData(std::string query)
{
	std::cout << query;
	std::string ID, fID, temp;
	std::vector<std::string> vec;
	std::cin >> ID;
	bool flag = false;
	std::ifstream info;
	info.open("schedule\\record.txt", std::ios::in);
	if (info.is_open()) {
		while (!info.eof())
		{
			std::getline(info, temp);
			std::stringstream ss(temp);
			ss >> fID;
			if (ID == fID) {
				std::fstream nFile;
				nFile.open("schedule\\userTemp.txt", std::ios::out);
				nFile << temp;
				nFile.close();
				flag = true;
				break;
			}
		}
	}
	else {
		std::cout << "No recored exist yet\n";
		mainMenu();
	}
	if (!flag) {
		std::cout << "You enterd wrong ID\nTry again!\n";
		getData(query);
	}
	else {
		return;
	}
	info.close();
}

void addNewBus()
{
	system("cls");
	bool flag;
	std::string fromWhere, toWhere, totalNoOfSeats, noOfStays, timeOfDeparture, timeOfArrival, priceE, priceB, temp;
	int noOfFile, intNoOfSeat;
	std::ifstream info;
	info.open("schedule\\info.txt", std::ios::in);
	if (info.is_open()) {
		std::getline(info, temp);
		info.close();
		noOfFile = stoi(temp);
		takeInput("What would be the Origin : ", fromWhere);
		takeInput("What would be the destination : ", toWhere);
		do {
			takeInput("How many seats are availabe in the bus : ", totalNoOfSeats);
			flag = isNumber(totalNoOfSeats);
		} while (!flag);
		intNoOfSeat = std::stoi(totalNoOfSeats);
		do {
			takeInput("How many stays will be there between satring point and destination : ", noOfStays);
			flag = isNumber(noOfStays);
		} while (!flag);
		do {
			takeInput("What is the Time of departure : ", timeOfDeparture);
			flag = isNumber(timeOfDeparture);
		} while (!flag);
		int itimeOfDeparture = (std::stoi(timeOfDeparture) % 24);
		do {
			takeInput("What is the time of arrival : ", timeOfArrival);
			flag = isNumber(timeOfArrival);
		} while (!flag);
		int itimeOfArrival = (std::stoi(timeOfArrival) % 24);
		do {
			takeInput("What is the price of economy seat : ", priceE);
			flag = isNumber(priceE);
		} while (!flag);
		saveBusDetail(fromWhere, toWhere, intNoOfSeat, noOfStays, itimeOfDeparture, itimeOfArrival, priceE);
	}
	else {
		createFolder();
		addNewBus();
	}
	system("pause");
	adminPanel();
	
}
void saveBusDetail(std::string fromWhere, std::string toWhere, int totalNoOfseats, std::string noOfStays, int timeOfdeparture, int timeOfArrival, std::string price)
{
	std::ifstream howManyBus;
	howManyBus.open("schedule\\info.txt", std::ios::in);
	int temp;
	howManyBus >> temp;
	howManyBus.close();
	std::ofstream overwrite;
	overwrite.open("schedule\\info.txt", std::ios::out);
	overwrite << temp + 1;
	overwrite.close();
	std::fstream writeTo;
	writeTo.open("schedule\\record.txt", std::ios::out | std::ios::app);
	writeTo << "\n" << temp + 1 << "\t" << fromWhere << "\t" << toWhere << "\t" << totalNoOfseats << "\t" << noOfStays << "\t" << timeOfdeparture << "\t" << timeOfArrival << "\t" << price << "\t" << totalNoOfseats;
	writeTo.close();
	
}
void editForAdminPanel()
{
	system("cls");
	std::string noOfStays, timeOfDeparture, timeOfArrival, priceE;
	showAllBusEDIT();
	std::cout << "\n\n\n";
	std::string sID, temp, fID;
	bool flag = false;
	do
	{
		takeInput("Enter the ID of the bus you want to edit : ", sID);
		flag=isNumber(sID);
	} while (!flag);
	std::ifstream info;
	info.open("schedule\\record.txt", std::ios::in);
	bool check = false;
	if (info.is_open()) {
		while (!info.eof())
		{
			std::getline(info, temp);
			std::stringstream ss(temp);
			ss >> fID;
			if (sID == fID) {
				check = true;
				std::fstream write;
				write.open("schedule\\temp.txt", std::ios::out);
				std::stringstream newSS(temp);
				write << temp;
				write.close();
				std::ifstream n;
				n.open("schedule\\temp.txt", std::ios::in);
				std::vector<std::string> vec;
				while(!n.eof()) {
					std::getline(n, temp, '\t');
					vec.push_back(temp);
				}
				n.close();
				printVecData(vec);
				remove("schedule\\temp.txt");
				std::cout << "\n----------------------\n";
				do {
					takeInput("What is the new time of arrival : ", timeOfArrival);
					flag = isNumber(timeOfArrival);
				} while (!flag);
				int itimeOfArrival = (std::stoi(timeOfArrival) % 24);
				do {
					takeInput("What is the new time of departure : ", timeOfDeparture);
					flag = isNumber(timeOfDeparture);
				} while (!flag);
				int itimeOfDeparture = (std::stoi(timeOfDeparture) % 24);
				do {
					takeInput("How many stays are gonna be this time : ", noOfStays);
					flag = isNumber(noOfStays);
				} while (!flag);
				do {
					takeInput("What is the new price : ", priceE);
					flag = isNumber(priceE);
				} while (!flag);
				info.close();
				createEditedFile(itimeOfArrival, itimeOfDeparture, noOfStays, priceE, sID);
				break;
			}
		}
	}
	else {
		info.close();
		std::cout << "No record exist yet!\n";
		adminPanel();
		system("pause");
	}
	if (!check) {
		std::cout << "There is no bus with your given ID\nTry Again!\n";
		info.close();
		system("pause");
		editForAdminPanel();
	}
	
	
}
void createEditedFile(int toA, int toD, std::string noOfStays, std::string newPrice, std::string ID) {
	std::string temp, checkID;
	std::vector<std::string> vec;
	std::ifstream info;
	info.open("schedule\\record.txt", std::ios::in);
	std::fstream writeTo;
	writeTo.open("schedule\\temp.txt", std::ios::out);
	if (info.is_open()) {
		while (!info.eof())
		{
			std::getline(info, temp);
			std::stringstream ss(temp);
			ss >> checkID;
			if (ID == checkID) {
				std::fstream tab;
				tab.open("schedule\\tab.txt", std::ios::out);
				tab << temp << std::endl;
				tab.close();
				std::ifstream n;
				n.open("schedule\\tab.txt", std::ios::in);
				while (!n.eof())
				{
					std::string str;
					std::getline(n, str, '\t');
					vec.push_back(str);
				}
				n.close();
				writeTo << vec.at(0) << "\t" << vec.at(1) << "\t" << vec.at(2) << "\t" << vec.at(3) << "\t" << noOfStays << "\t" << toD << "\t" << toA << "\t" << newPrice << "\t" << vec.at(8);
			}
			else {
				writeTo << temp << std::endl;
			}
		}
	}
	info.close();
	writeTo.close();
	remove("schedule\\tab.txt");
	remove("schedule\\record.txt");
	rename("schedule\\temp.txt", "schedule\\record.txt");
	system("pause");
	adminPanel();
}
void changeAdminPassword()
{
	system("cls");
	std::fstream oldData;
	std::fstream newData;
	oldData.open("Data\\admininfo.txt", std::ios::in);
	newData.open("Data\\temp.txt", std::ios::out);
	std::string filePass, filePassTest, newPass, newPassTest, temp, admin;
	if (oldData.is_open()) {
		std::getline(oldData, temp);
		std::stringstream ss(temp);
		ss >> admin >> filePass;
	}
	oldData.close();
	takeInput("Enter your old password : ", filePassTest);
	takeInput("Enter new password : ", newPass);
	takeInput("Enter new password again : ", newPassTest);
	if (filePass == filePassTest && newPass == newPassTest) {
		newData << admin << "\t" << newPass;
		newData.close();
		remove("Data\\admininfo.txt");
		rename("Data\\temp.txt", "Data\\admininfo.txt");
	}
	else {
		std::cout << "You enterd wrong password\nTr Again\n";
		newData.close();
		system("pause");
		changeAdminPassword();
	}
	system("pause");
	adminPanel();
}
void showAllBus() {
	system("cls");
	std::string temp;
	/*std::cout << "Serial-No\tFrom Where\tTo Where\tTotal Seats\tNo of Stays\tTOD\tTOA\tPrice of TICKET\tAvailable Seats\n";*/
	std::ifstream info;
	info.open("schedule\\record.txt", std::ios::in);
	std::string some;
	if (info.is_open()) {
		while (!info.eof()) {
			std::fstream tempFile;
			std::ifstream readFile;
			tempFile.open("schedule\\temp.txt", std::ios::out);
			readFile.open("schedule\\temp.txt");
			std::fstream to;
			to.open("schedule\\temp.txt", std::ios::out);
			to.close();
			std::getline(info, temp);
			tempFile << temp;
			tempFile.close();
			//std::cout << temp << std::endl;
			while (std::getline(readFile, some, '\t')) {
				std::cout << some << "\t";
			}
			readFile.close();
			if (temp.size() > 0) {
				std::cout << std::endl;
			}
			remove("schedule\\temp.txt");
		}
	}
	else {
		std::cout << "No record exisit yet\n";
	}
	info.close();
	system("pause");
	adminPanel();
}
void showAllBusEDIT() {
	system("cls");
	std::string temp;
	/*std::cout << "Serial-No\tFrom Where\tTo Where\tTotal Seats\tNo of Stays\tTOD\tTOA\tPrice of TICKET\tAvailable Seats\n";*/
	std::ifstream info;
	info.open("schedule\\record.txt", std::ios::in);
	std::string some;
	if (info.is_open()) {
		while (!info.eof()) {
			std::fstream tempFile;
			std::ifstream readFile;
			tempFile.open("schedule\\temp.txt", std::ios::out);
			readFile.open("schedule\\temp.txt");
			std::fstream to;
			to.open("schedule\\temp.txt", std::ios::out);
			to.close();
			std::getline(info, temp);
			tempFile << temp;
			tempFile.close();
			//std::cout << temp << std::endl;
			while (std::getline(readFile, some, '\t')) {
				std::cout << some << "\t";
			}
			readFile.close();
			if (temp.size() > 0) {
				std::cout << std::endl;
			}
			remove("schedule\\temp.txt");
		}
	}
	else {
		std::cout << "No record exisit yet\n";
		system("pause");
		adminPanel();
	}
	info.close();
}
void mainMenuBusShow() {
	system("cls");
	std::string temp;
	/*std::cout << "Serial-No\tFrom Where\tTo Where\tTotal Seats\tNo of Stays\tTOD\tTOA\tPrice of TICKET\tAvailable Seats\n";*/
	std::ifstream info;
	info.open("schedule\\record.txt", std::ios::in);
	std::string some;
	if (info.is_open()) {
		while (!info.eof()) {
			std::fstream tempFile;
			std::ifstream readFile;
			tempFile.open("schedule\\temp.txt", std::ios::out);
			readFile.open("schedule\\temp.txt");
			std::fstream to;
			to.open("schedule\\temp.txt", std::ios::out);
			to.close();
			std::getline(info, temp);
			tempFile << temp;
			tempFile.close();
			//std::cout << temp << std::endl;
			while (std::getline(readFile, some, '\t')) {
				std::cout << some << "\t";
			}
			readFile.close();
			if (temp.size() > 0) {
				std::cout << std::endl;
			}
			remove("schedule\\temp.txt");
		}
	}
	else {
		std::cout << "No record exisit yet\n";
		system("pause");
		mainMenu();
	}
	info.close();
}
void createFolder()
{
	std::ifstream info;
	info.open("schedule\\info.txt", std::ios::in);
	std::fstream baseFile;
	if (!info.is_open()) {
		_mkdir("schedule");
		baseFile.open("schedule\\info.txt", std::ios::out);
		baseFile << "0" << std::endl;
		baseFile.close();
		info.close();
		createFolder();
		
	}
	else {
		baseFile.close();
		info.close();
		return;
	}
}
bool compareValues(std::string& first, std::string& second)
{
	if (first == second) {
		return true;
	}
	else {
		return false;
	}
}

void takeInput(std::string query, std::string& target) {
	std::cout << query;
	do {
		std::getline(std::cin, target);
		if (target.length() == 0) {
			std::cout << "You did'nt enter any data\nTry Again\n";
			std::cout << query;
		}
	} while (target.length() == 0);
	
}
void createFile() {
	std::fstream file;
	file.open("new.txt", std::ios::out);
	file.close();
}

bool isEmpty(std::ifstream& file)
{
	bool flag = file.peek() == std::ifstream::traits_type::eof();
	return flag;
}

void selection(int& num, int min, int max, std::string query)
{
	std::cout << query;
	std::string choice;
	std::getline(std::cin, choice);
	if (isNumber(choice)) {
		if (stoi(choice) >= min && stoi(choice) <= max) {
			num = stoi(choice);
		}
		else {
			std::cout << "INVALID SELECTION\n";
			std::cout << "TRY AGAIN\n";
			system("pause");
			selection(num, min, max, query);
		}
	}
	else {
		std::cout << "INVALID SELECTION\n";
		std::cout << "TRY AGAIN\n";
		system("pause");
		selection(num, min, max, query);
	}
	
}

bool isNumber(std::string str)
{
	if (str.length() > 1) {
		for (int i = 0; i < str.length(); i++)
		{
			if (!isdigit(str.at(i))) {
				return false;
			}
		}
		return true;
	}
	return isdigit(str.at(0));
}

void filterByOrigin() {
	std::string temp, id, sID, arrival;
	std::ifstream info;
	info.open("schedule\\record.txt", std::ios::in);
	bool flag = false;
	if (info.is_open()) {
		std::cout << "Enter the origin you wish to see : ";
		std::cin >> id;
		while (std::getline(info, temp)) {
			std::stringstream ss(temp);
			ss >> sID >> arrival;
			if (id == arrival) {
				flag = true;
				info.close();
				std::ofstream write;
				write.open("schedule\\temp.txt", std::ios::out);
				write << temp;
				write.close();
				break;
			}
		}
		if (flag) {
			std::ifstream file;
			file.open("schedule\\temp.txt", std::ios::in);
			std::vector<std::string> vec;
			while (std::getline(file, temp, '\t'))
			{
				vec.push_back(temp);
			}
			file.close();
			printVecData(vec);
		}
		else {
			std::cout << "No record exist\n";
			std::cout << "You wanna try Again :\n";
			void (*arr[])() = { filterByOrigin };
			toLoopFunc(arr);
		}

	}
	else {
		info.close();
		std::cout << "No record exist\n";
	}
}

void filterByDestination() {
	std::string temp, id, sID, dest, arrival;
	std::ifstream info;
	info.open("schedule\\record.txt", std::ios::in);
	bool flag = false;
	if (info.is_open()) {
		std::cout << "Enter the destination you wish to see : ";
		std::cin >> id;
		while (std::getline(info, temp)) {
			std::stringstream ss(temp);
			ss >> sID >> arrival >> dest;
			if (id == dest) {
				flag = true;
				info.close();
				std::ofstream write;
				write.open("schedule\\temp.txt", std::ios::out);
				write << temp;
				write.close();
				break;
			}
		}
		if (flag) {
			std::ifstream file;
			file.open("schedule\\temp.txt", std::ios::in);
			std::vector<std::string> vec;
			while (std::getline(file, temp, '\t'))
			{
				vec.push_back(temp);
			}
			file.close();
			printVecData(vec);
		}
		else {
			std::cout << "No record exist\n";
			std::cout << "You wanna try Again :\n";
			void (*arr[])() = { filterByDestination };
			toLoopFunc(arr);
		}

	}
	else {
		info.close();
		std::cout << "No record exist\n";
	}
}

void filterByBusID() {
	std::string temp, id, sID;
	std::ifstream info;
	info.open("schedule\\record.txt", std::ios::in);
	bool flag = false;
	if (info.is_open()) {
		std::cout << "Enter the ID of the user you wish to see : ";
		std::cin >> id;
		while (std::getline(info, temp)) {
			std::stringstream ss(temp);
			ss >> sID;
			if (id == sID) {
				flag = true;
				info.close();
				std::ofstream write;
				write.open("schedule\\temp.txt", std::ios::out);
				write << temp;
				write.close();
				break;
			}
		}
		if (flag) {
			std::ifstream file;
			file.open("schedule\\temp.txt", std::ios::in);
			std::vector<std::string> vec;
			while (std::getline(file, temp, '\t'))
			{
				vec.push_back(temp);
			}
			file.close();
			printVecData(vec);
		}
		else {
			std::cout << "No record exist\n";
			std::cout << "You wanna try Again :\n";
			void (*arr[])() = { filterByBusID };
			toLoopFunc(arr);
		}
		
	}
	else {
		info.close();
		std::cout << "No record exist\n";
	}
}

void toLoopFunc(void (*arr[])())
{
	int num = 0;
	selection(num, 1, 2, "[1]-To Try again\n[2]-to return to previous menu\nEnter here : ");
	arr[num - 1]();
}

void printVecData(std::vector<std::string> vec)
{
	std::cout << "ID of the bus : " << vec.at(0) << std::endl;
	std::cout << "Origin : " << vec.at(1) << std::endl;
	std::cout << "Destination : " << vec.at(2) << std::endl;
	std::cout << "Total number of seats : " << vec.at(3) << std::endl;
	std::cout << "No of stays : " << vec.at(4) << std::endl;
	std::cout << "Time of departure : " << vec.at(5) << std::endl;
	std::cout << "Time of arrival : " << vec.at(6) << std::endl;
	std::cout << "Price of one seat : " << vec.at(7) << std::endl;
	std::cout << "Number of available seats : " << vec.at(8) << std::endl;
}

void exit() {
	exit(0);
	//return;
}