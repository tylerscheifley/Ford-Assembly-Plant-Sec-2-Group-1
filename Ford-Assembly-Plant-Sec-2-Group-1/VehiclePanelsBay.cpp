//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Vehicle Panels Bay File
//This is where all inventory will be stored and accessed
//
//November 11th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 11th   Initial

#include "VehiclePanelsBay.h"

//constructor for a vehicle bay
VehiclePanelsBay::VehiclePanelsBay()
{
	bayInUse = false;
	RegularF150PanelInventoryAmount = 0;
	SuperCabF150PanelInventoryAmount = 0;
	SuperCrewF150PanelInventoryAmount = 0;
	RegularExpeditionPanelInventoryAmount = 0;
	MaxExpeditionPanelInventoryAmount = 0;
}

//read the inventory amounts 

int VehiclePanelsBay::GetRegularF150InventoryAmount()
{
	return RegularF150PanelInventoryAmount;
}

int VehiclePanelsBay::GetSuperCabF150InventoryAmount()
{
	return SuperCabF150PanelInventoryAmount;
}

int VehiclePanelsBay::GetSuperCrewF150InventoryAmount()
{
	return SuperCrewF150PanelInventoryAmount;
}

int VehiclePanelsBay::GetRegularExpeditionInventoryAmount()
{
	return RegularExpeditionPanelInventoryAmount;
}

int VehiclePanelsBay::GetMaxExpeditionInventoryAmount()
{
	return MaxExpeditionPanelInventoryAmount;
}

//adjust the values of the inventory (as the attributes are private to ensure integrity) 

bool VehiclePanelsBay::SetRegularF150InventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		RegularF150PanelInventoryAmount = setInvTo;
		return true;
	}
}

bool VehiclePanelsBay::SetSuperCabF150InventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		SuperCabF150PanelInventoryAmount = setInvTo;
		return true;
	}
}

bool VehiclePanelsBay::SetSuperCrewF150InventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		SuperCrewF150PanelInventoryAmount = setInvTo;
		return true;
	}
}

bool VehiclePanelsBay::SetRegularExpeditionInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		RegularExpeditionPanelInventoryAmount = setInvTo;
		return true;
	}
}

bool VehiclePanelsBay::SetMaxExpeditionInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		MaxExpeditionPanelInventoryAmount = setInvTo;
		return true;
	}
}