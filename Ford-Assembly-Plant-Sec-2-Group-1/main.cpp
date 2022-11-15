#include <iostream>
#include "PaintMachine.h"
using namespace std;

int main(void)
{
	//cout << "PaintMachine" << endl;

	//PaintChamber p;

	//p.setmaximumTemperature(24.0);
	//p.setminimumTemperature(19.0);

	//double result;

	//result = p.readTemperature();

	//cout << "Temperature: " << result << endl;

	PaintMachine p;
	p.setcolour("Stone Grey Metallic");
	p.identifyRGBvalues();

	return 0;
}