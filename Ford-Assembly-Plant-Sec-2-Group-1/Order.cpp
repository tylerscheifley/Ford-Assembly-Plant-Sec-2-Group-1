#include "Order.h"

void Order::setColour(string colour) {
	this->colour = colour;
}
string Order::getColour(void) {
	return this->colour;
}
void Order::setModel(string model) {
	this->model = model;
}
string Order::getModel(void) {
	return this->model;
}

void Order::setYear(string year) {
	this->year = year;
}
string Order::getYear(void) {
	return this->year;
}
void Order::setMake(string make) {
	this->make = make;
}
string Order::getMake(void) {
	return this->make;
}
void Order::setTrim(string trim) {
	this->trim = trim;
}
string Order::getTrim(void) {
	return this->trim;
}

void Order::setDestination(string dest) {
	this->destination = dest;
}
string Order::getDestination(void) {
	return this->destination;
}

bool Order::saveOrder(FILE file) {

}

bool Order::loadOrder(FILE file) {

}