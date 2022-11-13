#include <iostream>
#include "BodyMachine.h"

using namespace std;

int main(void)
{
	cout << "Body" << endl;

	BodyMachine newBodyMachine;//notably a new body machine WILL NOT be created for every vehicle just for use in this test

	//This information will normally be handeled in the plant object
	Order placedOrder;
	Vehicle vehiclePlaceholder;

	placedOrder.bodyPanelSet = "Regular";
	placedOrder.model = "Expedition";
	vehiclePlaceholder.body = "N/A";

	newBodyMachine.UpdateRegularExpeditionInventoryAmount(100, "BayOne");

	newBodyMachine.RunBodyMachine(placedOrder, &vehiclePlaceholder);

	return 0;
}