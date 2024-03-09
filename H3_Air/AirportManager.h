#ifndef __AIR_MANAGER__
#define __AIR_MANAGER__

#include "Airport.h"
#include "list.h"

typedef struct
{
	LIST airportsList;
}AirportManager;

int		initManager(AirportManager* pManager, const char* fileName);
int		addAirport(AirportManager* pManager);
int		initAirport(Airport* pPort, AirportManager* pManager);
Airport* findAirportByCode(const AirportManager* pManager, const char* code);
int		checkUniqeCode(const char* code, const AirportManager* pManager);
int		countAirports(const AirportManager* pManager);
void	printAirports(const AirportManager* pManager);
void	freeManager(AirportManager* pManager);
void	freeAirportArr(AirportManager* pManager);

int saveManagerToFile(const AirportManager* pManager, const char* fileName);
int readManagerFromFile(AirportManager* pManager, const char* fileName);
#endif