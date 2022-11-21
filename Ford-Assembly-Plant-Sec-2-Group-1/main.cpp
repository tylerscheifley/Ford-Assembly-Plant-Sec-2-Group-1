#include "BodyMachine.h"
#include "ChassisMachine.h"
#include "InteriorMachine.h"
#include "PaintMachine.h"
#include "plant.h"

int main(void)
{
	InteriorMachine newInteriorMachine;//notably a new interior machine WILL NOT be created for every vehicle just for use in this test

	//This information will normally be handeled in the plant object
	Order placedOrder;
	Vehicle vehiclePlaceholder;

	placedOrder.getInteriorLevel() = "base";
	placedOrder.getModel() = "F150";
	vehiclePlaceholder.getInterior() = "N/A";

	newInteriorMachine.SwitchVehiclePanelsBays("BayTwo");

	newInteriorMachine.UpdateBaseInteriorF150InventoryAmount(100, "BayOne");

	newInteriorMachine.SwitchVehiclePanelsBays("BayOne");

	newInteriorMachine.RunInteriorMachine(placedOrder, &vehiclePlaceholder);

	return 0;
}