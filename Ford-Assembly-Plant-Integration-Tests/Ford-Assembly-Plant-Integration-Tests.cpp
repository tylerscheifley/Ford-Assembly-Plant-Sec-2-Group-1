#include "pch.h"
#include "CppUnitTest.h"
#include "InteriorMachine.h"
#include "ChassisMachine.h"
#include "BodyMachine.h"
#include "Vehicle.h"
#include "Order.h"
#include "Plant.h"
#include "PaintMachine.h"
//#include "PlantColours.txt"

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
			// Testing the PaintMachine class reading from the PlantColours.txt
			// Expected values: Red - 6, Green - 0, Blue - 0
			// Selects Race Red colour from the text file

			int expectedRvalue = 6;
			int expectedGvalue = 0;
			int expectedBvalue = 0;
			DipTank dipTank;
			DryingChamber dryingChamber;
			PaintChamber paintChamber;
			PaintMachine paintMachine(&dryingChamber,&paintChamber,&dipTank);
			
			//calls the identifyRGBvalues to search for Race Red and set RGB values

			paintMachine.setcolour("Race Red");
			paintMachine.identifyRGBvalues("PlantColours.txt");

			//Asserting that the paint machine RGB attributes are equal to the expected  

			Assert::AreEqual(expectedRvalue,paintMachine.getRED());
			Assert::AreEqual(expectedGvalue,paintMachine.getGREEN());
			Assert::AreEqual(expectedBvalue,paintMachine.getBLUE());

		}

		TEST_METHOD(PaintMachine_Read_RGB_Paint_Volume_INT_TEST)
		{
			// Testing the PaintMachine's class reading the RGB paint Volumes from the TestingVats.txt
			// Expected values: Red Volume - 500, Green Volume - 500, Blue Volume - 500

			int expectedRpaintVolume = 500;
			int expectedGpaintVolume = 500;
			int expectedBpaintVolume = 500;
			DipTank dipTank; // following objects are needed to initialize paintMachine constructor
			DryingChamber dryingChamber;
			PaintChamber paintChamber;
			PaintMachine paintMachine(&dryingChamber, &paintChamber, &dipTank);

			//Set painting volume is used to reset file to default 500  
			paintMachine.setpaintVolumeRED(500);
			paintMachine.setpaintVolumeGREEN(500);
			paintMachine.setpaintVolumeBLUE(500);
			paintMachine.resupplyRGBpaintVat("RED", 0, "TestingVats.txt"); //0 since not adding 500 uses paint volume instead
			paintMachine.resupplyRGBpaintVat("GREEN", 0, "TestingVats.txt");
			paintMachine.resupplyRGBpaintVat("BLUE", 0, "TestingVats.txt");

			paintMachine.readRGBpaintVat("TestingVats.txt");

			//Checking the paint volumes read from the testingVats.txt file returns 500 for RGB paint volumes
			Assert::AreEqual(expectedRpaintVolume, paintMachine.getpaintVolumeRED());
			Assert::AreEqual(expectedGpaintVolume, paintMachine.getpaintVolumeGREEN());
			Assert::AreEqual(expectedBpaintVolume, paintMachine.getpaintVolumeBLUE());

		}

		TEST_METHOD(PaintMachine_Update_RGB_Paint_Volume_INT_TEST)
		{
			// Testing the PaintMachine's class updating the RGB paint Volumes from the TestingVats.txt
			// Expected values: Red Volume - 100, Green Volume - 100, Blue Volume 
			// updating file with subtract 400 liters from the 500, leaving 100 as the difference

			int expectedRpaintVolume = 100;
			int expectedGpaintVolume = 100;
			int expectedBpaintVolume = 100;
			DipTank dipTank;
			DryingChamber dryingChamber;
			PaintChamber paintChamber;
			PaintMachine paintMachine(&dryingChamber, &paintChamber, &dipTank);
			
			//Resetting file to default RGB paint volumes of 500L each
			paintMachine.resupplyRGBpaintVat("RED", 500, "TestingVats.txt");
			paintMachine.resupplyRGBpaintVat("GREEN", 500, "TestingVats.txt");
			paintMachine.resupplyRGBpaintVat("BLUE", 500, "TestingVats.txt");
			
			//setting the amount of liters to be subtracted
			paintMachine.setpaintVolumeRED(500);
			paintMachine.setpaintVolumeGREEN(500);
			paintMachine.setpaintVolumeBLUE(500);

			//amount to be subtracted from paint volume in the file
			paintMachine.setRED(400);
			paintMachine.setGREEN(400);
			paintMachine.setBLUE(400);


			paintMachine.updateRGBpaintVat("TestingVats.txt");
			//
			paintMachine.readRGBpaintVat("TestingVats.txt");

			Assert::AreEqual(expectedRpaintVolume, paintMachine.getpaintVolumeRED());
			Assert::AreEqual(expectedGpaintVolume, paintMachine.getpaintVolumeGREEN());
			Assert::AreEqual(expectedBpaintVolume, paintMachine.getpaintVolumeBLUE());
		}

		TEST_METHOD(PaintMachine_DipTank_Read_Temperature_INT_TEST)
		{
			DipTank dipTank;
			double expectedTemp = 19;

			dipTank.readTemperature("testingTemperatureDiptank.txt");

			Assert::AreEqual(expectedTemp, dipTank.getTemperature());
			//dip tank had no setter
		}

		TEST_METHOD(PaintMachine_DipTank_Read_FluidLevel_INT_TEST)
		{
			DipTank dipTank;
			int expectedfluidLevel = 50;

			dipTank.readfluidLevel("testingfluidLevelDiptank.txt");

			Assert::AreEqual(expectedfluidLevel, dipTank.getfluidLevel());
			// Dip tank had no setter
		}

		TEST_METHOD(PaintMachine_PaintChamber_Read_Temperature_INT_TEST)
		{
			PaintChamber paintChamber;
			double expectedTemperature =20;

			paintChamber.readTemperature("testingTemperaturePaintChamber.txt");

			Assert::AreEqual(expectedTemperature, paintChamber.getTemperature());
		}

		TEST_METHOD(PaintMachine_PaintChamber_Read_Humidity_INT_TEST)
		{
			PaintChamber paintChamber;
			int expectedhumidity = 45;

			paintChamber.readHumidity("testingHumidityPaintChamber.txt");

			Assert::AreEqual(expectedhumidity, paintChamber.getHumidity());
		}

		TEST_METHOD(PaintMachine_Order_Pass_Colour_INT_TEST)
		{
			string expectedColour = "Infinite Blue Metallic Tinted Clearcoat";
			DipTank dipTank;
			DryingChamber dryingChamber;
			PaintChamber paintChamber;
			PaintMachine paintMachine(&dryingChamber, &paintChamber, &dipTank);
			Order order;

			order.setColour("Infinite Blue Metallic Tinted Clearcoat");
			paintMachine.setcolour(order.getColour());

			Assert::IsTrue(expectedColour == paintMachine.getcolour());

		}

		TEST_METHOD(PaintMachine_Initialize_DipTank_INT_TEST)
		{
			DipTank dipTank;
			DryingChamber dryingChamber;
			PaintChamber paintChamber;
			PaintMachine paintMachine(&dryingChamber, &paintChamber, &dipTank);
			double expectedMAXtemp = 24;
			double expectedMINtemp = 19;
			int expectedMAXfluidLevel = 50;

			Assert::AreEqual(expectedMAXfluidLevel, dipTank.getmaximumfluidLevel());
			Assert::AreEqual(expectedMINtemp, dipTank.getminimumTemperature());
			Assert::AreEqual(expectedMAXtemp, dipTank.getmaximumTemperature());

		}

		TEST_METHOD(PaintMachine_Initialize_DryingChamber_INT_TEST)
		{
			DipTank dipTank;
			DryingChamber dryingChamber;
			PaintChamber paintChamber;
			PaintMachine paintMachine(&dryingChamber, &paintChamber, &dipTank);
			double expectedMAXtemp = 24;
			double expectedMINtemp = 19;
			int expectedMAXhumidity = 50;
			int expectedMINhumidity = 40;

			Assert::AreEqual(expectedMAXhumidity, dryingChamber.getmaximumHumidity());
			Assert::AreEqual(expectedMINhumidity, dryingChamber.getminimumHumidity());
			Assert::AreEqual(expectedMAXtemp, dryingChamber.getmaximumTemperature());
			Assert::AreEqual(expectedMINtemp, dryingChamber.getminimumTemperature());
		}

		TEST_METHOD(PaintMachine_Initialize_PaintChamber_INT_TEST)
		{
			DipTank dipTank;
			DryingChamber dryingChamber;
			PaintChamber paintChamber;
			PaintMachine paintMachine(&dryingChamber, &paintChamber, &dipTank);
			double expectedMAXtemp = 24;
			double expectedMINtemp = 19;
			int expectedMAXhumidity = 50;
			int expectedMINhumidity = 40;

			Assert::AreEqual(expectedMAXhumidity, paintChamber.getmaximumHumidity());
			Assert::AreEqual(expectedMINhumidity, paintChamber.getminimumHumidity());
			Assert::AreEqual(expectedMAXtemp, paintChamber.getmaximumTemperature());
			Assert::AreEqual(expectedMINtemp, paintChamber.getminimumTemperature());
		}
	};
}