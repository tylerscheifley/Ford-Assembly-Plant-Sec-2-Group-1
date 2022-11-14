#pragma once
#include <iostream>


using namespace std;

class Order
{

	string year;
	string make;
	string trim;
	string model;
	string colour;
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
	bool saveOrder(FILE);
	bool loadOrder(FILE);

};
