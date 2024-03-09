#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"

void	initAirline(Airline* pComp)
{
	//printf("-----------  Init Airline\n");
	pComp->name = getStrExactName("Enter Airline name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	pComp->planeArr = NULL;
	pComp->planeCount = 0;
	pComp->type = eNotSorted;
}

int	addFlight(Airline* pComp, const AirportManager* pManager)
{
	if (countAirports(pManager) < 2)
	{
		printf("There are not enough airport to set a flight\n");
		return 0;
	}
	if (pComp->planeCount == 0)
	{
		printf("There is no plane in company\n");
		return 0;
	}

	Flight* pFlight = (Flight*)calloc(1, sizeof(Flight));
	if (!pFlight)
		return 0;

	Plane* thePlane = FindAPlane(pComp);
	printAirports(pManager);
	initFlight(pFlight, thePlane, pManager);

	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	if (!pComp->flightArr)
	{
		free(pFlight);
		return 0;
	}
	pComp->flightArr[pComp->flightCount] = pFlight;
	pComp->flightCount++;

	pComp->type = eNotSorted;
	return 1;
}

int		addPlane(Airline* pComp)
{
	pComp->planeArr = (Plane*)realloc(pComp->planeArr, (pComp->planeCount + 1) * sizeof(Plane));
	if (!pComp->planeArr)
		return 0;
	initPlane(&pComp->planeArr[pComp->planeCount], pComp->planeArr, pComp->planeCount);
	pComp->planeCount++;
	return 1;
}

Plane* FindAPlane(Airline* pComp)
{
	printf("Choose a plane from list, type its serial Number\n");
	printPlanesArr(pComp->planeArr, pComp->planeCount);
	int sn;
	Plane* temp = NULL;
	do {
		scanf("%d", &sn);
		temp = findPlaneBySN(pComp->planeArr, pComp->planeCount, sn);
		if (!temp)
			printf("No plane with that serial number! Try again!\n");
	} while (temp == NULL);

	return temp;
}


void printCompany(const Airline* pComp)
{
	printf("Airline %s\n", pComp->name);
	printf("\n -------- Has %d planes\n", pComp->planeCount);
	generalArrayFunction(pComp->planeArr, pComp->planeCount, sizeof(Plane), (void*)printPlane);
	//printPlanesArr(pComp->planeArr, pComp->planeCount);
	printf("\n\n -------- Has %d flights\n", pComp->flightCount);
	generalArrayFunction(*(pComp->flightArr), pComp->flightCount, sizeof(Flight), (void*)printFlight);
	//printFlightArr(pComp->flightArr, pComp->flightCount);
}

void	printFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		printFlight(arr[i]);
}

void	printPlanesArr(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		printPlane(&arr[i]);
}

void	doPrintFlightsWithPlaneType(const Airline* pComp)
{
	ePlaneType type = getPlaneType();
	int count = 0;
	printf("Flights with plane type %s:\n", GetPlaneTypeStr(type));
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneTypeInFlight(pComp->flightArr[i], type))
		{
			printFlight(pComp->flightArr[i]);
			count++;
		}
	}
	if (count == 0)
		printf("Sorry - could not find a flight with plane type %s:\n", GetPlaneTypeStr(type));
	printf("\n");
}

void	freeFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		free(arr[i]);
}

void	freePlanes(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		freePlane(&arr[i]);
}

void	freeCompany(Airline* pComp)
{
	//freeFlightArr(pComp->flightArr, pComp->flightCount);
	generalArrayFunction(pComp->planeArr, pComp->planeCount, sizeof(Plane), free);
	generalArrayFunction(pComp->flightArr, pComp->flightCount, sizeof(Flight*), free);
	free(pComp->flightArr);
	free(pComp->planeArr);
	free(pComp->name);
}

eSortType	getSortType()
{
	int option;
	printf("\n\n");
	do {
		printf("Based on what field do you want to sort?\n");
		for (int i = 1; i < eNofSortTypes; i++)
			printf("Enter %d for %s\n", i, SortTypeStr[i - 1]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofSortTypes);
	getchar();
	return (eSortType)option;
}

void sortFlight(Airline* pComp)
{
	int type;
	type = getSortType();
	switch (type)
	{
	case eSortedBySourceCode:
		sortBySourceCode(pComp);
		break;

	case eSortedByDestCode:
		sortByDestCode(pComp);
		break;

	case eSortedByDate:
		sortByDate(pComp);
		break;

	default:
		printf("Wrong option\n");
		break;
	}

	pComp->type = (eSortType)type;
}

void sortBySourceCode(Airline* pComp)
{
	qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightBySourceCode);
}

void sortByDestCode(Airline* pComp)
{
	qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightByDestCode);
}

void sortByDate(Airline* pComp)
{
	qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightByDate);
}

void findFlight(const Airline* pComp)
{
	int type = pComp->type;
	Flight* pFli = NULL;
	Flight temp;

	switch (type)
	{
	case eNotSorted:
		printf("NOT sorted yet, cant seacrh");
		break;

	case eSortedBySourceCode:
		getAirportCode(temp.sourceCode);
		pFli = &temp;
		pFli = *((Flight**)bsearch(&pFli, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightBySourceCode));
		if (!pFli)
			printf("Flight was not found\n");
		else {
			printf("Flight found, ");
			printFlight(pFli);
		}
		break;

	case eSortedByDestCode:
		getAirportCode(temp.destCode);
		pFli = &temp;
		pFli = *((Flight**)bsearch(&pFli, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightByDestCode));
		if (!pFli)
			printf("Flight was not found\n");
		else {
			printf("Flight found, ");
			printFlight(pFli);
		}
		break;

	case eSortedByDate:
		getCorrectDate(&temp.date);
		pFli = &temp;
		pFli = *((Flight**)bsearch(&pFli, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightByDate));
		if (!pFli)
			printf("Flight was not found\n");
		else {
			printf("Flight found, ");
			printFlight(pFli);
		}
		break;
	default:
		printf("Wrong option\n");
		break;
	}
}

