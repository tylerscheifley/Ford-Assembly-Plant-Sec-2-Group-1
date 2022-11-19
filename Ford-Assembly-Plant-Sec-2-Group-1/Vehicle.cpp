#include "Vehicle.h"

int Vehicle::getCount(void) {
	return this->count;
}
void Vehicle::setCount(int count) {
	this->count = count;
}
Order Vehicle::getOrder(void) {
	return this->order;
}
void Vehicle::setOrder(Order order) {
	this->order = order;
}
string Vehicle::getColour(void) {
	return this->colour;
}
void Vehicle::setColour(string colour) {
	this->colour = colour;
}
string Vehicle::getBody(void) {
	return this->body;
}
void Vehicle::setBody(string body) {
	this->body = body;
}
string Vehicle::getChassis(void) {
	return this->chassis;
}
void Vehicle::setChassis(string chassis) {
	this->chassis = chassis;
}
string Vehicle::getInterior(void) {
	return this->interior;
}
void Vehicle::setInterior(string interior) {
	this->interior = interior;
}
string Vehicle::getYear(void) {
	return this->year;
}
void Vehicle::setYear(string year) {
	this->year = year;
}
string Vehicle::getMake(void) {
	return this->make;
}
void Vehicle::setMake(string make) {
	this->make = make;
}
string Vehicle::getTrim(void) {
	return this->trim;
}
void Vehicle::setTrim(string trim) {
	this->trim = trim;
}
string Vehicle::getModel(void) {
	return this->model;
}
void Vehicle::setModel(string model) {
	this->model = model;
}
string Vehicle::getVIN(void){
	return this->VIN;
}
void Vehicle::setVIN(string vin) {
	this->VIN = vin;
}
string Vehicle::getDate(void) {
	return this->date;
}
void Vehicle::setDate(string date) {
	this->date = date;
}


string Vehicle::GenerateVIN(void) {
	string vin;

	vin += getModel();
	vin += getYear();
	int count = getCount();
	string sCount = to_string(count);
	vin += sCount;

	while (vin.length() != 17) {
		vin.insert(vin.length() - sCount.length(), "0");
	}

	return vin;
	
}

bool Vehicle::checkQAQC() {

	bool qaqc = false;

	if (getYear() == this->order.getYear()) {
		qaqc = true;
		if (getMake() == this->order.getMake()) {
			qaqc = true;
			if (getTrim() == this->order.getTrim()) {
				qaqc = true;
				if (getModel() == this->order.getModel()) {
					qaqc = true;
					if (getColour() == this->order.getColour()) {
						qaqc = true;
					
					}
				}
			}
		}

	}

	return qaqc;

}




