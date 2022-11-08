// PaintMachine.h
// Ford Assembly Plant: PaintMachine
// Created: 11/08/22
// Author: Tyler Scheifley

#pragma once
#include <iostream>
using namespace std;

class PaintMachine
{
private:
	string colour;
	//string paintType;
	int paintVolume;
	int maxpaintVolume;

public:
	void setcolour(string colour);
	string getcolour();
	//void setpaintType(string paintType);
	//string getpaintType();
	void setpaintVolume(int paintVolume);
	int getpaintVolume();
	void setmaxpaintVolume(int maxpaintVolume);
	int getmaxpaintVolume();
	//load random values, save: write changes taken out
};

class DipTank
{
private:
	int fluidLevel;
	double temperature;
	double minimumTemperature;
	double maximumTemperature;
	int maximumfluidLevel;

public:
	void setfluidLevel(int fluidLevel);
	int getfluidLevel();
	void settemperature(double temperature);
	double gettemperature();
	void setmaximumTemperature(double maxTemp);
	double getmaximumTemperature();
	void setminimumTemperature(double minTemp);
	double getminimumTemperature();
	void setmaximumfluidLevel(double maxfluid);
	double getmaximumfluidLevel();
	
};

class PaintChamber
{
private:
	double humidity;
	double temperature;
	double minimumHumidity;
	double maximumHumidity;
	double minimumTemperature;
	double maximumTemperature;

public:
	void setminimumTemperature(double minTemp);
	double getminimumTemperature();
	void settemperature(double temperature);
	double gettemperature();
	void setmaximumTemperature(double maxTemp);
	double getmaximumTemperature();

	void setminimumHumidity(double humidity);
	double getminimumHumidity();
	void setHumidity(double humidity);
	double getHumidity();
	void setmaximumHumidity(double humidity);
	double getmaximumHumidity();
};

class DryingChamber
{
private:
	double humidity;
	double temperature;
	double minimumHumidity;
	double maximumHumidity;
	double minimumTemperature;
	double maximumTemperature;
public:

};

