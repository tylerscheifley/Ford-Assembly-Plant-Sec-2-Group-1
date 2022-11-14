//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Vehicle Interior Bay Header
//
//November 14th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 14th   Initial

#pragma once

class VehicleInteriorsBay
{
private:

	bool bayBeingUsed;
	int BaseF150InteriorInventoryAmount;
	int MidF150InteriorInventoryAmount;
	int HighF150InteriorInventoryAmount;
	int BaseExpeditionInteriorInventoryAmount;
	int MidExpeditionInteriorInventoryAmount;
	int HighExpeditionInteriorInventoryAmount;

public:

	//constructor for a vehicle bay
	VehicleInteriorsBay();

	//read the inventory amounts 

	int GetBaseF150InteriorInventoryAmount();

	int GetMidF150InteriorInventoryAmount();

	int GetHighF150InteriorInventoryAmount();

	int GetBaseExpeditionInteriorInventoryAmount();

	int GetMidExpeditionInteriorInventoryAmount();

	int GetHighExpeditionInteriorInventoryAmount();

	//adjust the values of the inventory (as the attributes are private to ensure integrity) 

	bool SetBaseF150InteriorInventoryAmount(int);

	bool SetMidF150InteriorInventoryAmount(int);

	bool SetHighF150InteriorInventoryAmount(int);

	bool SetBaseExpeditionInteriorInventoryAmount(int);

	bool SetMidExpeditionInteriorInventoryAmount(int);

	bool SetHighExpeditionInteriorInventoryAmount(int);

	//show bay in use value
	void bayInUse();
	void bayNotInUse();
	bool bayThisLineInUse();
};