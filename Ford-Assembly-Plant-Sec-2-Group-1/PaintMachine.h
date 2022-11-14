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
private:

	int fluidLevel;
	double temperature;
	double minimumTemperature;
	double maximumTemperature;
	double maximumfluidLevel;

public:

	DipTank();
	void setfluidLevel(int fluidLevel);
	int getfluidLevel();
	void setTemperature(double temperature);
	double getTemperature();
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

	int humidity;
	double temperature;
	int minimumHumidity;
	int maximumHumidity;
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
	void setminimumHumidity(int humidity);
	int getminimumHumidity();
	void setHumidity(int humidity);
	int getHumidity();
	void setmaximumHumidity(int humidity);
	int getmaximumHumidity();
	double readTemperature(void);
	double readHumidity(void);
};

class DryingChamber
{
private:

	int humidity;
	double temperature;
	int minimumHumidity;
	int maximumHumidity;
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
	void setminimumHumidity(int humidity);
	int getminimumHumidity();
	void setHumidity(int humidity);
	int getHumidity();
	void setmaximumHumidity(int humidity);
	int getmaximumHumidity();
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
	void validatepaintVolume(string RGBcolor);

};


