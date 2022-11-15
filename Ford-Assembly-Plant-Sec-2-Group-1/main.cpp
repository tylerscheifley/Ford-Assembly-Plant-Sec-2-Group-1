#include <iostream>
#include "plant.h"
#include "plantFiles.h"
#define MAXPLANTS 4

using namespace std;

int main(void)
{
	Plant* plant = new Plant;
	sampleDataCreator();
	readLog(plant);
	updateLog(plant);


	//Plant* plant = new Plant[MAXPLANTS];
	cout << "Plant" << endl;

	return 0;
}