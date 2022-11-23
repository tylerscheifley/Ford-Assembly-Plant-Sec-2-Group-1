#include "pch.h"
#include "CppUnitTest.h"
#include "InteriorMachine.h"
#include "ChassisMachine.h"
#include "BodyMachine.h"
#include "Vehicle.h"
#include "Order.h"
#include "Plant.h"
#include "PaintMachine.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FordAssemblyPlantIntegrationTests
{
	TEST_CLASS(FordAssemblyPlantIntegrationTests)
	{
	public:

		TEST_METHOD(BodyMachineAndVehicle_UpdateBodyPanelSet_VehicleReceivesBodyPanelSet)
		{
			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			//This information will normally be handeled in the order object however we will set this test
			//up to simulate how this connection would begin
			placedOrder.setBodyPanelSet("SuperCrew");
			placedOrder.setModel("F150");
			vehiclePlaceholder.setBody("N/A");



			//This call will then connect the bodyMachine with the vehicle object and if this connetion
			//is successfull then the vehicles body varaible will be updated

			newBodyMachine.RunBodyMachine(placedOrder, &vehiclePlaceholder);



			//Now we confirm that the vehicle received the body from the bodyMachine

			string expectedBody = "SuperCrewF150";

			Assert::AreEqual(expectedBody, vehiclePlaceholder.getBody());
		}

		TEST_METHOD(BodyMachineAndOrder_ReadOrder_OrderInfoReceived)
		{
			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			//This information will normally be handeled in the order object however we will set this test
			//up to simulate how this connection would begin
			placedOrder.setBodyPanelSet("Max");
			placedOrder.setModel("Expedition");
			vehiclePlaceholder.setBody("N/A"); //this call is irrelevant to this test case it will just allow the method to be called



			//This call will then connect the bodyMachine with the order object and if this connection
			//is successfull then the body machine should receive info on what body to add to the vehicle

			newBodyMachine.RunBodyMachine(placedOrder, &vehiclePlaceholder);



			//Now we confirm that the bodyMachine received the body type info from the order

			string expectedBody = "MaxExpedition";

			Assert::AreEqual(expectedBody, newBodyMachine.GetBodyType());
		}

		TEST_METHOD(BodyMachineAndCurrentBay_InventoryLevel_BayReceivesUpdateInvAmount)
		{
//!!!!!!FAILED AT FIRST BECAUSE THE "BayTwo" was entered as "bayTwo"
			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			

			//This call will then connect the bodyMachine with the bay object and if this connection
			//is successfull than the bay will receive the inventory level 100
			newBodyMachine.UpdateMaxExpeditionInventoryAmount(100, "BayTwo");



			//Now we confirm that the bodyMachine sent the inventory level to the bay object
			Assert::AreEqual(100, newBodyMachine.bayTwo.GetMaxExpeditionInventoryAmount());
		}

		TEST_METHOD(BodyMachineAndCurrentBay_InventoryLevel_BodyMachineRecievesInvLevel)
		{
//!!!!!!FAILED AT FIRST BECAUSE THE "BayOne" was entered as "bayOne"
			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test
			// this call is irrelevant to the test but needs
			//to be in place so that an inventory level can be read
			newBodyMachine.SwitchVehiclePanelsBays("BayTwo");
			newBodyMachine.UpdateRegularExpeditionInventoryAmount(100, "BayOne");
			newBodyMachine.SwitchVehiclePanelsBays("BayOne");



			//Now we confirm that the bodyMachine sent the inventory level to the bay object
			Assert::AreEqual(100, newBodyMachine.bayOne.GetRegularExpeditionInventoryAmount());
		}

		TEST_METHOD(BodyMachineAndCurrentBay_BayInUse_BodyMachineReceivesTrueFlag)
		{
			//This is not needed for updating a bay it is just for use in this test this will normally be handeled in the plant object
			BodyMachine newBodyMachine;

		

			//Set this bay to in use
			newBodyMachine.bayOne.bayInUse();



			//Check to see if the bodyMachine can receive a true flag from bay one
			Assert::IsTrue(newBodyMachine.bayOne.bayThisLineInUse());
		}

		TEST_METHOD(BodyMachineAndCurrentBay_BayNotInUse_BayReceivesFalseFlag)
		{
			//This is not needed for updating a bay it is just for use in this test this will normally be handeled in the plant object
			BodyMachine newBodyMachine;
			//Set this bay to in use
			newBodyMachine.bayOne.bayInUse(); //This means that the opposite bay should have received false for in use value



			//Check to see if the bodyMachine can receive a false flag from bay two
			//even though it was not directly modified. This means that the body machine connected
			//to bayOne and then bayOne connected to bayTwo
			Assert::IsFalse(newBodyMachine.bayTwo.bayThisLineInUse());
		}

		TEST_METHOD(BodyMachineAndCurrentBay_SwitchBays_BaysSwitch)
		{
			//This is not needed for switching bays it is just for use in this test this will normally be handeled in the plant object
			BodyMachine newBodyMachine;
			newBodyMachine.bayOne.bayInUse();
			newBodyMachine.bayTwo.bayNotInUse();


			//This will make bayOne set to true and bayTwo set to false
			newBodyMachine.SwitchVehiclePanelsBays("BayTwo");



			//Check to see if both bays received the message to update their in use statuses
			Assert::IsFalse(newBodyMachine.bayOne.bayThisLineInUse());
			Assert::IsTrue(newBodyMachine.bayTwo.bayThisLineInUse());
		}

		TEST_METHOD(BodyMachineAndTextLog_WriteStringsToLog_DoesLogReceiveStrings)
		{
//FAILED FIRST TIME AS SENTENCE IN LOG OUTPUTTED DIFFERENTLY THAN EXPECTED - THIS IS TEST CASE TABLE ERROR
			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test



			newBodyMachine.SwitchVehiclePanelsBays("BayTwo");
			newBodyMachine.UpdateRegularExpeditionInventoryAmount(100, "BayOne");
			newBodyMachine.SwitchVehiclePanelsBays("BayOne");



			//check if the log was added to the file
			ifstream file("BodyMachineInventoryLog.txt");
			string line, substring, result;
			if (file.is_open()) {

				while (!file.eof()) {
					getline(file, line);
					substring = line.substr(0, 72);
					if (substring == "(M) Regular Expedition Inventory Level Amount Updated To: 100 In: BayOne")
					{
						result = substring;
					}
				}
			}
			else {
				cout << "file cannot be opened!\n" << "Filename : BodyMachineInventoryLog.txt" << endl;
			}
			file.close();
			string expectedResult = "(M) Regular Expedition Inventory Level Amount Updated To: 100 In: BayOne";
			Assert::AreEqual(expectedResult, result);
		}

		TEST_METHOD(BodyMachineAndTextLog_ReadStringFromLog_BodyMachineGetsLogStrings)
		{
			//NOTE IN project folder x64\Debug\BodyMachineBay1Inv.txt File Values must be set to 500,500,500,500,99
			//NOTE IN project folder x64\Debug\BodyMachineBay2Inv.txt File Values must be set to 500,500,500,9,500
			//^^THIS WILL MEAN THAT WE MUST SET THESE VALUES IN THE TEXT FILES
			ofstream fout;
			fout.open("BodyMachineBay1Inv.txt", ios::trunc); //we want to trunc to the file or essentially write over the existing data
			if (fout.is_open())
			{
				fout << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "99";
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
				fout1 << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "9"
					<< "," << "500";
				fout1.close();
			}
			else
			{
				cout << "file cannot be opened!\n" << "Filename : BodyMachineBay1Inv.txt" << endl;
			}


			//This constructor should read from the bay inventory level files and store them in the correct varaibles
			BodyMachine newBodyMachine;



			Assert::AreEqual(99, newBodyMachine.bayOne.GetMaxExpeditionInventoryAmount());
			Assert::AreEqual(500, newBodyMachine.bayOne.GetRegularExpeditionInventoryAmount());
			Assert::AreEqual(500, newBodyMachine.bayOne.GetRegularF150InventoryAmount());
			Assert::AreEqual(500, newBodyMachine.bayOne.GetSuperCabF150InventoryAmount());
			Assert::AreEqual(500, newBodyMachine.bayOne.GetSuperCrewF150InventoryAmount());
			Assert::AreEqual(500, newBodyMachine.bayTwo.GetMaxExpeditionInventoryAmount());
			Assert::AreEqual(9, newBodyMachine.bayTwo.GetRegularExpeditionInventoryAmount());
			Assert::AreEqual(500, newBodyMachine.bayTwo.GetRegularF150InventoryAmount());
			Assert::AreEqual(500, newBodyMachine.bayTwo.GetSuperCabF150InventoryAmount());
			Assert::AreEqual(500, newBodyMachine.bayTwo.GetSuperCrewF150InventoryAmount());
		}

		TEST_METHOD(BodyMachineAndBay_IntializeBayWithConstructor_BayIsCreated)
		{
			//This should call the bay constructors and we should get 2 bays
			BodyMachine newBodyMachine;

			

			//If the bays and bodyMachine are integrated correctly we just should
			//be able to access the variables in the bays and they should be both false
			//as this is defualt that the bay constructor sets (this will indicate if 
			//the construcor was called
			Assert::IsFalse(newBodyMachine.bayOne.bayThisLineInUse());
			Assert::IsFalse(newBodyMachine.bayTwo.bayThisLineInUse());
		}

		TEST_METHOD(ChassisMachineAndVehicle_UpdateEngineType_VehicleReceivesChassis)
		{
			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test
			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;
			//This information will normally be handeled in the order object however we will set this test
			//up to simulate how this connection would begin
			placedOrder.setEngineType("50LV8C");
			placedOrder.setModel("F150");
			vehiclePlaceholder.setChassis("N/A");



			//This call will then connect the chassisMachine with the vehicle object and if this connetion
			//is successfull then the vehicles chassis varaible will be updated
			newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);



			//Now we confirm that the vehicle received the chassis from the chassisMachine
			string expectedBody = "F15050LV8C";
			Assert::AreEqual(expectedBody, vehiclePlaceholder.getChassis());
		}

		TEST_METHOD(ChassisMachineAndOrder_ReadOrder_OrderInfoRecieved)
		{
			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test
			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;
			//This information will normally be handeled in the order object however we will set this test
			//up to simulate how this connection would begin
			placedOrder.setEngineType("35LV6C");
			placedOrder.setModel("Expedition");
			vehiclePlaceholder.setChassis("N/A"); //this call is irrelevant to this test case it will just allow the method to be called



			//This call will then connect the chassisMachine with the order object and if this connection
			//is successfull then the chassis machine should receive info on what chassis to add to the vehicle
			newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);



			//Now we confirm that the chassisMachine received the chassis type info from the order
			string expectedChassis = "Expedition35LV6C";
			Assert::AreEqual(expectedChassis, newChassisMachine.GetChassisType());
		}

		TEST_METHOD(ChassisMachineAndCurrentLine_InventoryLevel_LineReceivesUpdateInvAmount)
		{
//!!!!!!FAILED AT FIRST BECAUSE THE "LineOne" was entered as "lineOne"
			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test



			//This call will then connect the chassisMachine with the line object and if this connection
			//is successfull than the line will receive the inventory level 100
			newChassisMachine.UpdateExpedition35LV6CInventoryAmount(100, "LineTwo");



			//Now we confirm that the bodyMachine sent the inventory level to the bay object
			Assert::AreEqual(100, newChassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount());
		}

		TEST_METHOD(ChassisMachineAndCurrentLine_InventoryLevel_ChassisMachineRecievesInvLevel)
		{
//!!!!!!FAILED AT FIRST BECAUSE THE "LineOne" was entered as "lineOne"
			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test
			// this call is irrelevant to the test but needs
			//to be in place so that an inventory level can be read
			newChassisMachine.SwitchVehicleChassisLines("LineTwo");
			newChassisMachine.UpdateExpedition35LV6CInventoryAmount(100, "LineOne");
			newChassisMachine.SwitchVehicleChassisLines("LineOne");



			//Now we confirm that the bodyMachine sent the inventory level to the bay object
			Assert::AreEqual(100, newChassisMachine.lineOne.GetExpedition35LV6CInventoryAmount());
		}

		TEST_METHOD(ChassisMachineAndCurrentLine_LineInUse_ChassisMachineReceivesTrueFlag)
		{
			//This is not needed for updating a line it is just for use in this test this will normally be handeled in the plant object
			ChassisMachine newChassisMachine;



			//Set this line to in use
			newChassisMachine.lineOne.lineInUse();



			//Check to see if the chassisMachine can receive a true flag from line one
			Assert::IsTrue(newChassisMachine.lineOne.isThisLineInUse());
		}

		TEST_METHOD(ChassisMachineAndCurrentLine_LineNotInUse_LineReceivesFalseFlag)
		{
			//This is not needed for updating a line it is just for use in this test this will normally be handeled in the plant object
			ChassisMachine newChassisMachine;
			//Set this bay to in use
			newChassisMachine.lineOne.lineInUse(); //This means that the opposite line should have received false for the in use value 



			//Check to see if the chassisMachine can receive a false flag from line two
			//even though it was not directly modified. This means that the chassis machine connected
			//to lineOne and then lineOne connected to lineTwo
			Assert::IsFalse(newChassisMachine.lineTwo.isThisLineInUse());
		}

		TEST_METHOD(ChassisMachineAndCurrentLine_SwitchLines_LinesSwitch)
		{
			//This is not needed for switching lines it is just for use in this test this will normally be handeled in the plant object
			ChassisMachine newChassisMachine;
			newChassisMachine.lineOne.lineInUse();
			newChassisMachine.lineTwo.lineNotInUse();


			//This will make lineOne set to true and lineTwo set to false
			newChassisMachine.SwitchVehicleChassisLines("LineTwo");



			//Check to see if both lines received the message to update their in use statuses
			Assert::IsTrue(newChassisMachine.lineTwo.isThisLineInUse());
			Assert::IsFalse(newChassisMachine.lineOne.isThisLineInUse());
		}

		TEST_METHOD(ChassisMachineAndTextLog_WriteStringsToLog_DoesLogReceiveStrings)
		{
//FAILED FIRST TIME AS SENTENCE IN LOG OUTPUTTED DIFFERENTLY THAN EXPECTED - THIS IS TEST CASE TABLE ERROR
			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test



			newChassisMachine.SwitchVehicleChassisLines("LineTwo");
			newChassisMachine.UpdateExpedition35LV6CInventoryAmount(100, "BayOne");
			newChassisMachine.SwitchVehicleChassisLines("LineOne");



			//check if the log was added to the file
			ifstream file("ChassisMachineInventoryLog.txt");
			string line, substring, result;
			if (file.is_open()) {

				while (!file.eof()) {
					getline(file, line);
					substring = line.substr(0, 90);
					if (substring == "(M) Expedition 3.5L Ecoboost V6 Chassis Inventory Level Amount Updated To: 100 In: LineOne")
					{
						result = substring;
					}
				}
			}
			else {
				cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
			}
			file.close();
			string expectedResult = "(M) Expedition 3.5L Ecoboost V6 Chassis Inventory Level Amount Updated To: 100 In: LineOne";
			Assert::AreEqual(expectedResult, result);
		}

		TEST_METHOD(ChassisMachineAndTextLog_ReadStringFromLog_ChassisMachineGetsLogStrings)
		{
			//NOTE IN project folder x64\Debug\ChassisMachineLine1Inv.txt File Values must be set to 500,500,500,500,500,500,99
			//NOTE IN project folder x64\Debug\ChassisMachineLine2Inv.txt File Values must be set to 500,500,500,500,500,9,500
			//^^THIS WILL MEAN THAT WE MUST SET THESE VALUES IN THE TEXT FILES
			ofstream fout;
			fout.open("ChassisMachineLine1Inv.txt", ios::trunc); //we want to trunc to the file or essentially write over the existing data
			if (fout.is_open())
			{
				fout << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "99";
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
				fout1 << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "9"
					<< "," << "500";
				fout1.close();
			}
			else
			{
				cout << "file cannot be opened!\n" << "Filename : ChassisMachineLine2Inv.txt" << endl;
			}


			//This constructor should read from the line inventory level files and store them in the correct variables
			ChassisMachine newChassisMachine;



			Assert::AreEqual(99, newChassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineOne.GetExpedition35LV6CInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineOne.GetF15027LV6CInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineOne.GetF15033LV6CInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineOne.GetF15050LV8CInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineTwo.GetExpedition35LV6HOCInventoryAmount());
			Assert::AreEqual(9, newChassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineTwo.GetF15027LV6CInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineTwo.GetF15033LV6CInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineTwo.GetF15035LV6EcoCInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineTwo.GetF15035LV6PwrBstCInventoryAmount());
			Assert::AreEqual(500, newChassisMachine.lineTwo.GetF15050LV8CInventoryAmount());
		}

		TEST_METHOD(ChassisMachineAndLine_IntializeLineWithConstructor_LineIsCreated)
		{
			//This should call the line constructors and we should get 2 lines
			ChassisMachine newChassisMachine;



			//If the lines and chassisMachine are integrated correctly we just should
			//be able to access the variables in the lines and they should be both false
			//as this is defualt that the line constructor sets (this will indicate if 
			//the constructor was called
			Assert::IsFalse(newChassisMachine.lineOne.isThisLineInUse());
			Assert::IsFalse(newChassisMachine.lineTwo.isThisLineInUse());
		}

		TEST_METHOD(InteriorMachineAndVehicle_UpdateInteriorLevel_VehicleReceivesInterior)
		{
//THIS FAILED FIRST RUN AS THE TEST TABLE WAS LOOKING FOR "F150Base" but the result was "BaseF150"
			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			//This information will normally be handeled in the order object however we will set this test
			//up to simulate how this connection would begin
			placedOrder.setInteriorLevel("Base");
			placedOrder.setModel("F150");
			vehiclePlaceholder.setInterior("N/A");



			//This call will then connect the interiorMachine with the vehicle object and if this connetion
			//is successfull then the vehicles intetior varaible will be updated

			newInteriorMachine.RunInteriorMachine(placedOrder, &vehiclePlaceholder);



			//Now we confirm that the vehicle received the interior from the interiorMachine

			string expectedBody = "BaseF150";

			Assert::AreEqual(expectedBody, vehiclePlaceholder.getInterior());
		}

		TEST_METHOD(InteriorMachineAndOrder_ReadOrder_OrderInfoReceived)
		{
			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			//This information will normally be handeled in the order object however we will set this test
			//up to simulate how this connection would begin
			placedOrder.setInteriorLevel("High");
			placedOrder.setModel("Expedition");
			vehiclePlaceholder.setInterior("N/A"); //this call is irrelevant to this test case it will just allow the method to be called



			//This call will then connect the interiorMachine with the order object and if this connection
			//is successfull then the interior machine should receive info on what interior level to add to the vehicle

			newInteriorMachine.RunInteriorMachine(placedOrder, &vehiclePlaceholder);



			//Now we confirm that the interiorMachine received the interior level info from the order

			string expectedBody = "HighExpedition";

			Assert::AreEqual(expectedBody, newInteriorMachine.GetInteriorType());
		}

		TEST_METHOD(InteriorMachineAndCurrentBay_InventoryLevel_BayReceivesUpdateInvAmount)
		{
//!!!!!!FAILED AT FIRST BECAUSE THE "BayTwo" was entered as "bayTwo"
			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test



			//This call will then connect the interiorMachine with the bay object and if this connection
			//is successfull than the bay will receive the inventory level 100
			newInteriorMachine.UpdateHighInteriorExpeditionInventoryAmount(100, "BayTwo");



			//Now we confirm that the interiorMachine sent the inventory level to the bay object
			Assert::AreEqual(100, newInteriorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount());
		}

		TEST_METHOD(InteriorMachineAndCurrentBay_InventoryLevel_InteriorMachineRecievesInvLevel)
		{
//!!!!!!FAILED AT FIRST BECAUSE THE "BayOne" was entered as "bayOne"
			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test
			// this call is irrelevant to the test but needs
			//to be in place so that an inventory level can be read
			newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");
			newInteriorMachine.UpdateBaseInteriorExpeditionInventoryAmount(100, "BayOne");
			newInteriorMachine.SwitchVehiclePanelsBays("BayOne");



			//Now we confirm that the interiorMachine sent the inventory level to the bay object
			Assert::AreEqual(100, newInteriorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount());
		}

		TEST_METHOD(InteriorMachineAndCurrentBay_BayInUse_InteriorMachineReceivesTrueFlag)
		{
			//This is not needed for updating a bay it is just for use in this test this will normally be handeled in the plant object
			InteriorMachine newInteriorMachine;



			//Set this bay to in use
			newInteriorMachine.bayOne.bayInUse();



			//Check to see if the interiorMachine can receive a true flag from bay one
			Assert::IsTrue(newInteriorMachine.bayOne.bayThisLineInUse());
		}

		TEST_METHOD(InteriorMachineAndCurrentBay_BayNotInUse_BayReceivesFalseFlag)
		{
			//This is not needed for updating a bay it is just for use in this test this will normally be handeled in the plant object
			InteriorMachine newInteriorMachine;
			//Set this bay to in use
			newInteriorMachine.bayOne.bayInUse(); //This means that the opposite bay should have received false for in use value



			//Check to see if the interiorMachine can receive a false flag from bay two
			//even though it was not directly modified. This means that the interior machine connected
			//to bayOne and then bayOne connected to bayTwo
			Assert::IsFalse(newInteriorMachine.bayTwo.bayThisLineInUse());
		}

		TEST_METHOD(InteriorMachineAndCurrentBay_SwitchBays_BaysSwitch)
		{
			//This is not needed for switching bays it is just for use in this test this will normally be handeled in the plant object
			InteriorMachine newInteriorMachine;
			newInteriorMachine.bayOne.bayInUse();
			newInteriorMachine.bayTwo.bayNotInUse();


			//This will make bayOne set to true and bayTwo set to false
			newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");



			//Check to see if both bays received the message to update their in use statuses
			Assert::IsFalse(newInteriorMachine.bayOne.bayThisLineInUse());
			Assert::IsTrue(newInteriorMachine.bayTwo.bayThisLineInUse());
		}

		TEST_METHOD(InteriorMachineAndTextLog_WriteStringsToLog_DoesLogReceiveStrings)
		{
//FAILED FIRST TIME AS SENTENCE IN LOG OUTPUTTED DIFFERENTLY THAN EXPECTED - THIS IS TEST CASE TABLE ERROR
			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test



			newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");
			newInteriorMachine.UpdateBaseInteriorExpeditionInventoryAmount(100, "BayOne");
			newInteriorMachine.SwitchVehiclePanelsBays("BayOne");



			//check if the log was added to the file
			ifstream file("InteriorMachineInventoryLog.txt");
			string line, substring, result;
			if (file.is_open()) {

				while (!file.eof()) {
					getline(file, line);
					substring = line.substr(0, 84);
					if (substring == "(M) Base Level Expedition Interior Inventory Level Amount Updated To: 100 In: BayOne")
					{
						result = substring;
					}
				}
			}
			else {
				cout << "file cannot be opened!\n" << "Filename : InteriorMachineInventoryLog.txt" << endl;
			}
			file.close();
			string expectedResult = "(M) Base Level Expedition Interior Inventory Level Amount Updated To: 100 In: BayOne";
			Assert::AreEqual(expectedResult, result);
		}

		TEST_METHOD(InteriorMachineAndTextLog_ReadStringFromLog_InteriorMachineGetsLogStrings)
		{
			//NOTE IN project folder x64\Debug\InteriorMachineBay1Inv.txt File Values must be set to 500,500,500,500,500,99
			//NOTE IN project folder x64\Debug\InteriorMachineBay2Inv.txt File Values must be set to 500,500,500,500,9,500
			//^^THIS WILL MEAN THAT WE MUST SET THESE VALUES IN THE TEXT FILES
			ofstream fout;
			fout.open("InteriorMachineBay1Inv.txt", ios::trunc); //we want to trunc to the file or essentially write over the existing data
			if (fout.is_open())
			{
				fout << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "99";
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
				fout1 << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "500"
					<< "," << "9"
					<< "," << "500";
				fout1.close();
			}
			else
			{
				cout << "file cannot be opened!\n" << "Filename : InteriorMachineBay1Inv.txt" << endl;
			}


			//This constructor should read from the bay inventory level files and store them in the correct variables
			InteriorMachine newInteriorMachine;



			Assert::AreEqual(99, newInteriorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount());
			Assert::AreEqual(500, newInteriorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount());
			Assert::AreEqual(500, newInteriorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount());
			Assert::AreEqual(500, newInteriorMachine.bayOne.GetBaseF150InteriorInventoryAmount());
			Assert::AreEqual(500, newInteriorMachine.bayOne.GetMidF150InteriorInventoryAmount());
			Assert::AreEqual(500, newInteriorMachine.bayOne.GetHighF150InteriorInventoryAmount());
			Assert::AreEqual(500, newInteriorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount());
			Assert::AreEqual(500, newInteriorMachine.bayTwo.GetBaseExpeditionInteriorInventoryAmount());
			Assert::AreEqual(9, newInteriorMachine.bayTwo.GetMidExpeditionInteriorInventoryAmount());
			Assert::AreEqual(500, newInteriorMachine.bayTwo.GetBaseF150InteriorInventoryAmount());
			Assert::AreEqual(500, newInteriorMachine.bayTwo.GetMidF150InteriorInventoryAmount());
			Assert::AreEqual(500, newInteriorMachine.bayTwo.GetHighF150InteriorInventoryAmount());
		}

		TEST_METHOD(InteriorMachineAndBay_IntializeBayWithConstructor_BayIsCreated)
		{
			//This should call the line constructors and we should get 2 lines
			InteriorMachine newInteriorMachine;



			//If the lines and chassisMachine are integrated correctly we just should
			//be able to access the variables in the lines and they should be both false
			//as this is defualt that the line constructor sets (this will indicate if 
			//the constructor was called
			Assert::IsFalse(newInteriorMachine.bayOne.bayThisLineInUse());
			Assert::IsFalse(newInteriorMachine.bayTwo.bayThisLineInUse());
		}


		TEST_METHOD(PaintMachine_Read_RGB_Values_INT_TEST)
		{
			// Testing race red from testing file 
			int expectedRvalue = 6;
			int expectedGvalue = 0;
			int expectedBvalue = 0;
			DipTank dipTank;
			DryingChamber dryingChamber;
			PaintChamber paintChamber;
			PaintMachine paintMachine(&dryingChamber, &paintChamber, &dipTank);

			paintMachine.setcolour("Race Red");
			paintMachine.identifyRGBvalues("PlantColours.txt");


			Assert::AreEqual(expectedRvalue, paintMachine.getRED());
		}

		TEST_METHOD(PaintMachine_Read_RGB_Paint_Volume_INT_TEST)
		{

		}

		TEST_METHOD(PaintMachine_Update_RGB_Paint_Volume_INT_TEST)
		{

		}

		TEST_METHOD(PaintMachine_DipTank_Read_Temperature_INT_TEST)
		{

		}

		TEST_METHOD(PaintMachine_DipTank_Read_FluidLevel_INT_TEST)
		{

		}

		TEST_METHOD(PaintMachine_PaintChamber_Read_Temperature_INT_TEST)
		{

		}

		TEST_METHOD(PaintMachine_PaintChamber_Read_Humidity_INT_TEST)
		{

		}

		TEST_METHOD(PaintMachine_Order_Pass_Colour_INT_TEST)
		{

		}

		TEST_METHOD(PaintMachine_Initialize_DipTank_INT_TEST)
		{

		}

		TEST_METHOD(PaintMachine_Initialize_DryingChamber_INT_TEST)
		{

		}

		TEST_METHOD(PaintMachine_Initialize_PaintChamber_INT_TEST)
		{

		}
	};
}