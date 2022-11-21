//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Chassis Line File
//This is where all inventory will be stored and accessed
//
//November 13th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 13th   Initial

#include "ChassisLine.h"

//constructor for a vehicle bay
ChassisLine::ChassisLine()
{
	lineBeingUsed = false;
	F15033LV6CInventoryAmount = 0;
	F15027LV6CInventoryAmount = 0;
	F15050LV8CInventoryAmount = 0;
	F15035LV6EcoCInventoryAmount = 0;
	F15035LV6PwrBstCInventoryAmount = 0;
	Expedition35LV6CInventoryAmount = 0;
	Expedition35LV6HOCInventoryAmount = 0;
}

//read the inventory amounts 

int ChassisLine::GetF15033LV6CInventoryAmount()
{
	return F15033LV6CInventoryAmount;
}

int ChassisLine::GetF15027LV6CInventoryAmount()
{
	return F15027LV6CInventoryAmount;
}

int ChassisLine::GetF15050LV8CInventoryAmount()
{
	return F15050LV8CInventoryAmount;
}

int ChassisLine::GetF15035LV6EcoCInventoryAmount()
{
	return F15035LV6EcoCInventoryAmount;
}

int ChassisLine::GetF15035LV6PwrBstCInventoryAmount()
{
	return F15035LV6PwrBstCInventoryAmount;
}

int ChassisLine::GetExpedition35LV6CInventoryAmount()
{
	return Expedition35LV6CInventoryAmount;
}

int ChassisLine::GetExpedition35LV6HOCInventoryAmount()
{
	return Expedition35LV6HOCInventoryAmount;
}

//adjust the values of the inventory (as the attributes are private to ensure integrity) 

bool ChassisLine::SetF15033LV6CInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		F15033LV6CInventoryAmount = setInvTo;
		return true;
	}
}

bool ChassisLine::SetF15027LV6CInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		F15027LV6CInventoryAmount = setInvTo;
		return true;
	}
}

bool ChassisLine::SetF15050LV8CInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		F15050LV8CInventoryAmount = setInvTo;
		return true;
	}
}

bool ChassisLine::SetF15035LV6EcoCInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		F15035LV6EcoCInventoryAmount = setInvTo;
		return true;
	}
}

bool ChassisLine::SetF15035LV6PwrBstCInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		F15035LV6PwrBstCInventoryAmount = setInvTo;
		return true;
	}
}

bool ChassisLine::SetExpedition35LV6CInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		Expedition35LV6CInventoryAmount = setInvTo;
		return true;
	}
}

bool ChassisLine::SetExpedition35LV6HOCInventoryAmount(int setInvTo)
{
	//these inventory levels are not allowed so dont assign them
	if (setInvTo < 0 || setInvTo > 500)
	{
		return false;
	}
	else
	{
		Expedition35LV6HOCInventoryAmount = setInvTo;
		return true;
	}
}

//show line in use value
void ChassisLine::lineInUse()
{
	lineBeingUsed = true;
}

void ChassisLine::lineNotInUse()
{
	lineBeingUsed = false;
}

bool ChassisLine::isThisLineInUse()
{
	return lineBeingUsed;
}