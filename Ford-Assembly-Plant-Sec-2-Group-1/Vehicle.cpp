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

}

bool Vehicle::checkQAQC() {

}

Vehicle::Vehicle() {

}

Vehicle::Vehicle(int count, Order order, bool QAQC, string colour, string body, string chassis, string interior, string year, string make, string trim, string model, string VIN, string date) {

}