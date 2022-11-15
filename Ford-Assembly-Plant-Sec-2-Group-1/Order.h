#pragma once
#include <iostream>
#include <fstream>
#include <array>

using namespace std;

class Order
{

	string year;
	string make;
	string trim;
	string model;
	string colour;
	string bodyPanelSet;
	string engineType;
	string interiorLevel;
	string destination;
public:

	string getYear();
	void setYear(string);
	string getMake();
	void setMake(string);
	string getTrim();
	void setTrim(string);
	string getModel();
	void setModel(string);
	string getColour(void);
	void setColour(string);
	string getDestination(void);
	void setDestination(string);
	string getBodyPanelSet();
	void setBodyPanelSet(string);
	string getEngineType(void);
	void setEngineType(string);
	string getInteriorLevel(void);
	void setInteriorLevel(string);
	Order();
	bool saveOrder(string);
	bool loadOrder(string);

};
