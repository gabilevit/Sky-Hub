#ifndef __AIRLINEFILE__
#define __AIRLINEFILE__

#include "Airline.h"
#include "AirportManager.h"

int initAirlineFromFile(Airline* pComp, AirportManager* pManager, const char* fileName);
int saveAirlineToFile(const Airline* pComp, char* fileName);

#endif