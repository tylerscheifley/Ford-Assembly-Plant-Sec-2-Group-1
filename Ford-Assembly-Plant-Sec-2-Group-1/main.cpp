#include <iostream>
#include "ChassisMachine.h"

using namespace std;

int main(void)
{
	cout << "Chassis" << endl;

	ChassisMachine newChassisMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test

	//This information will normally be handeled in the plant object
	Order placedOrder;
	Vehicle vehiclePlaceholder;

	placedOrder.engineType = "35LV6C";
	placedOrder.model = "Expedition";
	vehiclePlaceholder.chassis = "N/A";

	newChassisMachine.SwitchVehicleChassisLines("LineTwo");

	newChassisMachine.UpdateExpedition35LV6CInventoryAmount(10, "LineOne");

	newChassisMachine.SwitchVehicleChassisLines("LineOne");

	newChassisMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);

	return 0;
}