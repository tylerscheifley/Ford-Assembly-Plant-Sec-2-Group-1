//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Interior Machine Header
//
//November 14th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 14th   Initial

#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <string.h>
#include "VehicleInteriorsBay.h"

using namespace std;

//----------------------------------------------THESE ARE STUBS AND WILL BE REMOVED AFTER INTEGRATION-------------------------------------------------------\\

class Vehicle
{
public:

	string interior;

	void setInterior(string interiorType)
	{
		interior = interiorType;
	}

};

class Order
{
public:

	string interiorLevel;
	string model;

};


class InteriorMachine
{
private:

	string interiorType;
	VehicleInteriorsBay* currentBay;
	int lowLevelLimit;

public:

	VehicleInteriorsBay bayOne;
	VehicleInteriorsBay bayTwo;
	bool inventoryLevelsValid;

	//InteriorMachine constructor
	InteriorMachine();

	//this method will run the InteriorMachine where an order and a vehicle outline will be provided
	bool RunInteriorMachine(Order, Vehicle*);

	//switch the inventory inlet into the InteriorMachine from the current bay to another valid bay to allow the original bay to be restocked 

	bool SwitchVehiclePanelsBays(string);

	//adjust the inventory counts 

	bool UpdateBaseInteriorF150InventoryAmount(int, string);

	bool UpdateMidInteriorF150InventoryAmount(int, string);

	bool UpdateHighInteriorF150InventoryAmount(int, string);

	bool UpdateBaseInteriorExpeditionInventoryAmount(int, string);

	bool UpdateMidInteriorExpeditionInventoryAmount(int, string);

	bool UpdateHighInteriorExpeditionInventoryAmount(int, string);

	//record activity to the log (will write inventory levels and time to a text file **should be called every time any change is made**) 

	bool WriteTakenInventoryToLog();

	//set the notification threshold for the amount of inventory 

	bool SetLowLevelLimit(int);

	//read the current threshold for the amount of inventory 

	int GetLowLevelLimit();

};
