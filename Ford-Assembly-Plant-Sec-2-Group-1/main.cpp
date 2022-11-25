#include "BodyMachine.h"
#include "ChassisMachine.h"
#include "InteriorMachine.h"
#include "PaintMachine.h"
#include "plant.h"

using namespace std;

string* pathFinder(Order order, string *container[])
{
	if (bodyMachineFinder(order.getYear(), order.getBodyPanelSet(), order.getModel()) != "DNE")
	{
		*container[0] = bodyMachineFinder(order.getYear(), order.getBodyPanelSet(), order.getModel());
	}

	*container[1] = "..\\GUI\\ChassisMachine\\" + order.getYear() + " " + order.getInteriorLevel() + " " + order.getModel() + ".png";
	*container[2] = "..\\GUI\\ChassisMachine\\" + order.getYear() + " " + order.getBodyPanelSet() + " " + order.getModel() + " " + order.getColour() + ".png";
	*container[3] = "..\\GUI\\ChassisMachine\\" + order.getYear() + " " + order.getModel() + " " + order.getTrim() + " " + order.getColour() + ".jpg";

	return *container;
}

string bodyMachineFinder(string year, string body, string model)
{
	if (year == "2022")
	{
		if (model == "F150")
		{
			if (body == "Regular")
			{
				return "..\\GUI\\BodyMachine\\RegF150.png";
			}
			else if (body == "SuperCab")
			{
				return "..\\GUI\\BodyMachine\\SprCabF150.png";
			}
			else if (body == "SuperCrew")
			{
				return "..\\GUI\\BodyMachine\\SprCrwF150.png";
			}
		}
		return "DNE";
	}
	else if (year == "2023")
	{
		if (model == "Expedition")
		{
			if (body == "Regular")
			{
				return "..\\GUI\\BodyMachine\\RegExp.png";
			}
			else if (body == "Max")
			{
				return "..\\GUI\\BodyMachine\\MaxExp.png";
			}
		}
		return "DNE";
	}
	else
	{
		return "DNE";
	}
}

int main(void)
{
	return 0;
}