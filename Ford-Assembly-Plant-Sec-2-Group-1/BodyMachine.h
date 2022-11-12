//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Body Machine Header
//
//November 11th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 11th   Initial

#pragma once

#include <iostream>
#include <string.h>
#include "VehiclePanelsBay.h"

using namespace std;

//----------------------------------------------THESE ARE STUBS AND WILL BE REMOVED AFTER INTEGRATION-------------------------------------------------------\\

class Vehicle
{
public:

	string body;

};

class Order
{
public:

	string bodyPanelSet;
	string model;

};


class BodyMachine
{
private:
	
	string bodyType;
	VehiclePanelsBay* currentBay;
	int lowLevelLimit;

public:

	VehiclePanelsBay bayOne;
	VehiclePanelsBay bayTwo;
	bool inventoryLevelsValid;
	
	//bodyMachine constructor
	BodyMachine();

	//this method will run the bodymachine where an order and a vehicle outline will be provided
	bool RunBodyMachine(Order, Vehicle);

	//switch the inventory inlet into the bodyMachine from the current bay to another valid bay to allow the original bay to be restocked 

	bool SwitchVehiclePanelsBays(string);

	//adjust the inventory counts 

	bool UpdateRegularF150InventoryAmount(int, string);

	bool UpdateSuperCabF150InventoryAmount(int, string);

	bool UpdateSuperCrewF150InventoryAmount(int, string);

	bool UpdateRegularExpeditionInventoryAmount(int, string);

	bool UpdateMaxExpeditionInventoryAmount(int, string);

	//record activity to the log (will write inventory levels and time to a text file **should be called every time any change is made**) 

	bool WriteTakenInventoryToLog(int, string);

	//set the notification threshold for the amount of inventory 

	bool SetLowLevelLimit(int);

	//read the current threshold for the amount of inventory 

	int GetLowLevelLimit();

};



