//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Chassis Machine Unit Tests
//
//November 13th, 2022
//
//Brodin Collins-Robb

//Revision History
//
//1.0   Nov. 13th   Initial

#include "pch.h"
#include "CppUnitTest.h"
#include "ChassisMachine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ChassisMachineTests
{
	TEST_CLASS(ChassisMachineTests)
	{
	public:

		TEST_METHOD(F15050LV8C_Build50LV8CF150_50LV8CAddedAndInventoryCountDown1)
		{
			//test to see if the chassis machine can pull and document the correct inventory for the correct given vehicle 

			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.engineType = "50LV8C";
			placedOrder.model = "F150";
			vehiclePlaceholder.chassis = "N/A";

			newChassisMachine.UpdateF15050LV8CInventoryAmount(100, "lineOne");

			newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct chassis type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedChassis = "F15050LV8C";

			Assert::AreEqual(expectedChassis, vehiclePlaceholder.chassis);
			Assert::AreEqual(99, newChassisMachine.lineOne.GetF15050LV8CInventoryAmount());

		}
		TEST_METHOD(F15035LV6PwrBstC_Build35LV6PwrBstCF150_35LV6PwrBstCAddedAndInventoryCountDown1)
		{
			//test to see if the chassis machine can pull and document the correct inventory for the correct given vehicle 

			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.engineType = "35LV6PwrBstC";
			placedOrder.model = "F150";
			vehiclePlaceholder.chassis = "N/A";

			newChassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(100, "lineOne");

			newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct chassis type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedChassis = "F15035LV6PwrBstC";

			Assert::AreEqual(expectedChassis, vehiclePlaceholder.chassis);
			Assert::AreEqual(99, newChassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount());

		}
		TEST_METHOD(F15035LV6EcoC_Build35LV6EcoCF150_35LV6EcoCAddedAndInventoryCountDown1)
		{
			//test to see if the chassis machine can pull and document the correct inventory for the correct given vehicle 

			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.engineType = "35LV6EcoC";
			placedOrder.model = "F150";
			vehiclePlaceholder.chassis = "N/A";

			newChassisMachine.UpdateF15035LV6EcoCInventoryAmount(100, "lineOne");

			newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct chassis type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedChassis = "F15035LV6EcoC";

			Assert::AreEqual(expectedChassis, vehiclePlaceholder.chassis);
			Assert::AreEqual(99, newChassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount());

		}
		TEST_METHOD(F15033LV6CInventory_Build33LV6CF150_33LV6CAddedAndInventoryCountDown1)
		{
			//test to see if the chassis machine can pull and document the correct inventory for the correct given vehicle 

			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.engineType = "33LV6C";
			placedOrder.model = "F150";
			vehiclePlaceholder.chassis = "N/A";

			newChassisMachine.UpdateF15033LV6CInventoryAmount(100, "lineOne");

			newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct chassis type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedChassis = "F15033LV6C";

			Assert::AreEqual(expectedChassis, vehiclePlaceholder.chassis);
			Assert::AreEqual(99, newChassisMachine.lineOne.GetF15033LV6CInventoryAmount());
		}
		TEST_METHOD(F15027LV6CInventory_Build27LV6CF150_27LV6CAddedAndInventoryCountDown1)
		{
			//test to see if the chassis machine can pull and document the correct inventory for the correct given vehicle 

			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.engineType = "27LV6C";
			placedOrder.model = "F150";
			vehiclePlaceholder.chassis = "N/A";

			newChassisMachine.UpdateF15027LV6CInventoryAmount(100, "lineOne");

			newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct chassis type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedChassis = "F15027LV6C";

			Assert::AreEqual(expectedChassis, vehiclePlaceholder.chassis);
			Assert::AreEqual(99, newChassisMachine.lineOne.GetF15027LV6CInventoryAmount());
		}
		TEST_METHOD(Expedition35LV6HOCInventory_Build35LV6HOCExpedition_35LV6HOCAddedAndInventoryCountDown1)
		{
			//test to see if the chassis machine can pull and document the correct inventory for the correct given vehicle 

			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.engineType = "35LV6HOC";
			placedOrder.model = "Expedition";
			vehiclePlaceholder.chassis = "N/A";

			newChassisMachine.UpdateExpedition35LV6HOCInventoryAmount(100, "lineOne");

			newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct chassis type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedChassis = "Expedition35LV6HOC";

			Assert::AreEqual(expectedChassis, vehiclePlaceholder.chassis);
			Assert::AreEqual(99, newChassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount());
		}
		TEST_METHOD(Expedition35LV6CInventory_BuildExpedition35LV6C_35LV6CAddedAndInventoryCountDown1)
		{
			//test to see if the chassis machine can pull and document the correct inventory for the correct given vehicle 

			ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.engineType = "35LV6C";
			placedOrder.model = "Expedition";
			vehiclePlaceholder.chassis = "N/A";

			newChassisMachine.UpdateExpedition35LV6CInventoryAmount(100, "lineOne");

			newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct chassis type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedChassis = "Expedition35LV6C";

			Assert::AreEqual(expectedChassis, vehiclePlaceholder.chassis);
			Assert::AreEqual(99, newChassisMachine.lineOne.GetExpedition35LV6CInventoryAmount());
		}
		TEST_METHOD(InventoryThreshold_BuildExpeditionMax_InventoryLevelBelowThreshold)
		{
			//Test to see if the ChassisMachine adeqately handles the inventory level low level threshold

			ChassisMachine newChassisMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.engineType = "35LV6HOC ";
			placedOrder.model = "Expedition";
			vehiclePlaceholder.chassis = "N/A";

			newChassisMachine.UpdateExpedition35LV6HOCInventoryAmount(10, "LineOne"); //default low level limit is 10

			newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the machine noticed an inventory level being too low
			Assert::AreEqual(false, newChassisMachine.inventoryLevelsValid);
		}
		TEST_METHOD(SwitchBays_SwitchToBay2_Bay2Selected)
		{
			//Test to ensure that the machine can switch bays

			ChassisMachine newChassisMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

			bool didSwitch = newChassisMachine.SwitchVehicleChassisLines("LineTwo");

			Assert::AreEqual(true, didSwitch);
		}
	};
}