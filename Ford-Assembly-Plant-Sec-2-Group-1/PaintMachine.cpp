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

int PaintChamber::readHumidity(void)
{
	int result = 0;
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

		result = stoi(humidity);
	}
	else
	{
		cout << "Error reading humidity of Paint Chamber" << endl;
	}

	return result;
}

void PaintChamber::validateTemperature(void)
{
	double temp = getTemperature();
	double input;

	if (temp < getminimumTemperature())
	{
		cout << "[WARNING] Current Temperature of: " << temp << "is below minimum permitted temperature of " <<  getminimumTemperature() << endl;
		cout << "Enter Temperature:" << endl;
		cin >> input;
		updateTemperature(input);
	}
	else if (temp > getmaximumTemperature())
	{
		cout << "[WARNING] Current Temperature of: " << temp << "is above maximum permitted temperature of " << getmaximumTemperature() << endl;
		cout << "Enter Temperature:" << endl;
		cin >> input;
		updateTemperature(input);
	}
	else
	{
		cout << "Current Temperature of Paint Chamber: " << temp << endl;
	}
}

void PaintChamber::updateTemperature(double temp)
{
	bool running = true;
	
	while (running)
	{
		if (isdigit(temp))
		{
			if (temp >= getminimumTemperature() && temp <= getmaximumTemperature())
			{
				setTemperature(temp);
				running = false;
			}
			else
			{
				cout << "Tempearture not between range of " << getminimumTemperature() << "-" << getmaximumTemperature() << endl;
				cout << "Enter Temperature:" << endl;
				cin >> temp;
			}
		}
		else
		{
			cout << "Invalid Temperature entered" << endl;
			cout << "Enter Temperature:" << endl;
			cin >> temp;
		}
	}
}

void PaintChamber::updateHumidity(int humidity)
{
	bool running = true;

	while (running)
	{
		if (isdigit(humidity))
		{
			if (humidity>= getminimumHumidity() && humidity<= getmaximumHumidity())
			{
				setHumidity(humidity);
				running = false;
			}
			else
			{
				cout << "Humidity not between range of " << getminimumHumidity() << "-" << getmaximumHumidity() << endl;
				cout << "Enter Humidity:" << endl;
				cin >> humidity;
			}
		}
		else
		{
			cout << "Invalid Humidity entered" << endl;
			cout << "Enter Humidity:" << endl;
			cin >> humidity;
		}
	}

}

void PaintChamber::validateHumidity(void)
{
	int humidity = getHumidity();
	int input;

	if (humidity < getminimumHumidity())
	{
		cout << "[WARNING] Current Humidity of: " << humidity << "is below minimum permitted humidity of " << getminimumHumidity() << endl;
		cout << "Enter Humidity:" << endl;
		cin >> input;
		updateHumidity(input);
	}
	else if (humidity> getmaximumHumidity())
	{
		cout << "[WARNING] Current Humidity of: " << humidity << "is above maximum permitted humidity of " << getmaximumHumidity() << endl;
		cout << "Enter Humidity:" << endl;
		cin >> input;
		updateHumidity(input);
	}
	else
	{
		cout << "Current Humidity of Paint Chamber: " << humidity << endl;
	}
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

double DryingChamber::readTemperature(void)
{
	double result = 0;
	string fileName = "DryingChamberTemperature.txt";
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
		cout << "Error reading temperature of Drying Chamber" << endl;
	}

	return result;
}

int DryingChamber::readHumidity(void)
{
	int result = 0;
	string fileName = "DryingChamberHumidity.txt";
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

		result = stoi(humidity);
	}
	else
	{
		cout << "Error reading humidity of Paint Chamber" << endl;
	}

	return result;
}

void DryingChamber::updateTemperature(double temp)
{
	bool running = true;

	while (running)
	{
		if (isdigit(temp))
		{
			if (temp >= getminimumTemperature() && temp <= getmaximumTemperature())
			{
				setTemperature(temp);
				running = false;
			}
			else
			{
				cout << "Tempearture not between range of " << getminimumTemperature() << "-" << getmaximumTemperature() << endl;
				cout << "Enter Temperature:" << endl;
				cin >> temp;
			}
		}
		else
		{
			cout << "Invalid Temperature entered" << endl;
			cout << "Enter Temperature:" << endl;
			cin >> temp;
		}
	}
}

void DryingChamber::validateTemperature(void)
{
	double temp = getTemperature();
	double input;

	if (temp < getminimumTemperature())
	{
		cout << "[WARNING] Current Temperature of: " << temp << "is below minimum permitted temperature of " << getminimumTemperature() << endl;
		cout << "Enter Temperature:" << endl;
		cin >> input;
		updateTemperature(input);
	}
	else if (temp > getmaximumTemperature())
	{
		cout << "[WARNING] Current Temperature of: " << temp << "is above maximum permitted temperature of " << getmaximumTemperature() << endl;
		cout << "Enter Temperature:" << endl;
		cin >> input;
		updateTemperature(input);
	}
	else
	{
		cout << "Current Temperature of Drying Chamber: " << temp << endl;
	}
}

void DryingChamber::validateHumidity(void)
{
	int humidity = getHumidity();
	int input;

	if (humidity < getminimumHumidity())
	{
		cout << "[WARNING] Current Humidity of: " << humidity << "is below minimum permitted humidity of " << getminimumHumidity() << endl;
		cout << "Enter Humidity:" << endl;
		cin >> input;
		updateHumidity(input);
	}
	else if (humidity > getmaximumHumidity())
	{
		cout << "[WARNING] Current Humidity of: " << humidity << "is above maximum permitted humidity of " << getmaximumHumidity() << endl;
		cout << "Enter Humidity:" << endl;
		cin >> input;
		updateHumidity(input);
	}
	else
	{
		cout << "Current Humidity of Drying Chamber: " << humidity << endl;
	}
}

void DryingChamber::updateHumidity(int humidity)
{
	bool running = true;

	while (running)
	{
		if (isdigit(humidity))
		{
			if (humidity >= getminimumHumidity() && humidity <= getmaximumHumidity())
			{
				setHumidity(humidity);
				running = false;
			}
			else
			{
				cout << "Humidity not between range of " << getminimumHumidity() << "-" << getmaximumHumidity() << endl;
				cout << "Enter Humidity:" << endl;
				cin >> humidity;
			}
		}
		else
		{
			cout << "Invalid Humidity entered" << endl;
			cout << "Enter Humidity:" << endl;
			cin >> humidity;
		}
	}
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