//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Vehicle Panels Bay Header
//
//November 11th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 11th   Initial

#pragma once

class VehiclePanelsBay
{
private:
	
	bool bayInUse;
	int RegularF150PanelInventoryAmount;
	int SuperCabF150PanelInventoryAmount;
	int SuperCrewF150PanelInventoryAmount;
	int RegularExpeditionPanelInventoryAmount;
	int MaxExpeditionPanelInventoryAmount;

public:

	//constructor for a vehicle bay
	VehiclePanelsBay();

	//read the inventory amounts 

	int GetRegularF150InventoryAmount();

	int GetSuperCabF150InventoryAmount();

	int GetSuperCrewF150InventoryAmount();

	int GetRegularExpeditionInventoryAmount();

	int GetMaxExpeditionInventoryAmount();

	//adjust the values of the inventory (as the attributes are private to ensure integrity) 

	bool SetRegularF150InventoryAmount(int);

	bool SetSuperCabF150InventoryAmount(int);

	bool SetSuperCrewF150InventoryAmount(int);

	bool SetRegularExpeditionInventoryAmount(int);

	bool SetMaxExpeditionInventoryAmount(int);
};

