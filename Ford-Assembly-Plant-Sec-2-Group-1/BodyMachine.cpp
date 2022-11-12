//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Body Machine File
//This file will control of features of the body machine including, tracking inventory of body panels and assigning the correct panel set to the given vehicles 
//
//November 11th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 11th   Initial

#include "BodyMachine.h"

//bodyMachine constructor
BodyMachine::BodyMachine()
{

}

//this method will run the bodymachine where an order and a vehicle outline will be provided
bool BodyMachine::RunBodyMachine(Order givenOrder, Vehicle vehiclePlaceHolder)
{

}

//switch the inventory inlet into the bodyMachine from the current bay to another valid bay to allow the original bay to be restocked 

bool BodyMachine::SwitchVehiclePanelsBays(string switchToBay)
{

}

//adjust the inventory counts 

bool BodyMachine::UpdateRegularF150InventoryAmount(int setInvLevelTo, string currentBay)
{

}

bool BodyMachine::UpdateSuperCabF150InventoryAmount(int setInvLevelTo, string currentBay)
{

}

bool BodyMachine::UpdateSuperCrewF150InventoryAmount(int setInvLevelTo, string currentBay)
{

}

bool BodyMachine::UpdateRegularExpeditionInventoryAmount(int setInvLevelTo, string currentBay)
{

}

bool BodyMachine::UpdateMaxExpeditionInventoryAmount(int setInvLevelTo, string currentBay)
{

}

//record activity to the log (will write inventory levels and time to a text file **should be called every time any change is made**) 

bool BodyMachine::WriteTakenInventoryToLog(int inventoryLevel, string timeAndDate)
{

}

//set the notification threshold for the amount of inventory 

bool BodyMachine::SetLowLevelLimit(int givenThreshold)
{

}

//read the current threshold for the amount of inventory 

int BodyMachine::GetLowLevelLimit()
{

}