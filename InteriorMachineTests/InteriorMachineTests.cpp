#include "pch.h"
#include "CppUnitTest.h"
#include "InteriorMachine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InteriorMachineTests
{
	TEST_CLASS(InteriorMachineTests)
	{
	public:
		
		TEST_METHOD(F150BaseInteriorInventory_BuildBaseInteriorF150_InventoryLevelAddedAndInventoryCountDown1)
		{
			//test to see if the inventory machine can pull and document the correct inventory for the correct given vehicle 

			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.interiorLevel = "base";
			placedOrder.model = "F150";
			vehiclePlaceholder.interior = "N/A";

			newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");

			newInteriorMachine.UpdateBaseInteriorF150InventoryAmount(100, "BayOne"); 

			newInteriorMachine.SwitchVehiclePanelsBays("BayOne");

			newInteriorMachine.RunInteriorMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedInterior = "baseF150";

			Assert::AreEqual(expectedInterior, vehiclePlaceholder.interior);
			Assert::AreEqual(99, newInteriorMachine.bayOne.GetBaseF150InteriorInventoryAmount());

		}
		TEST_METHOD(F150MidInteriorInventory_BuildMidInteriorF150_InventoryLevelAddedAndInventoryCountDown1)
		{
			//test to see if the inventory machine can pull and document the correct inventory for the correct given vehicle 

			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.interiorLevel = "mid";
			placedOrder.model = "F150";
			vehiclePlaceholder.interior = "N/A";

			newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");

			newInteriorMachine.UpdateMidInteriorF150InventoryAmount(100, "BayOne"); 

			newInteriorMachine.SwitchVehiclePanelsBays("BayOne");

			newInteriorMachine.RunInteriorMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedInterior = "midF150";

			Assert::AreEqual(expectedInterior, vehiclePlaceholder.interior);
			Assert::AreEqual(99, newInteriorMachine.bayOne.GetMidF150InteriorInventoryAmount());
		}
		TEST_METHOD(F150HighInteriorInventory_BuildHighInteriorF150_InventoryLevelAddedAndInventoryCountDown1)
		{
			//test to see if the inventory machine can pull and document the correct inventory for the correct given vehicle 

			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.interiorLevel = "high";
			placedOrder.model = "F150";
			vehiclePlaceholder.interior = "N/A";

			newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");

			newInteriorMachine.UpdateHighInteriorF150InventoryAmount(100, "BayOne"); 

			newInteriorMachine.SwitchVehiclePanelsBays("BayOne");

			newInteriorMachine.RunInteriorMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedInterior = "highF150";

			Assert::AreEqual(expectedInterior, vehiclePlaceholder.interior);
			Assert::AreEqual(99, newInteriorMachine.bayOne.GetHighF150InteriorInventoryAmount());
		}
		TEST_METHOD(ExpeditionBaseInteriorInventory_BuildBaseInteriorExpedition_InventoryLevelAddedAndInventoryCountDown1)
		{
			//test to see if the inventory machine can pull and document the correct inventory for the correct given vehicle 

			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.interiorLevel = "base";
			placedOrder.model = "Expedition";
			vehiclePlaceholder.interior = "N/A";

			newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");

			newInteriorMachine.UpdateBaseInteriorExpeditionInventoryAmount(100, "BayOne"); 

			newInteriorMachine.SwitchVehiclePanelsBays("BayOne");

			newInteriorMachine.RunInteriorMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedInterior = "baseExpedition";

			Assert::AreEqual(expectedInterior, vehiclePlaceholder.interior);
			Assert::AreEqual(99, newInteriorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount());
		}
		TEST_METHOD(ExpeditionMidInteriorInventory_BuildBaseInteriorExpedition_InventoryLevelAddedAndInventoryCountDown1)
		{
			//test to see if the inventory machine can pull and document the correct inventory for the correct given vehicle 

			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.interiorLevel = "mid";
			placedOrder.model = "Expedition";
			vehiclePlaceholder.interior = "N/A";

			newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");

			newInteriorMachine.UpdateMidInteriorExpeditionInventoryAmount(100, "BayOne"); 

			newInteriorMachine.SwitchVehiclePanelsBays("BayOne");

			newInteriorMachine.RunInteriorMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedInterior = "midExpedition";

			Assert::AreEqual(expectedInterior, vehiclePlaceholder.interior);
			Assert::AreEqual(99, newInteriorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount());
		}
		TEST_METHOD(ExpeditionHighInteriorInventory_BuildHighInteriorExpedition_InventoryLevelAddedAndInventoryCountDown1)
		{
			//test to see if the inventory machine can pull and document the correct inventory for the correct given vehicle 

			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.interiorLevel = "high";
			placedOrder.model = "Expedition";
			vehiclePlaceholder.interior = "N/A";

			newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");

			newInteriorMachine.UpdateHighInteriorExpeditionInventoryAmount(100, "BayOne"); 

			newInteriorMachine.SwitchVehiclePanelsBays("BayOne");

			newInteriorMachine.RunInteriorMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the correct body type was added to the correct spot (The vehicle)
			//also confirm that the correct inventory levels and amount was edited

			string expectedInterior = "highExpedition";

			Assert::AreEqual(expectedInterior, vehiclePlaceholder.interior);
			Assert::AreEqual(99, newInteriorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount());
		}
		TEST_METHOD(InventoryThreshold_BuildExpeditionHighInterior_InventoryLevelBelowThreshold)
		{
			//Test to see if the interiorMachine adeqately handles the inventory level low level threshold

			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

			//This information will normally be handeled in the plant object
			Order placedOrder;
			Vehicle vehiclePlaceholder;

			placedOrder.interiorLevel = "high";
			placedOrder.model = "Expedition";
			vehiclePlaceholder.interior = "N/A";

			newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");

			newInteriorMachine.UpdateHighInteriorExpeditionInventoryAmount(9, "BayOne"); //default low level limit is 10

			newInteriorMachine.SwitchVehiclePanelsBays("BayOne");

			newInteriorMachine.RunInteriorMachine(placedOrder, &vehiclePlaceholder);

			//confirm that the machine noticed an inventory level being too low
			Assert::AreEqual(false, newInteriorMachine.inventoryLevelsValid);
		}
		TEST_METHOD(SwitchBays_SwitchToBay2_Bay2Selected)
		{
			//Test to ensure that the machine can switch bays

			InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

			bool didSwitch = newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");

			Assert::AreEqual(true, didSwitch);
		}
	};
}