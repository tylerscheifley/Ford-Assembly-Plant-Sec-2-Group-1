#include <iostream>
#include "plant.h"

using namespace std;

Plant::Plant(string date, int globalAirQuality, int numVehicleToday, double globalTemp, double globalHumidity, int vehicleQuota, bool assemblyLineStatus)
{
	this->date = date;
	this->globalAirQuality = globalAirQuality;
	this->numVehicleToday = numVehicleToday;
	this->globalTemp = globalTemp;
	this->globalHumidity = globalHumidity;
	this->vehicleQuota = vehicleQuota;
	this->assemblyLineStatus = assemblyLineStatus;

	// init for order;
	order.GenerateOrder();

}

Plant::Plant()
{
	date = "00/00/0000";
	globalAirQuality = 0;
	numVehicleToday = 0;
	globalTemp = 0;
	globalHumidity = 0;
	vehicleQuota = 0;
	assemblyLineStatus = false;


}

string Plant::getDate()
{
	return date;
}

int Plant::getGlobalAirQuality()
{
	return globalAirQuality;
}

int Plant::getNumVehicleToday()
{
	return numVehicleToday;
}

double Plant::getGlobalTemp()
{
	return globalTemp;
}

double Plant::getGlobalHumidity()
{
	return globalHumidity;
}

int Plant::getVehicleQuota()
{
	return vehicleQuota;
}

bool Plant::getAssemblyLineStatus()
{
	return assemblyLineStatus;
}

void Plant::setDate(string date)
{
	this->date = date;
}

void Plant::setGlobalAirQuality(int globalAirQuality)
{
	this->globalAirQuality = globalAirQuality;
}

void Plant::setNumVehicleToday(int numVehicleToday)
{
	this->numVehicleToday = numVehicleToday;
}

void Plant::setGlobalTemp(double globalTemp)
{
	this->globalTemp = globalTemp;
}

void Plant::setGlobalHumidity(double globalHumidity)
{
	this->globalHumidity = globalHumidity;
}

void Plant::setVehicleQuota(int vehicleQuota)
{
	this->vehicleQuota = vehicleQuota;
}

void Plant::setAssemblyLineStatus(bool assemblyLineStatus)
{
	this->assemblyLineStatus = assemblyLineStatus;
}

void Plant::runPlant()
{
	bool result = false;

	while ((result == false) && (getAssemblyLineStatus() == true))
	{
		//bodyMachine.RunBodyMachine(order, *vehicle);
		//paintMachine.runMachine();
		//chassisMAchine.runMachine();
		//interiorMachine.runMachine();
		//result = Vehicle.CheckQAQC();

		if (result == true)
		{

		}
	}
}