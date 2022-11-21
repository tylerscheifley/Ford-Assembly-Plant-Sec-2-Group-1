//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Chassis Machine Header
//
//November 13th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 13th   Initial

#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <string.h>
#include "ChassisLine.h"

using namespace std;

//----------------------------------------------THESE ARE STUBS AND WILL BE REMOVED AFTER INTEGRATION-------------------------------------------------------\\

class Vehicle
{
public:

	string chassis;

	void setChassis(string chassisType)
	{
		chassis = chassisType;
	}

};

class Order
{
public:

	string engineType;
	string model;

};


class ChassisMachine
{
private:

	string chassisType;
	ChassisLine* currentLine;
	int lowLevelLimit;

public:

	ChassisLine lineOne;
	ChassisLine lineTwo;
	bool inventoryLevelsValid;

	//chassisMachine constructor
	ChassisMachine();

	//this method will run the chassismachine where an order and a vehicle outline will be provided
	bool RunChassisMachine(Order, Vehicle*);

	//switch the inventory inlet into the chassisMachine from the current line to another valid line to allow the original line to be restocked 

	bool SwitchVehicleChassisLines(string);

	//adjust the inventory counts 

	bool UpdateF15033LV6CInventoryAmount(int, string);

	bool UpdateF15027LV6CInventoryAmount(int, string);

	bool UpdateF15050LV8CInventoryAmount(int, string);

	bool UpdateF15035LV6EcoCInventoryAmount(int, string);

	bool UpdateF15035LV6PwrBstCInventoryAmount(int, string);

	bool UpdateExpedition35LV6CInventoryAmount(int, string);

	bool UpdateExpedition35LV6HOCInventoryAmount(int, string);

	//record activity to the log (will write inventory levels and time to a text file **should be called every time any change is made**) 

	bool WriteTakenInventoryToLog();

	//set the notification threshold for the amount of inventory 

	bool SetLowLevelLimit(int);

	//read the current threshold for the amount of inventory 

	int GetLowLevelLimit();

};
