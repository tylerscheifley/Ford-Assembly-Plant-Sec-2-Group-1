#include <iostream>
#include "PaintMachine.h"
using namespace std;

int main(void)
{
	cout << "PaintMachine" << endl;

	PaintChamber p;

	p.setmaximumTemperature(24.0);
	p.setminimumTemperature(19.0);

	double result;

	result = p.readTemperature();

	cout << "Temperature: " << result << endl;

	return 0;
}