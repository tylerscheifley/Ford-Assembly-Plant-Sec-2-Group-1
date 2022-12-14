// PaintMachine.cpp
// Ford Assembly Plant: PaintMachine
// Created: 11/08/22
// Author: Tyler Scheifley

#include "PaintMachine.h"
#include <string>
#define MAX_PAINT_NEEDED 5

using namespace std;

// PaintMachine Class Methods
PaintMachine::PaintMachine(DryingChamber* dryingChamber, PaintChamber* paintChamber, DipTank* dipTank)
{
	this->colour = "N/A";
	this->maxpaintVolume = 0;
	this->paintVolumeRED = 0;
	this->paintVolumeBLUE = 0;
	this->paintVolumeGREEN = 0;
	this->RED = 0;
	this->BLUE = 0;
	this->GREEN = 0;

	dryingChamber->setmaximumTemperature(24);
	dryingChamber->setminimumTemperature(19);//
	dryingChamber->setmaximumHumidity(50);
	dryingChamber->setminimumHumidity(40);
	paintChamber->setmaximumTemperature(24);
	paintChamber->setminimumTemperature(19);
	paintChamber->setmaximumHumidity(50);
	paintChamber->setminimumHumidity(40);
	dipTank->setmaximumfluidLevel(1000);
	dipTank->setmaximumTemperature(24);
	dipTank->setminimumTemperature(19);
	setmaxpaintVolume(500);
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
	this->maxpaintVolume = max;
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

void PaintMachine::validatepaintVolume(string RGBcolour, string fileName)
{
	int paintVolume;
	int requestVolume;
	int volume;

	if (RGBcolour == "RED")
	{
		paintVolume = getpaintVolumeRED();
		requestVolume = getRED();
	}
	else if (RGBcolour == "BLUE")
	{
		paintVolume = getpaintVolumeBLUE();
		requestVolume = getBLUE();
	}
	else
	{
		paintVolume = getpaintVolumeGREEN();
		requestVolume = getGREEN();
	}

	//calculating 20% of maximum capacity to warn user
	int lowVolume = (getmaxpaintVolume() * 20) / 100;
	int remainingVolume = (getmaxpaintVolume() / paintVolume) * 100;

	if (paintVolume== 0)
	{
		cout << RGBcolour << " Paint Vat is empty" << endl;
		
		cout << "Enter Paint Volume" << endl;
		cin >> volume;

		volume = checkValidresupply(volume,RGBcolour);
		resupplyRGBpaintVat(RGBcolour, volume, fileName);
		
	}
	else if (paintVolume == lowVolume)
	{
		cout << RGBcolour << " Paint Vat Is At 20% Capacity" << endl;

		cout << "Do you want to resupply the " << RGBcolour << "[Y/N]" << endl;
		string choice;
		cout << "Enter Paint Volume" << endl;
		cin >> volume;

		if (choice == "Y" || choice == "y")
		{
			volume = checkValidresupply(volume,RGBcolour);
			resupplyRGBpaintVat(RGBcolour, volume, fileName);

		}	
	}
	else if (requestVolume > paintVolume)
	{
		cout << RGBcolour << " Paint Vat's volume is too low to complete request" << endl;
		cout << "Enter Paint Volume" << endl;
		cin >> volume;

		volume = checkValidresupply(volume,RGBcolour);
		resupplyRGBpaintVat(RGBcolour, volume, fileName);
	}
	else
	{
		cout << RGBcolour << " Paint Vat is at " << remainingVolume << "% Capacity" << endl;
	}
}
void PaintMachine::readRGBpaintVat(string fileName)
{
	string RED;
	string BLUE;
	string GREEN;
	//string fileName = "RGBPaintVats.txt";
	string temp;
	
	ifstream fin;
	fin.open(fileName);

	if (fin.is_open())
	{
		
		getline(fin, RED);
		int REDLength = RED.length() - 3;
		string REDResult = RED.substr(3, REDLength);
		int REDConversion = stoi(REDResult);
		setpaintVolumeRED(REDConversion);

		getline(fin, GREEN);
		int GREENLength = GREEN.length() - 3;
		string GREENResult = GREEN.substr(3, GREENLength);
		int GREENConversion = stoi(GREENResult);
		setpaintVolumeGREEN(GREENConversion);

		getline(fin, BLUE);
		int BLUELength = BLUE.length() - 3;
		string BLUEResult = BLUE.substr(3, BLUELength);
		int BLUEConversion = stoi(BLUEResult);
		setpaintVolumeBLUE(BLUEConversion);

		fin.close();
	}
	else
	{
		cout << "Error reading RGB Paint Vats" << endl;
	}

}
void PaintMachine::updateRGBpaintVat(string fileName)
{
	ofstream fout;
	//string fileName = "RGBPaintVats.txt";

	fout.open(fileName);

	if (fout.is_open())
	{
		int newREDvolume = getpaintVolumeRED() - getRED();
		fout << "R: " << newREDvolume << endl;
		
		int newGREENvolume = getpaintVolumeGREEN() - getGREEN();
		fout << "G: " << newGREENvolume << endl;

		int newBLUEvolume = getpaintVolumeBLUE() - getBLUE();
		fout << "B: " << newBLUEvolume << endl;
		
		fout.close();
	}
	else
	{
		cout << "Error updating RGB Paint Vats" << endl;
	}
}

void PaintMachine::resupplyRGBpaintVat(string vat, int amount, string fileName)
{
	ofstream fout;
	//string fileName = "RGBPaintVats.txt";

	fout.open(fileName);

	if (fout.is_open())
	{
		if (vat == "RED")
		{
			int newREDvolume = amount;
			fout << "R: " << newREDvolume << endl;

			int newGREENvolume = getpaintVolumeGREEN();
			fout << "G: " << newGREENvolume << endl;

			int newBLUEvolume = getpaintVolumeBLUE();
			fout << "B: " << newBLUEvolume << endl;
		}
		else if (vat == "GREEN")
		{
			int newREDvolume = getpaintVolumeRED();
			fout << "R: " << newREDvolume << endl;

			int newGREENvolume = amount;
			fout << "G: " << newGREENvolume << endl;

			int newBLUEvolume = getpaintVolumeBLUE();
			fout << "B: " << newBLUEvolume << endl;
		}
		else
		{
			int newREDvolume = getpaintVolumeRED();
			fout << "R: " << newREDvolume << endl;

			int newGREENvolume = getpaintVolumeGREEN();
			fout << "G: " << newGREENvolume << endl;

			int newBLUEvolume = amount;
			fout << "B: " << newBLUEvolume << endl;
		}

		fout.close();
	}
	else
	{
		cout << "Error updating RGB Paint Vats" << endl;
	}
}

int PaintMachine::checkValidresupply(int volume, string RGBcolour)
{
	bool running = true;
	int paintVolume;
	if (RGBcolour == "RED")
	{
		paintVolume = getpaintVolumeRED();
	}
	else if (RGBcolour == "BLUE")
	{
		paintVolume = getpaintVolumeBLUE();
	}
	else
	{
		paintVolume = getpaintVolumeGREEN();
	}

	while (running)
	{
		if (isdigit(volume))
		{
			if (volume > 0 && volume <= getmaxpaintVolume() && volume + paintVolume < getmaxpaintVolume())
			{
				running = false;
			}
			else
			{
				cout << "Paint Volume is not between range of " << 0 << "-" << getmaxpaintVolume() << endl;
				cout << "Enter Paint Volume" << endl;
				cin >> volume;
			}
		}
		else
		{
			cout << "Invalid Paint Volume entered" << endl;
			cout << "Enter Paint Volume:" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> volume;
		}
	}
	return volume;
}

void PaintMachine::identifyRGBvalues(string fileName)
{
	int position;
	int RGBvalue;
	///string filename = "PlantColours.txt";
	string input;
	
	ifstream fin;
	fin.open(fileName);

	if (fin.is_open())
	{
		while (!fin.eof())
		{
			getline(fin, input);
			position = input.find(":");
			string result = input.substr(0, position);
			
			if (result == getcolour())
			{
				result = input.substr(position + 3, 1);
				RGBvalue = stoi(result);
				setRED(RGBvalue);

				result = input.substr(position + 6, 1);
				RGBvalue = stoi(result);
				setGREEN(RGBvalue);

				result = input.substr(position + 9, 1);
				RGBvalue = stoi(result);
				setBLUE(RGBvalue);

				break;
			}
		}
		fin.close();
	}
	else
	{
		cout << "Error Identifying Plant Colours" << endl;
	}
}

int PaintMachine::getRED()
{
	return RED;
}

void PaintMachine::setRED(int amount)
{
	this->RED = amount;
}

int PaintMachine::getBLUE()
{
	return BLUE;
}

void PaintMachine::setBLUE(int amount)
{
	this->BLUE = amount;
}

int PaintMachine::getGREEN()
{
	return GREEN;
}

void PaintMachine::setGREEN(int amount)
{
	this->GREEN = amount;
}

void PaintMachine::startMachine(DryingChamber* dryingChamber, PaintChamber* paintChamber, DipTank* dipTank)
{
	//dryingChamber->s;
	dipTank->startDipTank("DipTankTemperature.txt","DipTankFluidLevel.txt");
	paintChamber->startPaintChamber("PaintChamberTemperature.txt", "PaintChamberHumidity.txt");
	setmaxpaintVolume(500);

	identifyRGBvalues("PlantColours.txt");
	readRGBpaintVat("RGBPaintVats.txt");
	validatepaintVolume("RED","RGBPaintVats.txt");
	validatepaintVolume("GREEN","RGBPaintVats.txt");
	validatepaintVolume("BLUE","RGBPaintVats.txt");
	updateRGBpaintVat("RGBPaintVats.txt");
	dryingChamber->startDryingChamber("DryingChamberTemperature.txt", "DryingChamberHumidity.txt");

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

double PaintChamber::readTemperature(string fileName)
{
	double result = 0;
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
		setTemperature(result);
		fin.close();
	}
	else
	{
		cout << "Error reading temperature of Paint Chamber" << endl;
	}

	return result;
}

int PaintChamber::readHumidity(string fileName)
{
	int result = 0;
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
		setHumidity(result);
		fin.close();
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
		if (!cin.fail())
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
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

void PaintChamber::startPaintChamber(string temperatureFile, string humidityFile)
{
	//setmaximumTemperature(24);
	//setminimumTemperature(19);
	//setmaximumHumidity(50);
	//setminimumHumidity(40);

	double temp = readTemperature(temperatureFile);
	setTemperature(temp);
	validateTemperature();

	int humidity = readHumidity(humidityFile);
	setHumidity(humidity);
	validateHumidity();
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

double DryingChamber::readTemperature(string fileName)
{
	double result = 0;
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
		setTemperature(result);
		fin.close();
	}
	else
	{
		cout << "Error reading temperature of Drying Chamber" << endl;
	}

	return result;
}

int DryingChamber::readHumidity(string fileName)
{
	int result = 0;
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
		setHumidity(result);
		fin.close();
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
		if (!cin.fail())
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
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> humidity;
		}
	}
}

void DryingChamber::startDryingChamber(string temperatureFile, string humidityFile)
{
	//setmaximumTemperature(24);
	//setminimumTemperature(19);
	//setmaximumHumidity(50);
	//setminimumHumidity(40);

	double temp = readTemperature(temperatureFile);
	setTemperature(temp);
	validateTemperature();

	int humidity = readHumidity(humidityFile);
	setHumidity(humidity);
	validateHumidity();
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

int DipTank::getmaximumfluidLevel()
{
	return this->maximumfluidLevel;
}

void DipTank::setmaximumfluidLevel(int level)
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

double DipTank::readTemperature(string fileName)
{
	double result = 0;
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
		setTemperature(result);
		fin.close();
	}
	else
	{
		cout << "Error reading temperature of Dip Tank" << endl;
	}

	return result;
}

void DipTank::validateTemperature(void)
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
		cout << "Current Temperature of Dip Tank: " << temp << endl;
	}
}

void DipTank::updateTemperature(double temp)
{

	bool running = true;

	while (running)
	{
		if (!cin.fail())
		{
			if (temp > getminimumTemperature() && temp < getmaximumTemperature())
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
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> temp;
		}
	}
}

int DipTank::readfluidLevel(string fileName)
{
	int result = 0;
	string fluidLevel;
	srand((unsigned)time(NULL));
	int random = 1 + (rand() % 100);


	ifstream fin;
	fin.open(fileName);

	if (fin.is_open())
	{
		for (int index = 0; index < random; index++)
		{
			getline(fin, fluidLevel);
		}

		result = stoi(fluidLevel);
		setfluidLevel(result);
		fin.close();
	}
	else
	{
		cout << "Error reading fluid level of Dip Tank" << endl;
	}

	return result;
}

void DipTank::updatefluidLevel(int fluidLevel)
{
	bool running = true;

	while (running)
	{
		if (isdigit(fluidLevel))
		{
			if (fluidLevel > 0 && fluidLevel < getmaximumfluidLevel())
			{
				setTemperature(fluidLevel);
				running = false;
			}
			else
			{
				cout << "Fluid Level not between permitted range"<< endl;
				cout << "Enter Fluid Level:" << endl;
				cin >> fluidLevel;
			}
		}
		else
		{
			cout << "Invalid Fluid level entered" << endl;
			cout << "Enter Fluid level:" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> fluidLevel;
		}
	}
}

void DipTank::validatefluidLevel(void)
{
	int fluidLevel = getfluidLevel();
	int input;
	int lowVolume = (getmaximumfluidLevel() * 20) / 100;

	if (fluidLevel<= lowVolume)
	{
		cout << "[WARNING] Current Fluid level of: " << fluidLevel << "is below minimum permitted level of " << getmaximumfluidLevel() << endl;
		cout << "Enter Fluid level:" << endl;
		cin >> input;
		updatefluidLevel(input);
	}
	else if (fluidLevel > getmaximumfluidLevel())
	{
		cout << "[WARNING] Current Fluid level of: " << fluidLevel << "is above maximum permitted level of " << getmaximumfluidLevel() << endl;
		cout << "Enter Fluid level:" << endl;
		cin >> input;
		updatefluidLevel(input);
	}
	else
	{
		cout << "Current Fluid level of Dip Tank: " << fluidLevel << endl;
	}
}

void DipTank::startDipTank(string temperatureFile, string fluidFile)
{
	//setmaximumfluidLevel(50);
	//setmaximumTemperature(24);
	//setminimumTemperature(19);

	double temp = readTemperature(temperatureFile);
	setTemperature(temp);
	validateTemperature();

	int level = readfluidLevel(fluidFile);
	setfluidLevel(level);
	validatefluidLevel();
}