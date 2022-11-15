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

string Order::getBodyPanelSet(void) {
	return this->bodyPanelSet;
}
void Order::setBodyPanelSet(string bps) {
	this->bodyPanelSet = bps;
}
string Order::getEngineType(void) {
	return this->engineType;
}
void Order::setEngineType(string engine) {
	this->engineType = engine;
}
string Order::getInteriorLevel(void) {
	return this->interiorLevel;
}
void Order::setInteriorLevel(string level) {
	this->interiorLevel = level;
}

Order::Order() {

	string years[2] = { "2022", "2023" };

	string model[2] = { "F150", "Expedition" };

	string F150Trims[8] = { "RAPTOR", "TREMOR", "LIMITED",
	"PLATINUM", "KING RANCH", "LARIAT", "XLT", "XL" };

	string superCabtrims[] = { "LARIAT", "XLT", "XL" };
	string ExpeditionTrims[5] = { "XLT", "LIMITED","TIMBERLINE", "KING RANCH", "PLATINUM" };

	string F150Body[3] = { "Regular", "SuperCab", "SuperCrew" };
	string regCabTrims[] = { "XL", "XLT" };

	string ExpeditionBodys[2] = { "MAX", "REG" };

	string ExpeditionChass[2] = { "3.5L Ecoboost","3.5L Ecoboost High Output" };

	string F150Chass[5] = { "3.3L V6", "2.7L Ecoboost V6", "5.0L V8", "3.5L Ecoboost V6", "3.5L Powerboost V6" };

	string ExpeditionEngine[] = { "35LV6C", "35LV6HOC" };

	string F150Engine[] = { "27LV6C", "33LV6C", "35LV6EcoC", "35LV6PwrBstC", "50LV8C" };

	string InteriorLevels[] = { "base", "mid", "high" };
	string F150XLReg[] = { "Agate Black Metallic", "Antimatter Blue Metallic"
	,"Carbonized Grey Metallic", "Iconic Silver Metallic", "Oxford White"
	,"Race Red", "Stone Grey Metallic", "Atlas Blue Metallic" };


	string F150XLSup[] = { "Agate Black Metallic"
	"Antimatter Blue Metallic", "Carbonized Grey Metallic", "Iconic Sliver Metallic",
	"Oxford White", "Race Red", "Stone Grey Metallic", "Atlas Blue Metallic", "Space White Metallic" };

	string F150XLSupCrew[] = { "Agate Black Metallic", "Antimatter Blue Metallic", "Carbonized Grey Metallic", "Iconic Sliver Metallic", "Oxford White", "Race Red",
		"Stone Grey Metallic", "Atlas Blue Metallic", "Space White Metallic" };


	string F150XLTReg[] = { "Agate Black Metallic", "Antimatter Blue"
	,"Carbonized Grey Metallic", "Iconic Silver Metallic", "Oxford White"
	,"Race Red", "Stone Grey Metallic", "Space White Metallic", "Rapid Red Metallic Tinted Clearcoat","Atlas Blue Metallic" };


	string F150XLTSup[] = { "Agate Black Metallic"
	"Antimatter Blue Metallic", "Carbonized Grey Metallic", "Iconic Sliver Metallic",
	"Oxford White", "Race Red", "Stone Grey Metallic","Space White Metallic","Rapid Red Metallic Tinted Clearcoat", "Atlas Blue Metallic" };


	string F150XLTCrew[] = { "Agate Black Metallic", "Antimatter Blue Metallic", "Carbonized Grey Metallic", "Iconic Silver Metallic", "Oxford White", "Race Red",
	"Stone Grey Metallic","Space White Metallic", "Rapid Red Metallic Tinted Clearcoat", "Atlas Blue Metallic" };


	string F150LarSup[] = { "Agate Black Metallic", "Antimatter Blue", "Carbonized Grey Metallic", "Iconic Silver Metallic", "Oxford White", "Race Red",
	"Stone Grey Metallic", "Rapid Red Metallic Tinted Clearcoat Metallic", "Star White Metallic Tri-Coat", "Atlas Blue Metallic" };


	string F150LarCrew[] = { "Agate Black Metallic", "Carbonized Grey Metallic", "Iconic Silver Metallic", "Oxford White", "Race Red",
		"Stone Grey Metallic", "Space White Metallic","Rapid Red Metallic Tinted Clearcoat Metallic","Smoked Quartz Tinted ClearCoat Metallic", "Star White Metallic Tri-Coat", "Atlas Blue Metallic" };


	string F150KingRanchCrew[] = { "Agate Black Metallic", "Antimatter Blue", "Iconic Silver Metallic", "Oxford White", "Stone Grey Metallic",
			"Rapid Red Metallic Tinted Clearcoat", "Smoked Quartz Clearcoat Metallic", "Star White Metallic Tri-Coat" };


	string F150PlatCrew[] = { "Agate Black Metallic", "Antimatter Blue Metallic", "Carbonized Grey Metallic", "Iconic Silver Metallic",
		"Smoked Quartz Tinted Clearcoat Metallic", "Star White Metallic Tri-Coat" };


	string F150LimitCrew[] = { "Agate Black Metallic", "Antimatter Blue Metallic", "Carbonized Grey Metallic", "Iconic Silver Metallic", "Smoked Quartz Tinted Clearcoat Metallic", "Star White Metallic Tri-Coat" };


	string F150TremorCrew[] = { "Agate Black Metallic", "Antimattter Blue Metallic", "Iconic Silver Metallic", "Oxford White", "Stone Grey Metallic", "Atlas Blue Metallic" };


	string F150RaptorCrew[] = { "Agate Black Metallic", "Antimatter Blue Metallic", "Iconic Silver Metallic", "Oxford White", "Rapid Red Metallic Tinted Clearcoat", "Code Orange Metallic", "Atlas Blue Metallic" };

	string ExpXLTReg[] = { "Agate Black", "Oxford White", "Stone Blue Metallic", "Jewel Red Metallic Tinted Clearcoat", "Infinite Blue Metallic Tinted Clearcoat", "Dark Matter Metallic", "Forged Green Metallic"
	, "Star White Platinum Metallic Tri-Coat", "Iconic Silver" };

	string ExpLimReg[] = { "Agate Black", "Oxford White", "Stone Blue Metallic", "Jewel Red Metallic Tinted Clearcoat",
		"Infinite Blue Metallic Tinted Clearcoat", "Dark Matter Metallic", "Forged Green Metallic", "Star White Platinum Metallic Tri-Coat","Iconic Sliver" };

	string ExpLimMAX[] = { "Agate Black", "Oxford White", "Stone Blue Metallic", "Jewel Red Metallic Tinted Clearcoat",
		"Infinite Blue Metallic Tinted Clearcoat", "Dark Matter Metallic", "Forged Green Metallic", "Star White Platinum Metallic Tri-Coat","Iconic Sliver" };

	string ExpTimReg[] = { "Agate Black", "Oxford White", "Stone Blue Metallic",
		"Infinite Blue Metallic Tinted Clearcoat", "Dark Matter Metallic", "Forged Green Metallic", "Star White Platinum Metallic Tri-Coat","Iconic Sliver" };

	string ExpKRReg[] = { "Agate Black", "Oxford White", "Jewel Red Metallic Tinted Clearcoat", "Infinite Blue Metallic Tinted Clearcoat", "Forged Green Metallic", "Star White Platinum Metallic Tri-Coat" };

	string ExpKRMAX[] = { "Agate Black", "Oxford White", "Jewel Red Metallic Tinted Clearcoat", "Infinite Blue Metallic Tinted Clearcoat", "Forged Green Metallic", "Star White Platinum Metallic Tri-Coat" };

	string ExpPlatReg[] = { "Agate Black", "Stone Blue Metallic", "Jewel Red Metallic Tinted Clearcoat", "Infinite Blue Metallic Tinted Clearcoat", "Dark Matter Metallic", "Star White Platinum Metallic Tri-Coat", "Iconic Silver" };

	string ExpPlatMAX[] = { "Agate Black", "Stone Blue Metallic", "Jewel Red Metallic Tinted Clearcoat", "Infinite Blue Metallic Tinted Clearcoat", "Dark Matter Metallic", "Star White Platinum Metallic Tri-Coat", "Iconic Silver" };
	string Destinations[] = { "Kitchener", "Waterloo", "Walkerton", "Harriston","Owen Sound", "Toronto", "London"};

	int randomVal = 0;
	randomVal = rand();

	srand(clock() % rand()* rand());


	randomVal = rand() % size(years);

	if (years[randomVal] == "2022") {
		setYear(years[0]);
		setMake("FORD");
		setModel(model[0]);
		randomVal = rand() % size(InteriorLevels);

		setInteriorLevel(InteriorLevels[randomVal]);
		
		randomVal = rand() % size(F150Engine);

		setEngineType(F150Engine[randomVal]);

		randomVal = rand() % size(F150Body);

		if (F150Body[randomVal] == "Regular") {
			setBodyPanelSet(F150Body[randomVal]);
			
			randomVal = rand() % size(regCabTrims);
			setTrim(regCabTrims[randomVal]);

			if (getTrim() == "XL") {
				
				randomVal = rand() % size(F150XLReg);

				setColour(F150XLReg[randomVal]);
			}
			else if (getTrim() == "XLT") {
				
				randomVal = rand() % size(F150XLTReg);

				setColour(F150XLTReg[randomVal]);
			}


		}
		if (F150Body[randomVal] == "SuperCab") {
			setBodyPanelSet(F150Body[randomVal]);
			
			randomVal = rand() % size(superCabtrims);
			setTrim(superCabtrims[randomVal]);

			if (getTrim() == "XL") {
				
				randomVal = rand() % size(F150XLSup);

				setColour(F150XLSup[randomVal]);
			}
			if (getTrim() == "XLT") {
				
				randomVal = rand() % size(F150XLTSup);

				setColour(F150XLTSup[randomVal]);
			}
			if (getTrim() == "LARIAT") {
				
				randomVal = rand() % size(F150LarSup);

				setColour(F150LarSup[randomVal]);
			}

		}
		if (F150Body[randomVal] == "SuperCrew") {
			setBodyPanelSet(F150Body[randomVal]);
			
			randomVal = rand() % size(F150Trims);
			setTrim(F150Trims[randomVal]);

			if (getTrim() == "XL") {
				
				randomVal = rand() % size(F150XLSupCrew);

				setColour(F150XLSupCrew[randomVal]);
			}
			if (getTrim() == "XLT") {
				
				randomVal = rand() % size(F150XLTCrew);

				setColour(F150XLTCrew[randomVal]);
			}
			if (getTrim() == "LARIAT") {
				
				randomVal = rand() % size(F150LarCrew);

				setColour(F150LarCrew[randomVal]);
			}
			if (getTrim() == "KING RANCH") {
				
				randomVal = rand() % size(F150KingRanchCrew);

				setColour(F150KingRanchCrew[randomVal]);
			}
			if (getTrim() == "PLATINUM") {
				
				randomVal = rand() % size(F150PlatCrew);

				setColour(F150PlatCrew[randomVal]);
			}
			if (getTrim() == "LIMITED") {
				
				randomVal = rand() % size(F150LimitCrew);

				setColour(F150LimitCrew[randomVal]);
			}
			if (getTrim() == "TREMOR") {
				
				randomVal = rand() % size(F150TremorCrew);

				setColour(F150TremorCrew[randomVal]);
			}
			if (getTrim() == "RAPTOR") {
				
				randomVal = rand() % size(F150RaptorCrew);

				setColour(F150RaptorCrew[randomVal]);
			}

		

		}
		
		randomVal = rand() % size(Destinations);

		setDestination(Destinations[randomVal]);


	}
	if(years[randomVal] == "2023") {
		setYear(years[1]);
		setMake("FORD");
		setModel(model[1]);
		
		randomVal = rand() % size(InteriorLevels);

		
		setInteriorLevel(InteriorLevels[randomVal]);

		
		randomVal = rand() % size(ExpeditionEngine);

		setEngineType(ExpeditionEngine[randomVal]);
		
		randomVal = rand() % size(ExpeditionBodys);

		if (ExpeditionBodys[randomVal] == "MAX") {
			setBodyPanelSet(ExpeditionBodys[0]);
			
			randomVal = rand() % size(ExpeditionTrims);
			setTrim(ExpeditionTrims[randomVal]);

			if (getTrim() == "XLT") {
				
				randomVal = rand() % size(ExpXLTReg);

				setColour(ExpXLTReg[randomVal]);
			}
			if (getTrim() == "LIMITED") {
				
				randomVal = rand() % size(ExpLimMAX);

				setColour(ExpLimMAX[randomVal]);
			}
			if (getTrim() == "TIMBERLINE") {
				
				randomVal = rand() % size(ExpTimReg);

				setColour(ExpTimReg[randomVal]);
			}
			if (getTrim() == "KING RANCH") {
				
				randomVal = rand() % size(ExpKRMAX);

				setColour(ExpKRMAX[randomVal]);
			}
			if (getTrim() == "PLATINUM") {
				
				randomVal = rand() % size(ExpPlatMAX);

				setColour(ExpPlatMAX[randomVal]);
			}

				
		}
		else if (ExpeditionBodys[randomVal] == "REG") {
			setBodyPanelSet(ExpeditionBodys[1]);

			
			randomVal = rand() % size(ExpeditionTrims);
			setTrim(ExpeditionTrims[randomVal]);

			if (getTrim() == "XLT") {
				
				randomVal = rand() % size(ExpXLTReg);

				setColour(ExpXLTReg[randomVal]);
			}
			else if (getTrim() == "LIMITED") {
				
				randomVal = rand() % size(ExpLimReg);

				setColour(ExpLimReg[randomVal]);
			}
			else if (getTrim() == "TIMBERLINE") {
				
				randomVal = rand() % size(ExpTimReg);

				setColour(ExpTimReg[randomVal]);
			}
			else if (getTrim() == "KING RANCH") {
				
				randomVal = rand() % size(ExpKRReg);

				setColour(ExpKRReg[randomVal]);
			}
			else if (getTrim() == "PLATINUM") {
				
				randomVal = rand() % size(ExpPlatReg);

				setColour(ExpPlatReg[randomVal]);
			}

		}
		
		randomVal = rand() % size(Destinations);

		setDestination(Destinations[randomVal]);


	


	}

}

bool Order::saveOrder(string fileName) {
	ofstream fout;

	fout.open(fileName, ios::out | ios::app);

	if (fout.is_open()) {
		
		fout << getMake() << "|" << getYear() << "|" << getModel()<< "|" << getTrim()<< "|" << getBodyPanelSet()<< "|" << getColour() << "|" << getEngineType() << "|" << getInteriorLevel() << "|" << getDestination() << endl;


		fout.close();
		return true;
	}
	else {
		cout << "ERROR" << endl;
		return false;
	}
}

bool Order::loadOrder(string fileName) {
	return true;
}