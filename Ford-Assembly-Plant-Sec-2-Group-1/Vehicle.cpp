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
	if (getModel() == "Expedition") {
		vin += "EXP";
	}
	else {
		vin += "F150";
	}
	
	vin += getYear();
	int count = getCount();
	string sCount = to_string(count);
	vin += sCount;

	while (vin.length() != 17) {
		vin.insert(vin.length() - sCount.length(), "0");
	}

	return vin;
	
}


bool Vehicle ::LogCompletedVehicle(string fileName) {
	ofstream fout;

	fout.open(fileName, ios::out | ios::app);

	if (fout.is_open()) {

		
		fout << getMake() << "|" << getYear() << "|" << getModel() << "|" << getTrim() << "|" << getBody() << "|" << getColour() << "|" << getChassis() << "|" << getInterior() << "|" << getVIN() << "|" << getDate()  << endl;


		fout.close();
		return true;
	}
	else {
		cout << "ERROR" << endl;
		return false;
	}
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



bool Vehicle::generateCount(string fileName) {
	ifstream fin;
	string line = " ";
	int F150count = 0;
	int Expcount = 0;
	fin.open(fileName, ios::in);

	if (fin.is_open()) {



		while (fin.peek() != EOF) {
			getline(fin, line);
			vector<string> v;
			
			stringstream ss(line);

			
			while (ss.good()) {
				string substr;
				getline(ss, substr, '|');
				v.push_back(substr);
			}

			if (v[2] == "Expedition") {
				Expcount++;
			}
			else {
				F150count++;
			}
		}
		if (getModel() == "Expedition") {
			setCount(Expcount);
		}
		else {
			setCount(F150count);
		}
		return true;
	}
	else {
		return false;
	}
}

