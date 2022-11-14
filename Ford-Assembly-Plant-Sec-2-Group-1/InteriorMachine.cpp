//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Interior Machine File
//This file will control of features of the interior machine including, tracking inventory of interior sets and assigning the correct interior level set to the given vehicles 
//
//November 14th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 14th   Initial

#define _CRT_SECURE_NO_WARNINGS

#include "InteriorMachine.h"

//InteriorMachine constructor
InteriorMachine::InteriorMachine()
{
	interiorType = "N/A";
	currentBay = &bayOne;
	lowLevelLimit = 10;
	inventoryLevelsValid = false;
	VehicleInteriorsBay newBay;
	bayOne = newBay;
	VehicleInteriorsBay newBay1;
	bayTwo = newBay1;
	//read inventory levels in from the bay (the text file)

	//Bay 1-----------------------
	ifstream file("InteriorMachineBay1Inv.txt");
	string number;
	int baseF150, midF150, highF150, baseExp, midExp, highExp = 0;
	string line;
	if (file.is_open()) {

		while (getline(file, line)) {
			stringstream ss(line);
			getline(ss, number, ',');
			baseF150 = stoi(number);
			getline(ss, number, ',');
			midF150 = stoi(number);
			getline(ss, number, ',');
			highF150 = stoi(number);
			getline(ss, number, ',');
			baseExp = stoi(number);
			getline(ss, number, ',');
			midExp = stoi(number);
			number = "\0";
			getline(ss, number, '\n');
			if (number != "\0") {
				highExp = stoi(number);
			}
			bayOne.SetBaseF150InteriorInventoryAmount(baseF150);
			bayOne.SetMidF150InteriorInventoryAmount(midF150);
			bayOne.SetHighF150InteriorInventoryAmount(highF150);
			bayOne.SetBaseExpeditionInteriorInventoryAmount(baseExp);
			bayOne.SetMidExpeditionInteriorInventoryAmount(midExp);
			bayOne.SetHighExpeditionInteriorInventoryAmount(highExp);

		}
	}
	else {
		cout << "file cannot be opened!\n" << "Filename : InteriorMachineBay1Inv.txt" << endl;
	}

	file.close();

	//Bay 2-----------------

	ifstream file1("InteriorMachineBay2Inv.txt");
	if (file1.is_open()) {

		while (getline(file1, line)) {
			stringstream ss(line);
			getline(ss, number, ',');
			baseF150 = stoi(number);
			getline(ss, number, ',');
			midF150 = stoi(number);
			getline(ss, number, ',');
			highF150 = stoi(number);
			getline(ss, number, ',');
			baseExp = stoi(number);
			getline(ss, number, ',');
			midExp = stoi(number);
			number = "\0";
			getline(ss, number, '\n');
			if (number != "\0") {
				highExp = stoi(number);
			}
			bayTwo.SetBaseF150InteriorInventoryAmount(baseF150);
			bayTwo.SetMidF150InteriorInventoryAmount(midF150);
			bayTwo.SetHighF150InteriorInventoryAmount(highF150);
			bayTwo.SetBaseExpeditionInteriorInventoryAmount(baseExp);
			bayTwo.SetMidExpeditionInteriorInventoryAmount(midExp);
			bayTwo.SetHighExpeditionInteriorInventoryAmount(highExp);

		}
	}
	else {
		cout << "file cannot be opened!\n" << "Filename : InteriorMachineBay2Inv.txt" << endl;
	}

	file1.close();

}

//this method will run the InteriorMachine where an order and a vehicle outline will be provided
bool InteriorMachine::RunInteriorMachine(Order givenOrder, Vehicle* vehiclePlaceHolder)
{
	interiorType = givenOrder.interiorLevel + givenOrder.model;

	if (interiorType == "baseF150")
	{
		if (currentBay->GetBaseF150InteriorInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO BASE F150 INTERIOR SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (interiorType == "midF150")
	{
		if (currentBay->GetMidF150InteriorInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO MID F150 INTERIOR SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (interiorType == "highF150")
	{
		if (currentBay->GetHighF150InteriorInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO HIGH F150 INTERIOR SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (interiorType == "baseExpedition")
	{
		if (currentBay->GetBaseExpeditionInteriorInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO BASE EXPEDITION INTERIOR SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (interiorType == "midExpedition")
	{
		if (currentBay->GetMidExpeditionInteriorInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO MID EXPEDITION INTERIOR SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (interiorType == "highExpedition")
	{
		if (currentBay->GetHighExpeditionInteriorInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO HIGH EXPEDITION INTERIOR SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}


	//check inventory levels for "warnings"
	if (bayOne.GetBaseF150InteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Base level F150 interior sets inventory below threshold in bay1" << endl;
	}
	else if (bayTwo.GetBaseF150InteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Base level F150 interior sets inventory below threshold in bay2" << endl;

	}
	else if (bayOne.GetMidF150InteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Mid level F150 interior sets inventory below threshold in bay1" << endl;
	}
	else if (bayTwo.GetMidF150InteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Mid level F150 interior sets inventory below threshold in bay2" << endl;

	}
	else if (bayOne.GetHighF150InteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! High level F150 interior sets inventory below threshold in bay1" << endl;

	}
	else if (bayTwo.GetHighF150InteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! High level F150 interior sets inventory below threshold in bay2" << endl;

	}
	else if (bayOne.GetBaseExpeditionInteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Base level Expedition interior sets inventory below threshold in bay1" << endl;

	}
	else if (bayTwo.GetBaseExpeditionInteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Base level Expedition interior sets inventory below threshold in bay2" << endl;

	}
	else if (bayOne.GetMidExpeditionInteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Mid level Expedition interior sets inventory below threshold in bay1" << endl;

	}
	else if (bayTwo.GetMidExpeditionInteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Mid level Expedition interior sets inventory below threshold in bay2" << endl;
	}
	else if (bayOne.GetHighExpeditionInteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! High level Expedition interior sets inventory below threshold in bay1" << endl;

	}
	else if (bayTwo.GetHighExpeditionInteriorInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! High level Expedition interior sets inventory below threshold in bay2" << endl;
	}
	else // Inventory Levels are good across the board
	{
		inventoryLevelsValid = true;
	}

	//update the vehicle to have the right body
	vehiclePlaceHolder->setInterior(interiorType);

	bool didAdd = false;

	//update inventory levels
	if (vehiclePlaceHolder->interior == "baseF150")
	{
		currentBay->SetBaseF150InteriorInventoryAmount((currentBay->GetBaseF150InteriorInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->interior == "midF150")
	{
		currentBay->SetMidF150InteriorInventoryAmount((currentBay->GetMidF150InteriorInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->interior == "highF150")
	{
		currentBay->SetHighF150InteriorInventoryAmount((currentBay->GetHighF150InteriorInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->interior == "baseExpedition")
	{
		currentBay->SetBaseExpeditionInteriorInventoryAmount((currentBay->GetBaseExpeditionInteriorInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->interior == "midExpedition")
	{
		currentBay->SetMidExpeditionInteriorInventoryAmount((currentBay->GetMidExpeditionInteriorInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->interior == "highExpedition")
	{
		currentBay->SetHighExpeditionInteriorInventoryAmount((currentBay->GetHighExpeditionInteriorInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	//Only save the inventory in each bay if the inventory was changed
	if (didAdd == true)
	{
		ofstream fout;
		fout.open("InteriorMachineBay1Inv.txt", ios::trunc); //we want to trunc to the file or essentially write over the existing data
		if (fout.is_open())
		{
			fout << bayOne.GetBaseF150InteriorInventoryAmount()
				<< "," << bayOne.GetMidF150InteriorInventoryAmount()
				<< "," << bayOne.GetHighF150InteriorInventoryAmount()
				<< "," << bayOne.GetBaseExpeditionInteriorInventoryAmount()
				<< "," << bayOne.GetMidExpeditionInteriorInventoryAmount()
				<< "," << bayOne.GetHighExpeditionInteriorInventoryAmount() << endl;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : InteriorMachineBay1Inv.txt" << endl;
		}
		ofstream fout1;
		fout1.open("InteriorMachineBay2Inv.txt", ios::trunc); //we want to trunc to the file or essentially write over the existing data
		if (fout1.is_open())
		{
			fout1 << bayTwo.GetBaseF150InteriorInventoryAmount()
				<< "," << bayTwo.GetMidF150InteriorInventoryAmount()
				<< "," << bayTwo.GetHighF150InteriorInventoryAmount()
				<< "," << bayTwo.GetBaseExpeditionInteriorInventoryAmount()
				<< "," << bayTwo.GetMidExpeditionInteriorInventoryAmount()
				<< "," << bayTwo.GetHighExpeditionInteriorInventoryAmount() << endl;
			fout1.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : InteriorMachineBay2Inv.txt" << endl;
		}
		return true;
	}
	else
	{
		return false;
	}
}

//switch the inventory inlet into the bodyMachine from the current bay to another valid bay to allow the original bay to be restocked 

bool InteriorMachine::SwitchVehiclePanelsBays(string switchToBay)
{
	//Determine which bay to switch to
	if (switchToBay == "BayOne")
	{
		currentBay = &bayOne;
		bayOne.bayInUse();
		bayTwo.bayNotInUse();
		return true;
	}
	else if (switchToBay == "BayTwo")
	{
		currentBay = &bayTwo;
		bayTwo.bayInUse();
		bayOne.bayNotInUse();
		return true;
	}
	else
	{
		cout << "Invalid Bay" << endl;
		return false;
	}
}

//adjust the inventory counts 

bool InteriorMachine::UpdateBaseInteriorF150InventoryAmount(int setInvLevelTo, string givenBay)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentBay into string
	string currentBayString = "N/A";
	if (currentBay == &bayOne)
	{
		currentBayString = "BayOne";
	}
	else if (currentBay == &bayTwo)
	{
		currentBayString = "BayTwo";
	}

	//cannot restock if bay is in use
	if (givenBay == currentBayString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentBayString << " cannot be restocked while it is in use please switch bays" << endl;
		return false;
	}

	//restock only valid bays
	if (givenBay == "BayOne" || givenBay == "BayTwo")
	{

		//set the correct value
		if (givenBay == "BayOne")
		{
			bayOne.SetBaseF150InteriorInventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetBaseF150InteriorInventoryAmount(setInvLevelTo);
		}

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("InteriorMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) Base Level F150 Interior Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : InteriorMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}

bool InteriorMachine::UpdateMidInteriorF150InventoryAmount(int setInvLevelTo, string givenBay)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentBay into string
	string currentBayString = "N/A";
	if (currentBay == &bayOne)
	{
		currentBayString = "BayOne";
	}
	else if (currentBay == &bayTwo)
	{
		currentBayString = "BayTwo";
	}

	//cannot restock if bay is in use
	if (givenBay == currentBayString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentBayString << " cannot be restocked while it is in use please switch bays" << endl;
		return false;
	}

	//restock only valid bays
	if (givenBay == "BayOne" || givenBay == "BayTwo")
	{

		//set the correct value
		if (givenBay == "BayOne")
		{
			bayOne.SetMidF150InteriorInventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetMidF150InteriorInventoryAmount(setInvLevelTo);
		}

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("InteriorMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) Mid Level F150 Interior Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : InteriorMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}

bool InteriorMachine::UpdateHighInteriorF150InventoryAmount(int setInvLevelTo, string givenBay)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentBay into string
	string currentBayString = "N/A";
	if (currentBay == &bayOne)
	{
		currentBayString = "BayOne";
	}
	else if (currentBay == &bayTwo)
	{
		currentBayString = "BayTwo";
	}

	//cannot restock if bay is in use
	if (givenBay == currentBayString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentBayString << " cannot be restocked while it is in use please switch bays" << endl;
		return false;
	}

	//restock only valid bays
	if (givenBay == "BayOne" || givenBay == "BayTwo")
	{

		//set the correct value
		if (givenBay == "BayOne")
		{
			bayOne.SetHighF150InteriorInventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetHighF150InteriorInventoryAmount(setInvLevelTo);
		}

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("InteriorMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) High Level F150 Interior Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : InteriorMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}

bool InteriorMachine::UpdateBaseInteriorExpeditionInventoryAmount(int setInvLevelTo, string givenBay)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentBay into string
	string currentBayString = "N/A";
	if (currentBay == &bayOne)
	{
		currentBayString = "BayOne";
	}
	else if (currentBay == &bayTwo)
	{
		currentBayString = "BayTwo";
	}

	//cannot restock if bay is in use
	if (givenBay == currentBayString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentBayString << " cannot be restocked while it is in use please switch bays" << endl;
		return false;
	}

	//restock only valid bays
	if (givenBay == "BayOne" || givenBay == "BayTwo")
	{

		//set the correct value
		if (givenBay == "BayOne")
		{
			bayOne.SetBaseExpeditionInteriorInventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetBaseExpeditionInteriorInventoryAmount(setInvLevelTo);
		}

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("InteriorMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) Base Level Expedition Interior Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : InteriorMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}

bool InteriorMachine::UpdateMidInteriorExpeditionInventoryAmount(int setInvLevelTo, string givenBay)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentBay into string
	string currentBayString = "N/A";
	if (currentBay == &bayOne)
	{
		currentBayString = "BayOne";
	}
	else if (currentBay == &bayTwo)
	{
		currentBayString = "BayTwo";
	}

	//cannot restock if bay is in use
	if (givenBay == currentBayString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentBayString << " cannot be restocked while it is in use please switch bays" << endl;
		return false;
	}

	//restock only valid bays
	if (givenBay == "BayOne" || givenBay == "BayTwo")
	{

		//set the correct value
		if (givenBay == "BayOne")
		{
			bayOne.SetMidExpeditionInteriorInventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetMidExpeditionInteriorInventoryAmount(setInvLevelTo);
		}

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("InteriorMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) Mid Level Expedition Interior Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : InteriorMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}

bool InteriorMachine::UpdateHighInteriorExpeditionInventoryAmount(int setInvLevelTo, string givenBay)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentBay into string
	string currentBayString = "N/A";
	if (currentBay == &bayOne)
	{
		currentBayString = "BayOne";
	}
	else if (currentBay == &bayTwo)
	{
		currentBayString = "BayTwo";
	}

	//cannot restock if bay is in use
	if (givenBay == currentBayString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentBayString << " cannot be restocked while it is in use please switch bays" << endl;
		return false;
	}

	//restock only valid bays
	if (givenBay == "BayOne" || givenBay == "BayTwo")
	{

		//set the correct value
		if (givenBay == "BayOne")
		{
			bayOne.SetHighExpeditionInteriorInventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetHighExpeditionInteriorInventoryAmount(setInvLevelTo);
		}

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("InteriorMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) High Level Expedition Interior Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : InteriorMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}


//record activity to the log (will write inventory levels and time to a text file **should be called every time any change is made**) 

bool InteriorMachine::WriteTakenInventoryToLog()
{
	//get current data and time to use in our log
	time_t now = time(0);
	char* dateAndTime = ctime(&now);

	//add entry to log on updated inventory
	ofstream fout;
	fout.open("InteriorMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
	if (fout.is_open())
	{
		fout << "(A) Inventory Level Update @ " << dateAndTime;
		fout << "    Bay 1: BaseF150InteriorInvLvl: " << bayOne.GetBaseF150InteriorInventoryAmount() << " MidF150InteriorInvLvl: " << bayOne.GetMidF150InteriorInventoryAmount() <<
			" HighF150InteriorInvLvl: " << bayOne.GetHighF150InteriorInventoryAmount() << " BaseExpeditionInteriorInvLvl: " << bayOne.GetBaseExpeditionInteriorInventoryAmount() <<
			" MidExpeditionInteriorInvLvl: " << bayOne.GetMidExpeditionInteriorInventoryAmount() << " HighExpeditionInteriorInvLvl: " << bayOne.GetHighExpeditionInteriorInventoryAmount() << endl;
		fout << "    Bay 2: BaseF150InteriorInvLvl: " << bayTwo.GetBaseF150InteriorInventoryAmount() << " MidF150InteriorInvLvl: " << bayTwo.GetMidF150InteriorInventoryAmount() <<
			" HighF150InteriorInvLvl: " << bayTwo.GetHighF150InteriorInventoryAmount() << " BaseExpeditionInteriorInvLvl: " << bayTwo.GetBaseExpeditionInteriorInventoryAmount() <<
			" MidExpeditionInteriorInvLvl: " << bayTwo.GetMidExpeditionInteriorInventoryAmount() << " HighExpeditionInteriorInvLvl: " << bayTwo.GetHighExpeditionInteriorInventoryAmount() << endl;
		fout.close();
	}
	else
	{
		cout << "file cannot be opened!\n" << "Filename : InteriorMachineInventoryLog.txt" << endl;
		return false;
	}

	return true;
}

//set the notification threshold for the amount of inventory 

bool InteriorMachine::SetLowLevelLimit(int givenThreshold)
{
	if (givenThreshold < 0 || givenThreshold > 500)
	{
		return false;
	}
	else
	{
		lowLevelLimit = givenThreshold;
		return true;
	}
}

//read the current threshold for the amount of inventory 

int InteriorMachine::GetLowLevelLimit()
{
	return lowLevelLimit;
}