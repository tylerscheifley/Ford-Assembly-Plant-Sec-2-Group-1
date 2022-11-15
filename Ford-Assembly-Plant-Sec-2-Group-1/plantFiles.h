#pragma once
void updateLog(Plant* plant);
void readLog(Plant *plant);
void stopPlant(Plant* plant);
void sampleDataCreator();
void sampleDataSaver(string date, int globalAirQuality, int vehicleQuota, double globalTemp, double globalHumidity);