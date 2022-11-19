#pragma once
#include "Order.h"
#include <iostream>
#include <string>


using namespace std;

class Vehicle
{
	int count;
	Order order;
	bool QAQC;
	string colour;
	string body;
	string chassis;
	string interior;
	string year;
	string make;
	string trim;
	string model;
	string VIN;
	string date;

public:
	int getCount();
	void setCount(int);
	Order getOrder();
	void setOrder(Order);
	string getColour();
	void setColour(string);
	string getBody();
	void setBody(string);
	string getChassis();
	void setChassis(string);
	string getInterior();
	void setInterior(string);
	string getYear();
	void setYear(string);
	string getMake();
	void setMake(string);
	string getTrim();
	void setTrim(string);
	string getModel();
	void setModel(string);
	string getVIN();
	void setVIN(string);
	string getDate();
	void setDate(string);
	string GenerateVIN(void);
	bool checkQAQC();
	
	
};

