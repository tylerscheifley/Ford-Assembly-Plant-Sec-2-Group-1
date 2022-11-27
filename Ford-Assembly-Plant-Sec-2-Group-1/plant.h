#pragma once
#include "BodyMachine.h"
#include "ChassisMachine.h"
#include "InteriorMachine.h"
#include "PaintMachine.h"


using namespace std;

#ifndef PLANT_H
#define PLANT_H

class Plant
{
	string date;
	int globalAirQuality;
	int numVehicleToday;
	double globalTemp; //Rename into plantTemp when integration is complete
	double globalHumidity; // rename into plantHumidity when integration is complete
	int vehicleQuota;
	bool assemblyLineStatus;

public:

	Vehicle vehicle;
	Order order;
	
	BodyMachine bodyMachine = BodyMachine();
	ChassisMachine chassisMachine = ChassisMachine();
	InteriorMachine interiorMachine = InteriorMachine();
	DipTank dipTank = DipTank();
	DryingChamber dryingChamber = DryingChamber();
	PaintChamber paintChamber = PaintChamber();
	PaintMachine paintingMachine = PaintMachine(&dryingChamber, &paintChamber, &dipTank);

	Plant(string date, int globalAirQuality, int numVehicleToday, double globalTemp, double globalHumidity, int vehicleQuota, bool assemblyLineStatus);

	Plant();

	// Getters
	string getDate();
	int getGlobalAirQuality();
	int getNumVehicleToday();
	double getGlobalTemp();
	double getGlobalHumidity();
	int getVehicleQuota();
	bool getAssemblyLineStatus();

	// Setters
	void setDate(string date);
	void setGlobalAirQuality(int globalAirQuality);
	void setNumVehicleToday(int numVehicleToday);
	void setGlobalTemp(double globalTemp);
	void setGlobalHumidity(double globalHumidity);
	void setVehicleQuota(int vehicleQuota);
	void setAssemblyLineStatus(bool assemblyLineStatus);


	void runPlant();
};

#endif // !PLANT_H

