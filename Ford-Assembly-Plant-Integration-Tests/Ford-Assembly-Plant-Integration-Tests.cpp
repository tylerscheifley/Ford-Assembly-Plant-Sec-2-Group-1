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

			//This information will normally be handeled in the bodyMachine and this information
			//is not relevant to this test it will just ensure the environment is setup correctly

			newBodyMachine.SwitchVehiclePanelsBays("BayTwo");

			newBodyMachine.UpdateSuperCrewF150InventoryAmount(100, "BayOne");

			newBodyMachine.SwitchVehiclePanelsBays("BayOne");



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
			newBodyMachine.bayOne.bayInUse(); //This means that the opposite bay should have recieved 



			//Check to see if the bodyMachine can receive a false flag from bay two
			//even though it was not directly modified. This means that the body machine connected
			//to bayOne and then bayOne connected to bayTwo
			Assert::IsFalse(newBodyMachine.bayTwo.bayThisLineInUse());
		}

		TEST_METHOD(BodyMachineAndCurrentBay_SwitchBays_BaysSwitch)
		{

		}

		TEST_METHOD(BodyMachineAndTextLog_WriteStringsToLog_DoesLogReceiveStrings)
		{

		}

		TEST_METHOD(BodyMachineAndTextLog_ReadStringFromLog_BodyMachineGetsLogStrings)
		{

		}

		TEST_METHOD(BodyMachineAndBay_IntializeBayWithConstructor_BayIsCreated)
		{

		}

		TEST_METHOD(ChassisMachineAndVehicle_UpdateEngineType_VehicleReceivesChassis)
		{

		}

		TEST_METHOD(ChassisMachineAndOrder_ReadOrder_OrderInfoRecieved)
		{

		}

		TEST_METHOD(ChassisMachineAndCurrentLine_InventoryLevel_LineReceivesUpdateInvAmount)
		{

		}

		TEST_METHOD(ChassisMachineAndCurrentLine_InventoryLevel_ChassisMachineRecievesInvLevel)
		{

		}

		TEST_METHOD(ChassisMachineAndCurrentLine_LineInUse_ChassisMachineReceivesTrueFlag)
		{

		}

		TEST_METHOD(ChassisMachineAndCurrentLine_LineNotInUse_LineReceivesFalseFlag)
		{

		}

		TEST_METHOD(ChassisMachineAndCurrentLine_SwitchLines_LinesSwitch)
		{

		}

		TEST_METHOD(ChassisMachineAndTextLog_WriteStringsToLog_DoesLogReceiveStrings)
		{

		}

		TEST_METHOD(ChassisMachineAndTextLog_ReadStringFromLog_ChassisMachineGetsLogStrings)
		{

		}

		TEST_METHOD(ChassisMachineAndLine_IntializeLineWithConstructor_LineIsCreated)
		{

		}

		TEST_METHOD(InteriorMachineAndVehicle_UpdateInteriorLevel_VehicleReceivesInterior)
		{

		}

		TEST_METHOD(InteriorMachineAndOrder_ReadOrder_OrderInfoReceived)
		{

		}

		TEST_METHOD(InteriorMachineAndCurrentBay_InventoryLevel_BayReceivesUpdateInvAmount)
		{

		}

		TEST_METHOD(InteriorMachineAndCurrentBay_InventoryLevel_InteriorMachineRecievesInvLevel)
		{

		}

		TEST_METHOD(InteriorMachineAndCurrentBay_BayInUse_InteriorMachineReceivesTrueFlag)
		{

		}

		TEST_METHOD(InteriorMachineAndCurrentBay_BayNotInUse_BayReceivesFalseFlag)
		{

		}

		TEST_METHOD(InteriorMachineAndCurrentBay_SwitchBays_BaysSwitch)
		{

		}

		TEST_METHOD(InteriorMachineAndTextLog_WriteStringsToLog_DoesLogReceiveStrings)
		{

		}

		TEST_METHOD(InteriorMachineAndTextLog_ReadStringFromLog_InteriorMachineGetsLogStrings)
		{

		}

		TEST_METHOD(InteriorMachineAndBay_IntializeBayWithConstructor_BayIsCreated)
		{

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
			PaintMachine paintMachine(&dryingChamber,&paintChamber,&dipTank);
			
			paintMachine.identifyRGBvalues("PlantColours.txt");

			Assert::IsTrue(expectedRvalue == paintMachine.getRED() && expectedGvalue == paintMachine.getGREEN() && expectedBvalue == paintMachine.getBLUE());
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