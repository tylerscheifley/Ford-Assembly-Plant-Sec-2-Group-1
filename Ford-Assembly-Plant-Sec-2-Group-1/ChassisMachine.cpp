//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Chassis Machine File
//This file will control of features of the chassis machine including, tracking inventory of chassis and assigning the correct chassis set to the given vehicles 
//
//November 13th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 13th   Initial

#define _CRT_SECURE_NO_WARNINGS

#include "ChassisMachine.h"

//bodyMachine constructor
ChassisMachine::ChassisMachine()
{
	chassisType = "N/A";
	currentLine = &lineOne;
	lowLevelLimit = 10;
	inventoryLevelsValid = false;
	ChassisLine newLineOne;
	lineOne = newLineOne;
	ChassisLine newLineTwo;
	lineTwo = newLineTwo;
	//read inventory levels in from the bay (the text file)

	//Bay 1-----------------------
	ifstream file("ChassisMachineLine1Inv.txt");
	string number;
	int F15033, F15027, F15050, F15035E, F15035P, Exp35, Exp35HO = 0;
	string line;
	if (file.is_open()) {

		while (getline(file, line)) {
			stringstream ss(line);
			getline(ss, number, ',');
			F15033 = stoi(number);
			getline(ss, number, ',');
			F15027 = stoi(number);
			getline(ss, number, ',');
			F15050 = stoi(number);
			getline(ss, number, ',');
			F15035E = stoi(number);
			getline(ss, number, ',');
			F15035P = stoi(number);
			getline(ss, number, ',');
			Exp35 = stoi(number);
			number = "\0";
			getline(ss, number, '\n');
			if (number != "\0") {
				Exp35HO = stoi(number);
			}
			lineOne.SetF15033LV6CInventoryAmount(F15033);
			lineOne.SetF15027LV6CInventoryAmount(F15027);
			lineOne.SetF15050LV8CInventoryAmount(F15050);
			lineOne.SetF15035LV6EcoCInventoryAmount(F15035E);
			lineOne.SetF15035LV6PwrBstCInventoryAmount(F15035P);
			lineOne.SetExpedition35LV6CInventoryAmount(Exp35);
			lineOne.SetExpedition35LV6HOCInventoryAmount(Exp35HO);

		}
	}
	else {
		cout << "file cannot be opened!\n" << "Filename : ChassisMachineLine1Inv.txt" << endl;
	}

	file.close();

	//Bay 2-----------------

	ifstream file1("ChassisMachineLine2Inv.txt");
	if (file1.is_open()) {

		while (getline(file1, line)) {
			stringstream ss(line);
			getline(ss, number, ',');
			F15033 = stoi(number);
			getline(ss, number, ',');
			F15027 = stoi(number);
			getline(ss, number, ',');
			F15050 = stoi(number);
			getline(ss, number, ',');
			F15035E = stoi(number);
			getline(ss, number, ',');
			F15035P = stoi(number);
			getline(ss, number, ',');
			Exp35 = stoi(number);
			number = "\0";
			getline(ss, number, '\n');
			if (number != "\0") {
				Exp35HO = stoi(number);
			}
			lineTwo.SetF15033LV6CInventoryAmount(F15033);
			lineTwo.SetF15027LV6CInventoryAmount(F15027);
			lineTwo.SetF15050LV8CInventoryAmount(F15050);
			lineTwo.SetF15035LV6EcoCInventoryAmount(F15035E);
			lineTwo.SetF15035LV6PwrBstCInventoryAmount(F15035P);
			lineTwo.SetExpedition35LV6CInventoryAmount(Exp35);
			lineTwo.SetExpedition35LV6HOCInventoryAmount(Exp35HO);

		}
	}
	else {
		cout << "file cannot be opened!\n" << "Filename : ChassisMachineLine2Inv.txt" << endl;
	}

	file1.close();

}

//update line levels in text file
void ChassisMachine::UpdateLineLevelsInFile()
{
	ofstream fout;
	fout.open("ChassisMachineLine1Inv.txt", ios::trunc); //we want to trunc to the file or essentially write over the existing data
	if (fout.is_open())
	{
		fout << lineOne.GetF15033LV6CInventoryAmount()
			<< "," << lineOne.GetF15027LV6CInventoryAmount()
			<< "," << lineOne.GetF15050LV8CInventoryAmount()
			<< "," << lineOne.GetF15035LV6EcoCInventoryAmount()
			<< "," << lineOne.GetF15035LV6PwrBstCInventoryAmount()
			<< "," << lineOne.GetExpedition35LV6CInventoryAmount()
			<< "," << lineOne.GetExpedition35LV6HOCInventoryAmount() << endl;
		fout.close();
	}
	else
	{
		cout << "file cannot be opened!\n" << "Filename : ChassisMachineLine1Inv.txt" << endl;
	}
	ofstream fout1;
	fout1.open("ChassisMachineLine2Inv.txt", ios::trunc); //we want to trunc to the file or essentially write over the existing data
	if (fout1.is_open())
	{
		fout1 << lineTwo.GetF15033LV6CInventoryAmount()
			<< "," << lineTwo.GetF15027LV6CInventoryAmount()
			<< "," << lineTwo.GetF15050LV8CInventoryAmount()
			<< "," << lineTwo.GetF15035LV6EcoCInventoryAmount()
			<< "," << lineTwo.GetF15035LV6PwrBstCInventoryAmount()
			<< "," << lineTwo.GetExpedition35LV6CInventoryAmount()
			<< "," << lineTwo.GetExpedition35LV6HOCInventoryAmount() << endl;
		fout1.close();
	}
	else
	{
		cout << "file cannot be opened!\n" << "Filename : ChassisMachineLine2Inv.txt" << endl;
	}
}

//this method will run the bodymachine where an order and a vehicle outline will be provided
bool ChassisMachine::RunChassisMachine(Order givenOrder, Vehicle* vehiclePlaceHolder)
{
	chassisType = givenOrder.getModel() + givenOrder.getEngineType();

	if (chassisType == "Expedition35LV6C")
	{
		if (currentLine->GetExpedition35LV6CInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO 3.5 L V6 EXPEDITION CHASSIS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (chassisType == "Expedition35LV6HOC")
	{
		if (currentLine->GetExpedition35LV6HOCInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO 3.5 L V6 HIGH OUTPUT EXPEDITION CHASSIS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (chassisType == "F15027LV6C")
	{
		if (currentLine->GetF15027LV6CInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO 2.7 L V6 F150 CHASSIS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (chassisType == "F15033LV6C")
	{
		if (currentLine->GetF15033LV6CInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO 3.3 L V6 F150 CHASSIS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (chassisType == "F15035LV6EcoC")
	{
		if (currentLine->GetF15035LV6EcoCInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO 3.5 L V6 ECOBOOST F150 CHASSIS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (chassisType == "F15035LV6PwrBstC")
	{
		if (currentLine->GetF15035LV6PwrBstCInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO 3.5 L V6 POWERBOOST F150 CHASSIS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}
	else if (chassisType == "F15050LV8C")
	{
		if (currentLine->GetF15050LV8CInventoryAmount() == 0)
		{
			cout << "!!!!!!!!!!!!!!!!!!!!!!!!ATTENTION NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "WARNING! THE ASSEMBLY HAS HALTED WITH NO 5.0 L V8 F150 CHASSIS IN STOCK" << endl;
			inventoryLevelsValid = false;
			return false;
		}
	}


	//check inventory levels for "warnings"

	if (lineOne.GetF15033LV6CInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 3.3 L V6 F150 Chassis inventory level below threshold in line1" << endl;
	}
	else if (lineOne.GetF15027LV6CInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 2.7 L V6 F150 Chassis inventory level below threshold in line1" << endl;
	}
	else if (lineOne.GetF15050LV8CInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 5.0 L V8 F150 Chassis inventory level below threshold in line1" << endl;
	}
	else if (lineOne.GetF15035LV6EcoCInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 3.3 L V6 Ecoboost F150 Chassis inventory level below threshold in line1" << endl;
	}
	else if (lineOne.GetF15035LV6PwrBstCInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 3.5 L V6 Powerboost F150 Chassis inventory level below threshold in line1" << endl;
	}
	else if (lineOne.GetExpedition35LV6CInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 3.5 L V6 Expedition Chassis inventory level below threshold in line1" << endl;
	}
	else if (lineOne.GetExpedition35LV6HOCInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 3.5 L V6 High Output Expedition Chassis inventory level below threshold in line1" << endl;
	}
	else if (lineOne.GetF15033LV6CInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 3.3 L V6 F150 Chassis inventory level below threshold in line2" << endl;
	}
	else if (lineOne.GetF15027LV6CInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 2.7 L V6 F150 Chassis inventory level below threshold in line2" << endl;
	}
	else if (lineOne.GetF15050LV8CInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 5.0 L V8 F150 Chassis inventory level below threshold in line2" << endl;
	}
	else if (lineOne.GetF15035LV6EcoCInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 3.3 L V6 Ecoboost F150 Chassis inventory level below threshold in line2" << endl;
	}
	else if (lineOne.GetF15035LV6PwrBstCInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 3.5 L V6 Powerboost F150 Chassis inventory level below threshold in line2" << endl;
	}
	else if (lineOne.GetExpedition35LV6CInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 3.5 L V6 Expedition Chassis inventory level below threshold in line2" << endl;
	}
	else if (lineOne.GetExpedition35LV6HOCInventoryAmount() < lowLevelLimit)
	{
		cout << "WARNING! 3.5 L V6 High Output Expedition Chassis inventory level below threshold in line2" << endl;
	}
	else // Inventory Levels are good across the board
	{
		inventoryLevelsValid = true;
	}

	//update the vehicle to have the right body
	vehiclePlaceHolder->setChassis(chassisType);

	bool didAdd = false;;

	//update inventory levels
	if (vehiclePlaceHolder->getChassis() == "Expedition35LV6C")
	{
		currentLine->SetExpedition35LV6CInventoryAmount((currentLine->GetExpedition35LV6CInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->getChassis() == "Expedition35LV6HOC")
	{
		currentLine->SetExpedition35LV6HOCInventoryAmount((currentLine->GetExpedition35LV6HOCInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->getChassis() == "F15027LV6C")
	{
		currentLine->SetF15027LV6CInventoryAmount((currentLine->GetF15027LV6CInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->getChassis() == "F15033LV6C")
	{
		currentLine->SetF15033LV6CInventoryAmount((currentLine->GetF15033LV6CInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->getChassis() == "F15035LV6EcoC")
	{
		currentLine->SetF15035LV6EcoCInventoryAmount((currentLine->GetF15035LV6EcoCInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->getChassis() == "F15035LV6PwrBstC")
	{
		currentLine->SetF15035LV6PwrBstCInventoryAmount((currentLine->GetF15035LV6PwrBstCInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}
	else if (vehiclePlaceHolder->getChassis() == "F15050LV8C")
	{
		currentLine->SetF15050LV8CInventoryAmount((currentLine->GetF15050LV8CInventoryAmount() - 1));
		WriteTakenInventoryToLog();
		didAdd = true;
	}

	//Only save the inventory in each line if the inventory was changed
	if (didAdd == true)
	{
		//update line levels in text file
		void UpdateLineLevelsInFile();
		return true;
	}
	else
	{
		return false;
	}
}

//switch the inventory inlet into the chassisMachine from the current line to another valid line to allow the original line to be restocked 

bool ChassisMachine::SwitchVehicleChassisLines(string switchToLine)
{
	//Determine which line to switch to
	if (switchToLine == "LineOne")
	{
		currentLine = &lineOne;
		lineOne.lineInUse();
		lineTwo.lineNotInUse();
		return true;
	}
	else if (switchToLine == "LineTwo")
	{
		currentLine = &lineTwo;
		lineTwo.lineInUse();
		lineOne.lineNotInUse();
		return true;
	}
	else
	{
		cout << "Invalid Line" << endl;
		return false;
	}
}

//adjust the inventory counts 

bool ChassisMachine::UpdateF15033LV6CInventoryAmount(int setInvLevelTo, string givenLine)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentLine into string
	string currentLineString = "N/A";
	if (currentLine == &lineOne)
	{
		currentLineString = "LineOne";
	}
	else if (currentLine == &lineTwo)
	{
		currentLineString = "LineTwo";
	}

	//cannot restock if line is in use
	if (givenLine == currentLineString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentLineString << " cannot be restocked while it is in use please switch lines" << endl;
		return false;
	}

	//restock only valid lines
	if (givenLine == "LineOne" || givenLine == "LineTwo")
	{

		//set the correct value
		if (givenLine == "LineOne")
		{
			lineOne.SetF15033LV6CInventoryAmount(setInvLevelTo);
		}
		else if (givenLine == "LineTwo")
		{
			lineTwo.SetF15033LV6CInventoryAmount(setInvLevelTo);
		}

		//update line levels in text file
		void UpdateLineLevelsInFile();

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("ChassisMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) F150 3.3L V6 Chassis Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenLine << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid line selected so inv not updated 
	return false;
}

bool ChassisMachine::UpdateF15027LV6CInventoryAmount(int setInvLevelTo, string givenLine)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentLine into string
	string currentLineString = "N/A";
	if (currentLine == &lineOne)
	{
		currentLineString = "LineOne";
	}
	else if (currentLine == &lineTwo)
	{
		currentLineString = "LineTwo";
	}

	//cannot restock if line is in use
	if (givenLine == currentLineString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentLineString << " cannot be restocked while it is in use please switch lines" << endl;
		return false;
	}

	//restock only valid lines
	if (givenLine == "LineOne" || givenLine == "LineTwo")
	{

		//set the correct value
		if (givenLine == "LineOne")
		{
			lineOne.SetF15027LV6CInventoryAmount(setInvLevelTo);
		}
		else if (givenLine == "LineTwo")
		{
			lineTwo.SetF15027LV6CInventoryAmount(setInvLevelTo);
		}

		//update line levels in text file
		void UpdateLineLevelsInFile();

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("ChassisMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) F150 2.7L Ecoboost V6 Chassis Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenLine << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid line selected so inv not updated 
	return false;
}

bool ChassisMachine::UpdateF15050LV8CInventoryAmount(int setInvLevelTo, string givenLine)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentLine into string
	string currentLineString = "N/A";
	if (currentLine == &lineOne)
	{
		currentLineString = "LineOne";
	}
	else if (currentLine == &lineTwo)
	{
		currentLineString = "LineTwo";
	}

	//cannot restock if line is in use
	if (givenLine == currentLineString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentLineString << " cannot be restocked while it is in use please switch lines" << endl;
		return false;
	}

	//restock only valid lines
	if (givenLine == "LineOne" || givenLine == "LineTwo")
	{

		//set the correct value
		if (givenLine == "LineOne")
		{
			lineOne.SetF15050LV8CInventoryAmount(setInvLevelTo);
		}
		else if (givenLine == "LineTwo")
		{
			lineTwo.SetF15050LV8CInventoryAmount(setInvLevelTo);
		}

		//update line levels in text file
		void UpdateLineLevelsInFile();

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("ChassisMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) F150 5.0L V8 Chassis Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenLine << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid line selected so inv not updated 
	return false;
}

bool ChassisMachine::UpdateF15035LV6EcoCInventoryAmount(int setInvLevelTo, string givenLine)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentLine into string
	string currentLineString = "N/A";
	if (currentLine == &lineOne)
	{
		currentLineString = "LineOne";
	}
	else if (currentLine == &lineTwo)
	{
		currentLineString = "LineTwo";
	}

	//cannot restock if line is in use
	if (givenLine == currentLineString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentLineString << " cannot be restocked while it is in use please switch lines" << endl;
		return false;
	}

	//restock only valid lines
	if (givenLine == "LineOne" || givenLine == "LineTwo")
	{

		//set the correct value
		if (givenLine == "LineOne")
		{
			lineOne.SetF15035LV6EcoCInventoryAmount(setInvLevelTo);
		}
		else if (givenLine == "LineTwo")
		{
			lineTwo.SetF15035LV6EcoCInventoryAmount(setInvLevelTo);
		}

		//update line levels in text file
		void UpdateLineLevelsInFile();

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("ChassisMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) F150 3.5L Ecoboost V6 Chassis Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenLine << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid line selected so inv not updated 
	return false;
}

bool ChassisMachine::UpdateF15035LV6PwrBstCInventoryAmount(int setInvLevelTo, string givenLine)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentLine into string
	string currentLineString = "N/A";
	if (currentLine == &lineOne)
	{
		currentLineString = "LineOne";
	}
	else if (currentLine == &lineTwo)
	{
		currentLineString = "LineTwo";
	}

	//cannot restock if line is in use
	if (givenLine == currentLineString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentLineString << " cannot be restocked while it is in use please switch lines" << endl;
		return false;
	}

	//restock only valid lines
	if (givenLine == "LineOne" || givenLine == "LineTwo")
	{

		//set the correct value
		if (givenLine == "LineOne")
		{
			lineOne.SetF15035LV6PwrBstCInventoryAmount(setInvLevelTo);
		}
		else if (givenLine == "LineTwo")
		{
			lineTwo.SetF15035LV6PwrBstCInventoryAmount(setInvLevelTo);
		}

		//update line levels in text file
		void UpdateLineLevelsInFile();

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("ChassisMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) F150 3.5L Powerboost V6 Chassis Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenLine << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid line selected so inv not updated 
	return false;
}

bool ChassisMachine::UpdateExpedition35LV6CInventoryAmount(int setInvLevelTo, string givenLine)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentLine into string
	string currentLineString = "N/A";
	if (currentLine == &lineOne)
	{
		currentLineString = "LineOne";
	}
	else if (currentLine == &lineTwo)
	{
		currentLineString = "LineTwo";
	}

	//cannot restock if line is in use
	if (givenLine == currentLineString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentLineString << " cannot be restocked while it is in use please switch lines" << endl;
		return false;
	}

	//restock only valid lines
	if (givenLine == "LineOne" || givenLine == "LineTwo")
	{

		//set the correct value
		if (givenLine == "LineOne")
		{
			lineOne.SetExpedition35LV6CInventoryAmount(setInvLevelTo);
		}
		else if (givenLine == "LineTwo")
		{
			lineTwo.SetExpedition35LV6CInventoryAmount(setInvLevelTo);
		}

		//update line levels in text file
		void UpdateLineLevelsInFile();

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("ChassisMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) Expedition 3.5L Ecoboost V6 Chassis Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenLine << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid line selected so inv not updated 
	return false;
}

bool ChassisMachine::UpdateExpedition35LV6HOCInventoryAmount(int setInvLevelTo, string givenLine)
{
	//only update the information if it is 100% valid
	if (setInvLevelTo < 0 || setInvLevelTo > 500)
	{
		return false;
	}

	//convert currentLine into string
	string currentLineString = "N/A";
	if (currentLine == &lineOne)
	{
		currentLineString = "LineOne";
	}
	else if (currentLine == &lineTwo)
	{
		currentLineString = "LineTwo";
	}

	//cannot restock if line is in use
	if (givenLine == currentLineString)
	{
		cout << "WARNING Inventory Levels NOT Updated! " << currentLineString << " cannot be restocked while it is in use please switch lines" << endl;
		return false;
	}

	//restock only valid lines
	if (givenLine == "LineOne" || givenLine == "LineTwo")
	{

		//set the correct value
		if (givenLine == "LineOne")
		{
			lineOne.SetExpedition35LV6HOCInventoryAmount(setInvLevelTo);
		}
		else if (givenLine == "LineTwo")
		{
			lineTwo.SetExpedition35LV6HOCInventoryAmount(setInvLevelTo);
		}

		//update line levels in text file
		void UpdateLineLevelsInFile();

		//get current data and time to use in our log
		time_t now = time(0);
		char* dateAndTime = ctime(&now);

		//add entry to log on updated inventory
		ofstream fout;
		fout.open("ChassisMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
		if (fout.is_open())
		{
			fout << "(M) Expedition 3.5L Ecoboost High Output V6 Chassis Inventory Level Amount Updated To: " << setInvLevelTo << " In: " << givenLine << " @ " << dateAndTime;
			fout.close();
		}
		else
		{
			cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
		}

		return true;
	}
	//there wasnt a valid line selected so inv not updated 
	return false;
}

//record activity to the log (will write inventory levels and time to a text file **should be called every time any change is made**) 

bool ChassisMachine::WriteTakenInventoryToLog()
{
	//get current data and time to use in our log
	time_t now = time(0);
	char* dateAndTime = ctime(&now);

	//add entry to log on updated inventory
	ofstream fout;
	fout.open("ChassisMachineInventoryLog.txt", ios::app); //we want to append to the file or essentially add a line to the file
	if (fout.is_open())
	{
		fout << "(A) Inventory Level Update @ " << dateAndTime;
		fout << "    Line 1: F15033LV6CInvLvl: " << lineOne.GetF15033LV6CInventoryAmount() << " F15027LV6CInvLvl: " << lineOne.GetF15027LV6CInventoryAmount() <<
			" F15050LV8CInvLvl: " << lineOne.GetF15050LV8CInventoryAmount() << " F15035LV6EcoCInvLvl: " << lineOne.GetF15035LV6EcoCInventoryAmount() <<
			" F15035LV6PwrBstCInvLvl: " << lineOne.GetF15035LV6PwrBstCInventoryAmount() << " Expedition35LV6CInvLvl: " << lineOne.GetExpedition35LV6CInventoryAmount() <<
			" Expedition35LV6HOCInvLvl: " << lineOne.GetExpedition35LV6HOCInventoryAmount() << endl;
		fout << "    Line 2: F15033LV6CInvLvl: " << lineTwo.GetF15033LV6CInventoryAmount() << " F15027LV6CInvLvl: " << lineTwo.GetF15027LV6CInventoryAmount() <<
			" F15050LV8CInvLvl: " << lineTwo.GetF15050LV8CInventoryAmount() << " F15035LV6EcoCInvLvl: " << lineTwo.GetF15035LV6EcoCInventoryAmount() <<
			" F15035LV6PwrBstCInvLvl: " << lineTwo.GetF15035LV6PwrBstCInventoryAmount() << " Expedition35LV6CInvLvl: " << lineTwo.GetExpedition35LV6CInventoryAmount() <<
			" Expedition35LV6HOCInvLvl: " << lineTwo.GetExpedition35LV6HOCInventoryAmount() << endl;
		fout.close();
	}
	else
	{
		cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
		return false;
	}

	return true;
}

//set the notification threshold for the amount of inventory 

bool ChassisMachine::SetLowLevelLimit(int givenThreshold)
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

int ChassisMachine::GetLowLevelLimit()
{
	return lowLevelLimit;
}

//read the current chassis type that is working in this machine

string ChassisMachine::GetChassisType()
{
	return chassisType;
}