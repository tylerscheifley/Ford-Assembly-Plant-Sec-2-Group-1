#include <iostream>
#include <fstream>
#include <ctime>
#include "plant.h"
#include "plantFiles.h"

#define MAXPLANTS 4

using namespace std;

void updateLog(Plant *plant)
{

	ofstream plantSave;
	plantSave.open("plantLog.txt", ios::app);

	string date;
	
	int globalAirQuality, numVehicleToday, vehicleQuota;
	double globalTemp,globalHumidity; 
	bool assemblyLineStatus;

	if (plantSave.is_open())
	{	
			date = plant->getDate();
			globalAirQuality = plant->getGlobalAirQuality();
			numVehicleToday = plant->getNumVehicleToday();
			vehicleQuota = plant->getVehicleQuota();
			globalTemp = plant->getGlobalTemp();
			globalHumidity = plant->getGlobalHumidity();
			assemblyLineStatus = plant->getAssemblyLineStatus();

			plantSave << "\n --------------------\n" << " Date: " << date << "\n CO2 Concentration: " << globalAirQuality << "ppm\n" << " Number of vehicles made today: " << numVehicleToday << "\n Today's vehicle quota:  " << vehicleQuota << "\n Plant's global temperature: " << globalTemp << "F\n Plant's global humidity: " << globalHumidity << "%\n Assembly line running:" << assemblyLineStatus << "\n" << " --------------------\n";
	}
	plantSave.close();
}

void readLog(Plant *plant)
{

	ifstream plantLoad;
	plantLoad.open("sampleLog.txt");

	string date;
	int globalAirQuality = 0, numVehicleToday = 0, vehicleQuota = 0;
	double globalTemp = 0, globalHumidity = 0;

	if (plantLoad.is_open())
	{
		plantLoad >> date >> globalAirQuality >> vehicleQuota >> globalTemp >> globalHumidity;

		plant->setDate(date);
		plant->setGlobalAirQuality(globalAirQuality);
		plant->setNumVehicleToday(numVehicleToday);
		plant->setVehicleQuota(vehicleQuota);
		plant->setGlobalTemp(globalTemp);
		plant->setGlobalHumidity(globalHumidity);
	}
	plantLoad.close();
}

void stopPlant(Plant* plant)
{

	cout << "Plant is shutting down..." << endl;
	updateLog(plant);
	plant->setAssemblyLineStatus(false);
}

void sampleDataCreator()
{

	srand(clock());

	char temp[10];
	int day, month;
	int globalAirQuality,vehicleQuota;
	double globalTemp, globalHumidity;

	for (int i = 0; i < 31; i++)
	{
		string date;

		globalTemp = rand() % 150 + 0;
		globalHumidity = rand() % 100 + 0;
		globalAirQuality = rand() % 350 + 0;
		vehicleQuota = rand() % 1660 + 1;

		month = rand() % 12 + 1;
		day = rand() % 31 + 1;

		if (((month == 9) || (month == 4) || (month == 6) || (month == 11)) && (day == 31))
			day--;

		_itoa_s(day, temp, 10);
		date.append(temp);
		date.append("/");

		_itoa_s(month, temp, 10);
		date.append(temp);
		date.append("/2022");

		sampleDataSaver(date, globalAirQuality, vehicleQuota, globalTemp, globalHumidity);
	}
}

void sampleDataSaver(string date, int globalAirQuality, int vehicleQuota, double globalTemp, double globalHumidity)
{

	ofstream sampleSave;
	sampleSave.open("sampleLog.txt", ios::app);

	if (sampleSave.is_open())
	{
		sampleSave << date << " " << globalAirQuality << " " << vehicleQuota << " " << globalTemp << " " << globalHumidity << "\n";
	}

	sampleSave.close();
}