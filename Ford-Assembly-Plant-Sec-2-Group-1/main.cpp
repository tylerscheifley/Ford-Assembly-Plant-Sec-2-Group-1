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
	DryingChamber d;
	PaintChamber paint;
	DipTank Dip;
	PaintMachine p(&d,&paint,&Dip);
	p.setcolour("Antimatter Blue");
	p.startMachine(&d,&paint,&Dip);
	//p.identifyRGBvalues();

	return 0;
}