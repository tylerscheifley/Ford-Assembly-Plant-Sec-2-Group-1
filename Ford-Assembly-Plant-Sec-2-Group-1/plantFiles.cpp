#include <iostream>
#include <fstream>
#include "plant.h"
#include "plantFiles.h"
#define MAXPLANTS 4

using namespace std;


void updateLog(Plant plant)
{
	ofstream plantSave;
	plantSave.open("plantLog.txt", ios_base::app);

	string date;
	//int i = 0;
	int globalAirQuality, numVehicleToday, vehicleQuota;
	double globalTemp,globalHumidity; 
	bool assemblyLineStatus;

	if (plantSave.is_open())
	{	
		while (plant.getDate().compare("00/00/0000") != 0)
		{
			date = plant.getDate();
			globalAirQuality = plant.getGlobalAirQuality();
			numVehicleToday = plant.getNumVehicleToday();
			vehicleQuota = plant.getVehicleQuota();
			globalTemp = plant.getGlobalTemp();
			globalHumidity = plant.getGlobalHumidity();
			assemblyLineStatus = plant.getAssemblyLineStatus();


			plantSave << "\n --------------------\n" << " Date: " << date << "\n CO2 Concentration: " << globalAirQuality << "ppm\n" << "Number of vehicles made today: " << numVehicleToday << "\n Today's vehicle quota:  " << vehicleQuota << "\n Plant's global temperature: " << globalTemp << "F\n Plant's global humidity: " << globalHumidity << "%\n Assembly line running:" << assemblyLineStatus << "\n" << " --------------------\n";
			//i++;
		}
	}
	plantSave.close();
}

void readLog(Plant plant)
{
	ifstream plantLoad;
	plantLoad.open("plantSensors.txt");


	string date;
	int globalAirQuality, numVehicleToday, vehicleQuota;
	double globalTemp, globalHumidity;
	bool assemblyLineStatus;
//	int counter = 0;

	if (plantLoad.is_open())
	{
		while (plantLoad >> date >> globalAirQuality >> numVehicleToday >> vehicleQuota >> globalTemp >> globalHumidity >> assemblyLineStatus)
		{
			plant.setDate(date);
			plant.setGlobalAirQuality(globalAirQuality);
			plant.setNumVehicleToday(numVehicleToday);
			plant.setVehicleQuota(vehicleQuota);
			plant.setGlobalTemp(globalTemp);
			plant.setGlobalHumidity(globalHumidity);
			plant.setAssemblyLineStatus(assemblyLineStatus);

			//counter++;
		}
	}
	plantLoad.close();
}

void stopPlant(Plant plant)
{
	cout << "Plant is shutting down..." << endl;
	updateLog(plant);
	plant.setAssemblyLineStatus(false);
}