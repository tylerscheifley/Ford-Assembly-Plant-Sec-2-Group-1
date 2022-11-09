// PaintMachine.h
// Ford Assembly Plant: PaintMachine
// Created: 11/08/22
// Author: Tyler Scheifley

#pragma once
#include <iostream>
#include <string.h>
using namespace std;


class DipTank
{
private:

	int fluidLevel;
	double temperature;
	double minimumTemperature;
	double maximumTemperature;
	int maximumfluidLevel;

public:

	DipTank();
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

	PaintChamber();
	void setminimumTemperature(double minTemp);
	double getminimumTemperature();
	void setTemperature(double temperature);
	double getTemperature();
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

	DryingChamber();
	void setminimumTemperature(double minTemp);
	double getminimumTemperature();
	void setTemperature(double temperature);
	double getTemperature();
	void setmaximumTemperature(double maxTemp);
	double getmaximumTemperature();
	void setminimumHumidity(double humidity);
	double getminimumHumidity();
	void setHumidity(double humidity);
	double getHumidity();
	void setmaximumHumidity(double humidity);
	double getmaximumHumidity();
};

class PaintMachine
{
private:

	string colour;
	int paintVolumeRED;
	int paintVolumeBLUE;
	int paintVolumeGREEN;
	int maxpaintVolume;
	//DryingChamber* dryingChamber;
	//PaintChamber* paintChamber;
	//DipTank* dipTank;

public:

	PaintMachine();
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

};


