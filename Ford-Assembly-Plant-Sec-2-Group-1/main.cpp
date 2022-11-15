#include <iostream>
#include "Vehicle.h"
using namespace std;

int main(void)
{

	
	for (int i = 0; i < 1660; i++) {
		Order order;
		if (order.saveOrder("Order.txt")) {
			cout << "File Saved" << endl;
		}
		else {
			cout << "File did not save" << endl;
		}
	}

	return 0;
}