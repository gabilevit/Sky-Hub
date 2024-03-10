#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "airlineFile.h"


int initAirlineFromFile(Airline* pComp, AirportManager* pManager, const char* fileName)
{
	FILE* f = fopen(fileName, "rb");
	if (!f)
		return 0;
	int len;
	if (fread(&len, sizeof(int), 1, f) != 1)
		return 0;
	pComp->name = (char*)malloc(len * sizeof(char));
	if (fread(pComp->name, sizeof(char), len, f) != len)
	{
		free(pComp->name);
		return 0;
	}
	if (fread(&pComp->planeCount, sizeof(int), 1, f) != 1)
	{
		fclose(f);
		return 0;
	}
	pComp->planeArr = (Plane*)malloc(pComp->planeCount * sizeof(Plane));
	for (int i = 0; i < pComp->planeCount; i++)
	{
		if (!readPlaneFromFile(f, &pComp->planeArr[i]))
		{
			generalArrayFunction(pComp->planeArr, pComp->planeCount, sizeof(Plane), free);
			fclose(f);
			return 0;
		}
	}
	if (fread(&pComp->flightCount, sizeof(int), 1, f) != 1)
	{
		fclose(f);
		return 0;
	}
	pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
	if (!pComp->flightArr)
	{
		return 0;
	}
		
	for (int i = 0; i < pComp->flightCount; i++)
	{
		pComp->flightArr[i] = (Flight*)malloc(pComp->flightCount * sizeof(Flight));
		if (!readFlightFromFile(f, pComp->flightArr[i]))
		{
			generalArrayFunction(pComp->flightArr, pComp->planeCount, sizeof(Plane), free);
			fclose(f);
			return 0;
		}
		int sn;
		if (fread(&sn, sizeof(int), 1, f) != 1)
		{
			free(&pComp->flightArr[i]->sourceCode);
			free(&pComp->flightArr[i]->destCode);
			return 0;
		}
		Plane* p = findPlaneBySN(pComp->planeArr, pComp->planeCount, sn);
		pComp->flightArr[i]->flightPlane = *p;
		if (fread(&pComp->flightArr[i]->date, sizeof(Date), 1, f) != 1)
		{
			free(&pComp->flightArr[i]->sourceCode);
			free(&pComp->flightArr[i]->destCode);
			return 0;
		}
	}
	pComp->type = eNotSorted;
	fclose(f);
	return 1;
}

int saveAirlineToFile(const Airline* pComp, char* fileName)
{
	FILE* f = fopen(fileName, "wb");
	if (!f)
		return 0;

	int len = (int)strlen(pComp->name) + 1;
	if (fwrite(&len, sizeof(int), 1, f) != 1)
	{
		fclose(f);
		return 0;
	}
	if (fwrite(pComp->name, sizeof(char), len, f) != len)
	{
		fclose(f);
		return 0;
	}
	if (fwrite(&pComp->planeCount, sizeof(int), 1, f) != 1)
	{
		fclose(f);
		return 0;
	}
	for (int i = 0; i < pComp->planeCount; i++)
	{
		if (!savePlaneToFile(f, &pComp->planeArr[i]))
		{
			fclose(f);
			return 0;
		}
	}
	if (fwrite(&pComp->flightCount, sizeof(int), 1, f) != 1)
	{
		fclose(f);
		return 0;
	}
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (!saveFlightToFile(f, pComp->flightArr[i]))
		{
			fclose(f);
			return 0;
		}
	}
	fclose(f);
	return 1;
}