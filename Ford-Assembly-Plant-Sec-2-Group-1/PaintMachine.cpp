// PaintMachine.cpp
// Ford Assembly Plant: PaintMachine
// Created: 11/08/22
// Author: Tyler Scheifley

#include "PaintMachine.h"
#include <string>

using namespace std;

// PaintMachine Class Methods
PaintMachine::PaintMachine()
{
	this->colour = "N/A";
	this->maxpaintVolume = 0;
	this->paintVolumeRED = 0;
	this->paintVolumeBLUE = 0;
	this->paintVolumeGREEN = 0;
}

string PaintMachine::getcolour()
{
	return this->colour;
}

void PaintMachine::setcolour(string colour)
{
	this->colour = colour;
}

int PaintMachine::getmaxpaintVolume()
{
	return this->maxpaintVolume;
}

void PaintMachine::setmaxpaintVolume(int max)
{
	this->maxpaintVolume = maxpaintVolume;
}

int PaintMachine::getpaintVolumeBLUE()
{
	return this->paintVolumeBLUE;
}

void PaintMachine::setpaintVolumeBLUE(int paintVolume)
{
	this->paintVolumeBLUE = paintVolume;
}

int PaintMachine::getpaintVolumeGREEN()
{
	return this->paintVolumeGREEN;
}

void PaintMachine::setpaintVolumeGREEN(int paintVolume)
{
	this->paintVolumeGREEN = paintVolume;
}

int PaintMachine::getpaintVolumeRED()
{
	return this->paintVolumeRED;
}

void PaintMachine::setpaintVolumeRED(int paintVolume)
{
	this->paintVolumeRED = paintVolume;
}

void PaintMachine::validatepaintVolume(string RGBcolour)
{
	//calculating 20% of maximum capacity to warn user
	int lowVolume = (getmaxpaintVolume() * 20) / 100;
	int remainingVolume = (getmaxpaintVolume() / getpaintVolumeRED()) * 100;

	if (getpaintVolumeRED() == 0)
	{
		cout << RGBcolour << " Paint Vat is empty" << endl;
		//resupply
		// if resupply is false, loop until user decides to resupply
	}
	else if (getpaintVolumeRED() == lowVolume)
	{
		cout << RGBcolour << " Paint Vat Is At 20% Capacity" << endl;
		//resupply
	}
	else
	{
		cout << RGBcolour << " Paint Vat is at " << remainingVolume << "% Capacity" << endl;
	}
}

// PaintChamber Class Methods:

PaintChamber::PaintChamber()
{
	this->temperature = 0;
	this->humidity = 0;
	this->maximumTemperature = 0;
	this->minimumTemperature = 0;
	this->maximumHumidity = 0;
	this->minimumHumidity = 0;
}

int PaintChamber::getHumidity()
{
	return this->humidity;
}

void PaintChamber::setHumidity(int humidity)
{
	this->humidity = humidity;
}

double PaintChamber::getTemperature()
{
	return this->temperature;
}

void PaintChamber::setTemperature(double temp)
{
	this->temperature = temp;
}

double PaintChamber::getmaximumTemperature()
{
	return this->maximumTemperature;
}

void PaintChamber::setmaximumTemperature(double temp)
{
	this->maximumTemperature = temp;
}

double PaintChamber::getminimumTemperature()
{
	return this->minimumTemperature;
}

void PaintChamber::setminimumTemperature(double temp)
{
	this->minimumTemperature = temp;
}

int PaintChamber::getmaximumHumidity()
{
	return this->maximumHumidity;
}

void PaintChamber::setmaximumHumidity(int humidity)
{
	this->maximumHumidity = humidity;
}

int PaintChamber::getminimumHumidity()
{
	return this->minimumHumidity;
}

void PaintChamber::setminimumHumidity(int humidity)
{
	this->minimumHumidity = humidity;
}

double PaintChamber::readTemperature(void)
{
	double result = 0;
	string fileName = "PaintChamberTemperature.txt";
	string temp;
	srand((unsigned)time(NULL));
	int random = 1 + (rand() % 100);
	

	ifstream fin;
	fin.open(fileName);

	if (fin.is_open())
	{
		for (int index = 0; index < random; index++)
		{
			getline(fin, temp);
		}

		result = stod(temp);
	}
	else
	{
		cout << "Error reading temperature of Paint Chamber" << endl;
	}

	return result;
}

double PaintChamber::readHumidity(void)
{
	double result = 0;
	string fileName = "PaintChamberHumidity.txt";
	string humidity;
	srand((unsigned)time(NULL));
	int random = 1 + (rand() % 100);
	

	ifstream fin;
	fin.open(fileName);

	if (fin.is_open())
	{
		for (int index = 0; index < random; index++)
		{
			getline(fin, humidity);
		}

		result = stod(humidity);
	}
	else
	{
		cout << "Error reading humidity of Paint Chamber" << endl;
	}

	return result;
}

// DryingChamber Class Methods:
DryingChamber::DryingChamber()
{
	this->temperature = 0;
	this->humidity = 0;
	this->maximumTemperature = 0;
	this->minimumTemperature = 0;
	this->maximumHumidity = 0;
	this->minimumHumidity = 0;
}

int DryingChamber::getHumidity()
{
	return this->humidity;
}

void DryingChamber::setHumidity(int humidity)
{
	this->humidity = humidity;
}

double DryingChamber::getTemperature()
{
	return this->temperature;
}

void DryingChamber::setTemperature(double temp)
{
	this->temperature = temp;
}

double DryingChamber::getmaximumTemperature()
{
	return this->maximumTemperature;
}

void DryingChamber::setmaximumTemperature(double temp)
{
	this->maximumTemperature = temp;
}

double DryingChamber::getminimumTemperature()
{
	return this->minimumTemperature;
}

void DryingChamber::setminimumTemperature(double temp)
{
	this->minimumTemperature = temp;
}

int DryingChamber::getmaximumHumidity()
{
	return this->maximumHumidity;
}

void DryingChamber::setmaximumHumidity(int humidity)
{
	this->maximumHumidity = humidity;
}

int DryingChamber::getminimumHumidity()
{
	return this->minimumHumidity;
}

void DryingChamber::setminimumHumidity(int humidity)
{
	this->minimumHumidity = humidity;
}

// DipTank Class Methods:

DipTank::DipTank()
{
	this->fluidLevel = 0;
	this->maximumfluidLevel = 0;
	this->minimumTemperature = 0;
	this->maximumTemperature = 0;
	this->temperature = 0;
}

int DipTank::getfluidLevel()
{
	return this->fluidLevel;
}

void DipTank::setfluidLevel(int level)
{
	this->fluidLevel = level;
}

double DipTank::getmaximumfluidLevel()
{
	return this->maximumfluidLevel;
}

void DipTank::setmaximumfluidLevel(double level)
{
	this->maximumfluidLevel = level;
}

double DipTank::getmaximumTemperature()
{
	return this->maximumTemperature;
}

void DipTank::setmaximumTemperature(double temp)
{
	this->maximumTemperature = temp;
}

double DipTank::getminimumTemperature()
{
	return this->minimumTemperature;
}

void DipTank::setminimumTemperature(double temp)
{
	this->minimumTemperature = temp;
}

double DipTank::getTemperature()
{
	return this->temperature;
}

void DipTank::setTemperature(double temp)
{
	this->temperature = temp;
}