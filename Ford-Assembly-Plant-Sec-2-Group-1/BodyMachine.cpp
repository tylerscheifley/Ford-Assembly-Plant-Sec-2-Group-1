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

#define _CRT_SECURE_NO_WARNINGS

#include "BodyMachine.h"

//bodyMachine constructor
BodyMachine::BodyMachine()
{
	bodyType = "N/A";
	currentBay = &bayOne;
	lowLevelLimit = 10;
	inventoryLevelsValid = false;
	VehiclePanelsBay newBay;
	bayOne = newBay;
	VehiclePanelsBay newBay1;
	bayTwo = newBay1;
	//read inventory levels in from the bay (the text file)

	//Bay 1-----------------------
	ifstream file("BodyMachineBay1Inv.txt");
	string number;
	int regF150, SprCaF150, SprCrF150, regExp, MaxExp = 0;
	string line;
	if (file.is_open()) {

		while (getline(file, line)) {
			stringstream ss(line);
			getline(ss, number, ',');
			regF150 = stoi(number);
			getline(ss, number, ',');
			SprCaF150 = stoi(number);
			getline(ss, number, ',');
			SprCrF150 = stoi(number);
			getline(ss, number, ',');
			regExp = stoi(number);
			number = "\0";
			getline(ss, number, '\n');
			if (number != "\0") {
				MaxExp = stoi(number);
			}
			bayOne.SetRegularF150InventoryAmount(regF150);
			bayOne.SetSuperCabF150InventoryAmount(SprCaF150);
			bayOne.SetSuperCrewF150InventoryAmount(SprCrF150);
			bayOne.SetRegularExpeditionInventoryAmount(regExp);
			bayOne.SetMaxExpeditionInventoryAmount(MaxExp);

		}
	}
	else {
		cout << "file cannot be opened!\n" << "Filename : BodyMachineBay1Inv.txt" << endl;
	}

	file.close();

	//Bay 2-----------------

	ifstream file1("BodyMachineBay2Inv.txt");
	if (file1.is_open()) {

		while (getline(file1, line)) {
			stringstream ss(line);
			getline(ss, number, ',');
			regF150 = stoi(number);
			getline(ss, number, ',');
			SprCaF150 = stoi(number);
			getline(ss, number, ',');
			SprCrF150 = stoi(number);
			getline(ss, number, ',');
			regExp = stoi(number);
			number = "\0";
			getline(ss, number, '\n');
			if (number != "\0") {
				MaxExp = stoi(number);
			}
			bayTwo.SetRegularF150InventoryAmount(regF150);
			bayTwo.SetSuperCabF150InventoryAmount(SprCaF150);
			bayTwo.SetSuperCrewF150InventoryAmount(SprCrF150);
			bayTwo.SetRegularExpeditionInventoryAmount(regExp);
			bayTwo.SetMaxExpeditionInventoryAmount(MaxExp);

		}
	}
	else {
		cout << "file cannot be opened!\n" << "Filename : BodyMachineBay2Inv.txt" << endl;
	}

	file1.close();

}

//this method will run the bodymachine where an order and a vehicle outline will be provided
bool BodyMachine::RunBodyMachine(Order givenOrder, Vehicle* vehiclePlaceHolder)
{
	bodyType = givenOrder.bodyPanelSet + givenOrder.model;

	if (bodyType == "RegularF150")
	{
		if (currentBay->GetRegularF150InventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO REGULAR F150 BODY PANEL SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (bodyType == "SuperCabF150")
	{
		if (currentBay->GetSuperCabF150InventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO REGULAR F150 BODY PANEL SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (bodyType == "SuperCrewF150")
	{
		if (currentBay->GetSuperCrewF150InventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO REGULAR F150 BODY PANEL SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (bodyType == "RegularExpedition")
	{
		if (currentBay->GetRegularExpeditionInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO REGULAR F150 BODY PANEL SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (bodyType == "MaxExpedition")
	{
		if (currentBay->GetMaxExpeditionInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO REGULAR F150 BODY PANEL SETS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	

	//check inventory levels for "warnings"
	if (bayOne.GetRegularF150InventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Regular F150 body panel sets inventory below threshold in bay1" << endl;
	}
	else if(bayTwo.GetRegularF150InventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Regular F150 body panel sets inventory below threshold in bay2" << endl;

	}
	else if (bayOne.GetSuperCabF150InventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! SuperCab F150 body panel sets inventory below threshold in bay1" << endl;
	}
	else if (bayTwo.GetSuperCabF150InventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! SuperCab F150 body panel sets inventory below threshold in bay2" << endl;

	}
	else if (bayOne.GetSuperCrewF150InventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! SuperCrew F150 body panel sets inventory below threshold in bay1" << endl;

	}
	else if (bayTwo.GetSuperCrewF150InventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! SuperCrew F150 body panel sets inventory below threshold in bay2" << endl;

	}
	else if (bayOne.GetRegularExpeditionInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Regular Expedition body panel sets inventory below threshold in bay1" << endl;

	}
	else if (bayTwo.GetRegularExpeditionInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Regular Expedition body panel sets inventory below threshold in bay2" << endl;

	}
	else if (bayOne.GetMaxExpeditionInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Max Expedition body panel sets inventory below threshold in bay1" << endl;

	}
	else if (bayTwo.GetMaxExpeditionInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! Max Expedition body panel sets inventory below threshold in bay2" << endl;

	}
	else // Inventory Levels are good across the board
	{
		inventoryLevelsValid = true;
	}
	
	//update the vehicle to have the right body
	vehiclePlaceHolder->setBody(bodyType);

	bool didAdd = false;

	//update inventory levels
	if (vehiclePlaceHolder->body == "RegularF150")
	{
		currentBay->SetRegularF150InventoryAmount((currentBay->GetRegularF150InventoryAmount() - 1 ));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->body == "SuperCabF150")
	{
		currentBay->SetSuperCabF150InventoryAmount((currentBay->GetSuperCabF150InventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->body == "SuperCrewF150")
	{
		currentBay->SetSuperCrewF150InventoryAmount((currentBay->GetSuperCrewF150InventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->body == "RegularExpedition")
	{
		currentBay->SetRegularExpeditionInventoryAmount((currentBay->GetRegularExpeditionInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->body == "MaxExpedition")
	{
		currentBay->SetMaxExpeditionInventoryAmount((currentBay->GetMaxExpeditionInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	//Only save the inventory in each bay if the inventory was changed
	if (didAdd == true)
	{
		ofstream fout;
		fout.open("BodyMachineBay1Inv.txt", ios::trunc); //we want to trunc to the file or essentially write over the existing data
		if (fout.is_open())
		{
			fout << bayOne.GetRegularF150InventoryAmount()
				<< "," << bayOne.GetSuperCabF150InventoryAmount()
				<< "," << bayOne.GetSuperCrewF150InventoryAmount()
				<< "," << bayOne.GetRegularExpeditionInventoryAmount()
				<< "," << bayOne.GetMaxExpeditionInventoryAmount() << endl;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : BodyMachineBay1Inv.txt" << endl;
		}
		ofstream fout1;
		fout1.open("BodyMachineBay2Inv.txt", ios::trunc); //we want to trunc to the file or essentially write over the existing data
		if (fout1.is_open())
		{
			fout1 << bayTwo.GetRegularF150InventoryAmount()
				<< "," << bayTwo.GetSuperCabF150InventoryAmount()
				<< "," << bayTwo.GetSuperCrewF150InventoryAmount()
				<< "," << bayTwo.GetRegularExpeditionInventoryAmount()
				<< "," << bayTwo.GetMaxExpeditionInventoryAmount() << endl;
			fout1.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : BodyMachineBay2Inv.txt" << endl;
		}
		return true;
	}
	else
	{
		return false;
	}
}

//switch the inventory inlet into the bodyMachine from the current bay to another valid bay to allow the original bay to be restocked 

bool BodyMachine::SwitchVehiclePanelsBays(string switchToBay)
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

bool BodyMachine::UpdateRegularF150InventoryAmount(int setInvLevelTo, string givenBay)
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
			bayOne.SetRegularF150InventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetRegularF150InventoryAmount(setInvLevelTo);
		}
		
		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("BodyMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) Regular F150 Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : BodyMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}

bool BodyMachine::UpdateSuperCabF150InventoryAmount(int setInvLevelTo, string givenBay)
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
			bayOne.SetSuperCabF150InventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetSuperCabF150InventoryAmount(setInvLevelTo);
		}

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("BodyMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) SuperCab F150 Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : BodyMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}

bool BodyMachine::UpdateSuperCrewF150InventoryAmount(int setInvLevelTo, string givenBay)
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
			bayOne.SetSuperCrewF150InventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetSuperCrewF150InventoryAmount(setInvLevelTo);
		}

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("BodyMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) SuperCrew F150 Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : BodyMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}

bool BodyMachine::UpdateRegularExpeditionInventoryAmount(int setInvLevelTo, string givenBay)
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
			bayOne.SetRegularExpeditionInventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetRegularExpeditionInventoryAmount(setInvLevelTo);
		}

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("BodyMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) Regular Expedition Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : BodyMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}

bool BodyMachine::UpdateMaxExpeditionInventoryAmount(int setInvLevelTo, string givenBay)
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
			bayOne.SetMaxExpeditionInventoryAmount(setInvLevelTo);
		}
		else if (givenBay == "BayTwo")
		{
			bayTwo.SetMaxExpeditionInventoryAmount(setInvLevelTo);
		}

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("BodyMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) Max Expedition Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenBay << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : BodyMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid bay selected so inv not updated 
	return false;
}

//record activity to the log (will write inventory levels and time to a text file **should be called every time any change is made**) 

bool BodyMachine::WriteTakenInventoryToLog()
{
	//get current data and time to use in our log
	time_t now = time(0);
	char* dateAndTime = ctime(&now);

	//add entry to log on updated inventory
	ofstream fout;
	fout.open("BodyMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
	if (fout.is_open())
	{
		fout << "(A) Inventory Level Update @ " << dateAndTime;
		fout << "    Bay 1: RegF150InvLvl: " << bayOne.GetRegularF150InventoryAmount() << " SprCaF150InvLvl: " << bayOne.GetSuperCabF150InventoryAmount() <<
			" SprCrF150InvLvl: " << bayOne.GetSuperCrewF150InventoryAmount() << " RegExpInvLvl: " << bayOne.GetRegularExpeditionInventoryAmount() <<
			" MaxExpInvLvl: " << bayOne.GetMaxExpeditionInventoryAmount() << endl;
		fout << "    Bay 2: RegF150InvLvl: " << bayTwo.GetRegularF150InventoryAmount() << " SprCaF150InvLvl: " << bayTwo.GetSuperCabF150InventoryAmount() <<
			" SprCrF150InvLvl: " << bayTwo.GetSuperCrewF150InventoryAmount() << " RegExpInvLvl: " << bayTwo.GetRegularExpeditionInventoryAmount() <<
			" MaxExpInvLvl: " << bayTwo.GetMaxExpeditionInventoryAmount() << endl;
		fout.close();
	}
	else
	{
		cout << "file cannot be opened!\n" << "Filename : BodyMachineInventoryLog.txt" << endl;
		return false;
	}

	return true;
}

//set the notification threshold for the amount of inventory 

bool BodyMachine::SetLowLevelLimit(int givenThreshold)
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

int BodyMachine::GetLowLevelLimit()
{
	return lowLevelLimit;
}