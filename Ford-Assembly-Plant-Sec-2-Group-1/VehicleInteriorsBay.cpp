//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Vehicle Interiors Bay File
//This is where all inventory will be stored and accessed
//
//November 14th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 14th   Initial

#include "VehicleInteriorsBay.h"

//constructor for a vehicle bay
VehicleInteriorsBay::VehicleInteriorsBay()
{
	bayBeingUsed = false;
	BaseF150InteriorInventoryAmount = 0;
	MidF150InteriorInventoryAmount = 0;
	HighF150InteriorInventoryAmount = 0;
	BaseExpeditionInteriorInventoryAmount = 0;
	MidExpeditionInteriorInventoryAmount = 0;
	HighExpeditionInteriorInventoryAmount = 0;
}

//read the inventory amounts 

int VehicleInteriorsBay::GetBaseF150InteriorInventoryAmount()
{
	return BaseF150InteriorInventoryAmount;
}

int VehicleInteriorsBay::GetMidF150InteriorInventoryAmount()
{
	return MidF150InteriorInventoryAmount;
}

int VehicleInteriorsBay::GetHighF150InteriorInventoryAmount()
{
	return HighF150InteriorInventoryAmount;
}

int VehicleInteriorsBay::GetBaseExpeditionInteriorInventoryAmount()
{
	return BaseExpeditionInteriorInventoryAmount;
}

int VehicleInteriorsBay::GetMidExpeditionInteriorInventoryAmount()
{
	return MidExpeditionInteriorInventoryAmount;
}

int VehicleInteriorsBay::GetHighExpeditionInteriorInventoryAmount()
{
	return HighExpeditionInteriorInventoryAmount;
}

//adjust the values of the inventory (as the attributes are private to ensure integrity) 

bool VehicleInteriorsBay::SetBaseF150InteriorInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		BaseF150InteriorInventoryAmount = setInvTo;
		return true;
	}
}

bool VehicleInteriorsBay::SetMidF150InteriorInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		MidF150InteriorInventoryAmount = setInvTo;
		return true;
	}
}

bool VehicleInteriorsBay::SetHighF150InteriorInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		HighF150InteriorInventoryAmount = setInvTo;
		return true;
	}
}

bool VehicleInteriorsBay::SetBaseExpeditionInteriorInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		BaseExpeditionInteriorInventoryAmount = setInvTo;
		return true;
	}
}

bool VehicleInteriorsBay::SetMidExpeditionInteriorInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		MidExpeditionInteriorInventoryAmount = setInvTo;
		return true;
	}
}

bool VehicleInteriorsBay::SetHighExpeditionInteriorInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		HighExpeditionInteriorInventoryAmount = setInvTo;
		return true;
	}
}

void VehicleInteriorsBay::bayInUse()
{
	bayBeingUsed = true;
}

void VehicleInteriorsBay::bayNotInUse()
{
	bayBeingUsed = false;
}

bool VehicleInteriorsBay::bayThisLineInUse()
{
	return bayBeingUsed;
}