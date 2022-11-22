#include "pch.h"
#include "CppUnitTest.h"
#include "PaintMachine.h"
#include "Vehicle.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FordAssemblyPlantIntegrationTests
{
	TEST_CLASS(FordAssemblyPlantIntegrationTests)
	{
	public:

		TEST_METHOD(BodyMachineAndVehicle_UpdateBodyPanelSet_VehicleReceivesBodyPanelSet)
		{

		}

		TEST_METHOD(BodyMachineAndOrder_ReadOrder_OrderInfoReceived)
		{

		}

		TEST_METHOD(BodyMachineAndCurrentBay_InventoryLevel_BayReceivesUpdateInvAmount)
		{

		}

		TEST_METHOD(BodyMachineAndCurrentBay_InventoryLevel_BodyMachineRecievesInvLevel)
		{

		}

		TEST_METHOD(BodyMachineAndCurrentBay_BayInUse_BodyMachineReceivesTrueFlag)
		{

		}

		TEST_METHOD(BodyMachineAndCurrentBay_BayNotInUse_BayReceivesFalseFlag)
		{

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
			int expectedRvalue = 6;
			int expectedGvalue = 0;
			int expectedBvalue = 0;
			DipTank dipTank;
			DryingChamber dryingChamber;
			PaintChamber paintChamber;
			PaintMachine paintMachine(&dryingChamber,&paintChamber,&dipTank);

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