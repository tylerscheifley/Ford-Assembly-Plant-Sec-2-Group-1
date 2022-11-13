//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Chassis Line HeaderFile
//
//November 13th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 13th   Initial

#pragma once

class ChassisLine
{
private:

	bool lineBeingUsed;
	int F15033LV6CInventoryAmount;
	int F15027LV6CInventoryAmount;
	int F15050LV8CInventoryAmount;
	int F15035LV6EcoCInventoryAmount;
	int F15035LV6PwrBstCInventoryAmount;
	int Expedition35LV6CInventoryAmount;
	int Expedition35LV6HOCInventoryAmount;

public:

	//constructor for a vehicle chassis line
	ChassisLine();

	//read the inventory amounts 

	int GetF15033LV6CInventoryAmount();

	int GetF15027LV6CInventoryAmount();

	int GetF15050LV8CInventoryAmount();

	int GetF15035LV6EcoCInventoryAmount();

	int GetF15035LV6PwrBstCInventoryAmount();

	int GetExpedition35LV6CInventoryAmount();

	int GetExpedition35LV6HOCInventoryAmount();

	//adjust the values of the inventory (as the attributes are private to ensure integrity) 

	bool SetF15033LV6CInventoryAmount(int);

	bool SetF15027LV6CInventoryAmount(int);

	bool SetF15050LV8CInventoryAmount(int);

	bool SetF15035LV6EcoCInventoryAmount(int);

	bool SetF15035LV6PwrBstCInventoryAmount(int);

	bool SetExpedition35LV6CInventoryAmount(int);

	bool SetExpedition35LV6HOCInventoryAmount(int);

	//show line in use value
	void lineInUse();
	void lineNotInUse();
	bool isThisLineInUse();
};
