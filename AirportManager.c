#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"
#include "General.h"

int	initManager(AirportManager* pManager, const char* fileName)
{
	if (!L_init(&pManager->airportsList))
		return 0;

	if (!readManagerFromFile(pManager, fileName))
		return 2;
	return 1;
}

int	addAirport(AirportManager* pManager)
{
	Airport* pPort = (Airport*)calloc(1, sizeof(Airport));
	if (!pPort)
		return 0;

	if (!initAirport(pPort, pManager))
	{
		freeAirport(pPort);
		free(pPort);
		return 0;
	}

	NODE* pNode;
	pNode = &pManager->airportsList.head;

	// if its the first node
	if (pNode->next == NULL)
	{
		L_insert(pNode, pPort);
		return 1;
	}	

	NODE* tmpNode;
	tmpNode = pNode;
	// after the head (first valid node)
	pNode = pNode->next;

	while (pNode != NULL)
	{
		if (compareAirportByAirportCode(pPort, pNode->key) < 0)
		{
			L_insert(tmpNode, pPort);
			return 1;
		}
		pNode = pNode->next;
		tmpNode = tmpNode->next;	
	}
	L_insert(tmpNode, pPort);
	return 1;
	//pManager->airportsArray = (Airport**)realloc(pManager->airportsArray,
	//	(pManager->airportsCount + 1) * sizeof(Airport*));
	//if (!pManager->airportsArray)
	//{
	//	freeAirport(pPort);
	//	free(pPort);
	//	return 0;
	//}


	//pManager->airportsArray[pManager->airportsCount] = pPort;
	//pManager->airportsCount++;
	//return 1;
}

int  initAirport(Airport* pPort, AirportManager* pManager)
{
	while (1)
	{
		getAirportCode(pPort->code);
		if (checkUniqeCode(pPort->code, pManager))
			break;

		printf("This code already in use - enter a different code\n");
	}

	return initAirportNoCode(pPort);
}

Airport* findAirportByCode(const AirportManager* pManager, const char* code)
{
	const NODE* pRes = L_find(pManager->airportsList.head.next, code, isNOTAirportCode);
	if (pRes)
	{
		return pRes->key;
	}
	/*for (int i = 0; i < pManager->airportsCount; i++)
	{
		if (isAirportCode(pManager->airportsArray[i], code))
			return pManager->airportsArray[i];
	}*/
	return NULL;
}

int checkUniqeCode(const char* code, const AirportManager* pManager)
{
	Airport* port = findAirportByCode(pManager, code);

	if (port != NULL)
		return 0;

	return 1;
}


void	printAirports(const AirportManager* pManager)
{
	int airportsCount = countAirports(pManager);
	printf("there are %d airports\n", airportsCount);

	NODE* pNode = pManager->airportsList.head.next;

	if (!pNode)
		return;

	while (pNode)
	{
		printAirport(pNode->key);
		pNode = pNode->next;
	}

	/*for (int i = 0; i < airportsCount; i++)
	{
		printAirport();
		printf("\n");
	}*/
}

int countAirports(AirportManager* pManager)
{
	int count = 0;
	NODE* pNode = pManager->airportsList.head.next;

	if (!pNode)
		return 0;

	while (pNode != NULL)
	{
		pNode = pNode->next;
		count++;
	}

	return count;
}

void	freeManager(AirportManager* pManager)
{
	freeAirportArr(pManager);
}


void	freeAirportArr(AirportManager* pManager)
{
	NODE* pNode = pManager->airportsList.head.next;

	if (!pNode)
		return;

	while (!pNode)
	{
		freeAirport(pNode->key);
		free(pNode->key);
		pNode = pNode->next;
	}
	L_free(&pManager->airportsList, free);
	/*for (int i = 0; i < pManager->airportsCount; i++)
	{
		freeAirport(pManager->airportsArray[i]);
		free(pManager->airportsArray[i]);
	}
	free(pManager->airportsArray);*/
}

int saveManagerToFile(const AirportManager* pManager, const char* fileName)
{
	FILE* f = fopen(fileName, "w");
	if (f == NULL)
	{
		printf("Failed opening the file. Exiting!\n");
		return 0;
	}
	int size = countAirports(pManager);
	fprintf(f, "%d", size);
	NODE* pNode = pManager->airportsList.head.next;

	if (!pNode)
		return 0;

	while (pNode)
	{
		Airport* theAirport = (Airport*)pNode->key;
		writeAirportToTextFile(f, theAirport);
		pNode = pNode->next;
	}
	fclose(f);
	return 1;
}

int readManagerFromFile(const AirportManager* pManager, const char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (f == NULL)
	{
		printf("Failed opening the file. Exiting!\n");
		return 2;
	}
	int size = countAirports(pManager);
	if (fscanf(f, "%d", &size) != 1)
	{
		fclose(f);
		return 2;
	}
	Airport* pPort = (Airport*)calloc(1, sizeof(Airport));
	if (!pPort)
		return 2;
	NODE* pNode = pManager->airportsList.head.next;

	if (!pNode)
		return 2;

	while (readAirportFromTextFile(f, pPort))
	{
		L_insert(pNode, pPort);
		pNode = pNode->next;
	}
	fclose(f);
	return 1;
}