//CSCN72030-Software Development Life Cycle-Ford Assembly Line HMI
//
//Plant Unit Tests
//
//November 25th, 2022
//
//Zebadiah Lake

//Revision History
//
//1.0   Nov. 25th   Initial

#include "pch.h"
#include "CppUnitTest.h"
#include <iostream>
#include <fstream>
#include <Ford-Assembly-Plant-Sec-2-Group-1/plant.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlantTests
{
	TEST_CLASS(PlantTests)
	{
	public:
		
		TEST_METHOD(GetSetDate_Successfully_Test)
		{
			Plant plant;

			ifstream sampleTxt;
			sampleTxt.open("Sample.txt");

			string date;

			if (sampleTxt.is_open())
			{
				sampleTxt >> date;

				plant.setDate(date);
			}
			sampleTxt.close();
			
			Assert::AreEqual(date, plant.getDate());
		}

		TEST_METHOD(GetSetGlobalAirQuality_Successfully_Test)
		{
			Plant plant;

			int quality = 30;
			plant.setGlobalAirQuality(quality);

			Assert::AreEqual(quality, plant.getGlobalAirQuality());
		}

		TEST_METHOD(TestMethod1)
		{
		}

		TEST_METHOD(TestMethod1)
		{
		}

		TEST_METHOD(TestMethod1)
		{
		}

		TEST_METHOD(TestMethod1)
		{
		}
		
		TEST_METHOD(TestMethod1)
		{
		}
		
		TEST_METHOD(TestMethod1)
		{
		}
		
		TEST_METHOD(TestMethod1)
		{
		}
		
		TEST_METHOD(TestMethod1)
		{
		}
		
		TEST_METHOD(TestMethod1)
		{
		}

		TEST_METHOD(TestMethod1)
		{
		}

	};
}
