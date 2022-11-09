// PaintMachine.cpp
// Ford Assembly Plant: PaintMachine
// Created: 11/08/22
// Author: Tyler Scheifley

#include "PaintMachine.h"

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

void PaintMachine::setpaintVolumeBLUE(int paintVolume)
{
	this->paintVolumeGREEN = paintVolume;
}

int PaintMachine::getpaintVolumeRED()
{
	return this->paintVolumeRED;
}

void PaintMachine::setpaintVolumeBLUE(int paintVolume)
{
	this->paintVolumeRED = paintVolume;
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

double PaintChamber::getHumidity()
{
	return this->humidity;
}

void PaintChamber::setHumidity(double humidity)
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

double PaintChamber::getmaximumHumidity()
{
	return this->maximumHumidity;
}

void PaintChamber::setmaximumHumidity(double humidity)
{
	this->maximumHumidity = humidity;
}

double PaintChamber::getminimumHumidity()
{
	return this->minimumHumidity;
}

void PaintChamber::setminimumHumidity(double humidity)
{
	this->minimumHumidity = humidity;
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

double DryingChamber::getHumidity()
{
	return this->humidity;
}

void DryingChamber::setHumidity(double humidity)
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

double DryingChamber::getmaximumHumidity()
{
	return this->maximumHumidity;
}

void DryingChamber::setmaximumHumidity(double humidity)
{
	this->maximumHumidity = humidity;
}

double DryingChamber::getminimumHumidity()
{
	return this->minimumHumidity;
}

void DryingChamber::setminimumHumidity(double humidity)
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