#include <iostream>
#include "InteriorMachine.h"

using namespace std;

int main(void)
{
	cout << "Interior" << endl;

	InteriorMachine newInteriorMachine;

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


	return 0;
}