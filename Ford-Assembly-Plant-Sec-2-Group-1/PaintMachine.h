// PaintMachine.h
// Ford Assembly Plant: PaintMachine
// Created: 11/08/22
// Author: Tyler Scheifley

#pragma once
#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>

using namespace std;


class DipTank
{
public:
	int fluidLevel;
	double temperature;
	double minimumTemperature;
	double maximumTemperature;
	int maximumfluidLevel;
	DipTank();
	void startDipTank(string temperatureFile, string fluidFile);
	void setfluidLevel(int fluidLevel);
	int getfluidLevel();
	void setTemperature(double temperature);
	double getTemperature();
	void setmaximumTemperature(double maxTemp);
	double getmaximumTemperature();
	void setminimumTemperature(double minTemp);
	double getminimumTemperature();
	void setmaximumfluidLevel(int maxfluid);
	int getmaximumfluidLevel();
	void updateTemperature(double temp);
	void validateTemperature(void);
	double readTemperature(string filename);
	int readfluidLevel(string fileName);
	void updatefluidLevel(int fluidLevel);
	void validatefluidLevel(void);

	
};

class PaintChamber
{

public:
	int humidity;
	double temperature;
	int minimumHumidity;
	int maximumHumidity;
	double minimumTemperature;
	double maximumTemperature;
	PaintChamber();
	void startPaintChamber(string temperatureFile, string humidityFile);
	void setminimumTemperature(double minTemp);
	double getminimumTemperature();
	void setTemperature(double temperature);
	double getTemperature();
	void setmaximumTemperature(double maxTemp);
	double getmaximumTemperature();
	void setminimumHumidity(int humidity);
	int getminimumHumidity();
	void setHumidity(int humidity);
	int getHumidity();
	void setmaximumHumidity(int humidity);
	int getmaximumHumidity();
	double readTemperature(string fileName);
	int readHumidity(string fileName);
	void validateTemperature(void);
	void validateHumidity(void);
	void updateTemperature(double temp);
	void updateHumidity(int humidity);
};

class DryingChamber
{

public:
	int humidity;
	double temperature;
	int minimumHumidity;
	int maximumHumidity;
	double minimumTemperature;
	double maximumTemperature;
	DryingChamber();
	void startDryingChamber(string temperatureFile, string humidityFile);
	void setminimumTemperature(double minTemp);
	double getminimumTemperature();
	void setTemperature(double temperature);
	double getTemperature();
	void setmaximumTemperature(double maxTemp);
	double getmaximumTemperature();
	void setminimumHumidity(int humidity);
	int getminimumHumidity();
	void setHumidity(int humidity);
	int getHumidity();
	void setmaximumHumidity(int humidity);
	int getmaximumHumidity();
	double readTemperature(string fileName);
	int readHumidity(string fileName);
	void validateTemperature(void);
	void validateHumidity(void);
	void updateTemperature(double temp);
	void updateHumidity(int humidity);
};

class PaintMachine
{
private:

	string colour;
	int paintVolumeRED;
	int paintVolumeBLUE;
	int paintVolumeGREEN;
	int maxpaintVolume;
	int RED;
	int GREEN;
	int BLUE;
	DryingChamber* dryingChamber;
	PaintChamber* paintChamber;
	DipTank* dipTank;

public:

	PaintMachine(DryingChamber* dryingChamber, PaintChamber* paintChamber, DipTank* dipTank);
	void startMachine(DryingChamber* dryingChamber, PaintChamber* paintChamber, DipTank* dipTank);
	void setcolour(string colour);
	string getcolour();
	void setpaintVolumeBLUE(int paintVolume);
	int getpaintVolumeBLUE();
	void setpaintVolumeRED(int paintVolume);
	int getpaintVolumeRED();
	void setpaintVolumeGREEN(int paintVolume);
	int getpaintVolumeGREEN();
	void setmaxpaintVolume(int max);
	int getmaxpaintVolume();
	void validatepaintVolume(string RGBcolor, string fileName);
	int checkValidresupply(int volume, string RGBcolor);
	void identifyRGBvalues(string fileName);
	void readRGBpaintVat(string fileName);
	void updateRGBpaintVat(string fileName);
	void resupplyRGBpaintVat(string vat, int volume, string fileName);
	void setRED(int amount);
	void setGREEN(int amount);
	void setBLUE(int amount);
	int getRED(void);
	int getGREEN(void);
	int getBLUE(void);
};


