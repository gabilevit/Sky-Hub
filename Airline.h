#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"

typedef enum {
	eNotSorted, eSortedBySourceCode, eSortedByDestCode, eSortedByDate, eNofSortTypes
} eSortType;

static const char* SortTypeStr[eNofSortTypes]
= {"Source code", "Dest code", "Date"};

typedef struct
{
	char* name;
	int			flightCount;
	Flight** flightArr;
	int			planeCount;
	Plane* planeArr;
	eSortType type;
}Airline;

void	initAirline(Airline* pComp);
int		addFlight(Airline* pComp, const AirportManager* pManager);
int		addPlane(Airline* pComp);
Plane* FindAPlane(Airline* pComp);
void	printCompany(const Airline* pComp);
void	printFlightArr(Flight** arr, int size);
void	printPlanesArr(Plane* arr, int size);
void	doPrintFlightsWithPlaneType(const Airline* pComp);
void	freeFlightArr(Flight** arr, int size);
void	freePlanes(Plane* arr, int size);
void	freeCompany(Airline* pComp);

eSortType	getSortType();
void sortFlight(Airline* pComp);
void sortBySourceCode(Airline* pComp);
void sortByDestCode(Airline* pComp);
void sortByDate(Airline* pComp);
void findFlight(const Airline* pComp);
int initAirlineFromFile(Airline* pComp, AirportManager* pManager, const char* fileName);
void saveAirlineToFile(const Airline* pComp, char* fileName);

#endif

