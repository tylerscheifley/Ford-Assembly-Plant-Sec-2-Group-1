#include <iostream>
#include "ChassisMachine.h"

using namespace std;

int main(void)
{
	cout << "Chassis" << endl;

	ChassisMachine newBodyMachine;//notably a new chassis machine WILL NOT be created for every vehicle just for use in this test

	//This information will normally be handeled in the plant object
	Order placedOrder;
	Vehicle vehiclePlaceholder;

	placedOrder.engineType = "35LV6C";
	placedOrder.model = "Expedition";
	vehiclePlaceholder.chassis = "N/A";

	newBodyMachine.UpdateExpedition35LV6CInventoryAmount(100, "BayOne");

	newBodyMachine.RunChassisMachine(placedOrder, &vehiclePlaceholder);

	return 0;
}