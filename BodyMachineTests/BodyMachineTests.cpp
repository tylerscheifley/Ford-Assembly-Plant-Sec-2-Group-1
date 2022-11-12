//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Body Machine Unit Tests
//
//November 11th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 11th   Initial

#include "pch.h"
#include "CppUnitTest.h"
#include "BodyMachine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BodyMachineTests
{
	TEST_CLASS(BodyMachineTests)
	{
	public:
		
		TEST_METHOD(F150RegularBodyInventory_BuildRegularF150_BodyAddedAndInventoryCountDown1)
		{
			//test to see if the body machine can pull and document the correct inventory for the correct given vehicle 

			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.bodyPanelSet = "Regular";
			placedOrder.model = "F150";
			vehiclePlaceholder.body = "N/A";

			newBodyMachine.UpdateRegularF150InventoryAmount(100, "BayOne");

			newBodyMachine.RunBodyMachine(placedOrder, vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited
			
			string expectedBody = "F150Regular";

			Assert::AreEqual(expectedBody,vehiclePlaceholder.body);
			Assert::AreEqual(99, newBodyMachine.bayOne.GetRegularF150InventoryAmount());

		}
		TEST_METHOD(F150SuperCabBodyInventory_BuildSuperCabF150_BodyAddedAndInventoryCountDown1)
		{
			//test to see if the body machine can pull and document the correct inventory for the correct given vehicle 

			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.bodyPanelSet = "SuperCab";
			placedOrder.model = "F150";
			vehiclePlaceholder.body = "N/A";

			newBodyMachine.UpdateSuperCabF150InventoryAmount(100, "BayOne");

			newBodyMachine.RunBodyMachine(placedOrder, vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedBody = "F150SuperCab";

			Assert::AreEqual(expectedBody, vehiclePlaceholder.body);
			Assert::AreEqual(99, newBodyMachine.bayOne.GetSuperCabF150InventoryAmount());
		}
		TEST_METHOD(F150SuperCrewBodyInventory_BuildSuperCrewF150_BodyAddedAndInventoryCountDown1)
		{
			//test to see if the body machine can pull and document the correct inventory for the correct given vehicle 

			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.bodyPanelSet = "SuperCrew";
			placedOrder.model = "F150";
			vehiclePlaceholder.body = "N/A";

			newBodyMachine.UpdateSuperCrewF150InventoryAmount(100, "BayOne");

			newBodyMachine.RunBodyMachine(placedOrder, vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedBody = "F150SuperCrew";

			Assert::AreEqual(expectedBody, vehiclePlaceholder.body);
			Assert::AreEqual(99, newBodyMachine.bayOne.GetSuperCrewF150InventoryAmount());
		}
		TEST_METHOD(ExpeditionRegularBodyInventory_BuildRegularExpedition_BodyAddedAndInventoryCountDown1)
		{
			//test to see if the body machine can pull and document the correct inventory for the correct given vehicle 

			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.bodyPanelSet = "Regular";
			placedOrder.model = "Expedition";
			vehiclePlaceholder.body = "N/A";

			newBodyMachine.UpdateRegularExpeditionInventoryAmount(100, "BayOne");

			newBodyMachine.RunBodyMachine(placedOrder, vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedBody = "EXPRegular";

			Assert::AreEqual(expectedBody, vehiclePlaceholder.body);
			Assert::AreEqual(99, newBodyMachine.bayOne.GetRegularExpeditionInventoryAmount());
		}
		TEST_METHOD(ExpeditionMaxBodyInventory_BuildExpeditionMax_BodyAddedAndInventoryCountDown1)
		{
			//test to see if the body machine can pull and document the correct inventory for the correct given vehicle 

			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.bodyPanelSet = "Max";
			placedOrder.model = "Expedition";
			vehiclePlaceholder.body = "N/A";

			newBodyMachine.UpdateMaxExpeditionInventoryAmount(100, "BayOne");

			newBodyMachine.RunBodyMachine(placedOrder, vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedBody = "EXPMax";

			Assert::AreEqual(expectedBody, vehiclePlaceholder.body);
			Assert::AreEqual(99, newBodyMachine.bayOne.GetMaxExpeditionInventoryAmount());
		}
		TEST_METHOD(InventoryThreshold_BuildExpeditionMax_InventoryLevelBelowThreshold)
		{
			//Test to see if the bodyMachine adeqately handles the inventory level low level threshold

			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.bodyPanelSet = "Max";
			placedOrder.model = "Expedition";
			vehiclePlaceholder.body = "N/A";

			newBodyMachine.UpdateMaxExpeditionInventoryAmount(10, "BayOne"); //default low level limit is 10

			newBodyMachine.RunBodyMachine(placedOrder, vehiclePlaceholder);

			//confirm that the machine noticed an inventory level being too low
			Assert::AreEqual(false, newBodyMachine.inventoryLevelsValid);
		}
		TEST_METHOD(SwitchBays_SwitchToBay2_Bay2Selected)
		{
			//Test to ensure that the machine can switch bays

			BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			bool didSwitch = newBodyMachine.SwitchVehiclePanelsBays("BayTwo");

			Assert::AreEqual(true, didSwitch);
		}
	};
}
