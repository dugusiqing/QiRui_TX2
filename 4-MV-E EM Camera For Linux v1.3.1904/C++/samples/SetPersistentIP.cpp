#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "MVGigE.h"


int main(int argc, char *argv[])
{
	int iCams;
	HANDLE hCam;

	if (argc != 4)
	{
		printf("./SetPersistentIp [ip] [sub-netmask] [gateway]\n");
		return -1;
	}

	const char* pIp = argv[1];
	const char* pGateway = argv[3];
	const char* pSubnetMask = argv[2];

	MVGetNumOfCameras(&iCams);

	if (iCams <= 0)
	{
		printf("find 0 camera\n");
		return -1;
	}
	
	if (MVOpenCamByIndex(0, &hCam) !=  MVST_SUCCESS)
	{
		printf("can't open camera\n");
		return -1;
	}

	if (MVSetPersistentIpAddress(hCam, pIp, pSubnetMask, pGateway) != MVST_SUCCESS)
	{
		printf("SetPersistentIpAddress error\n");
		return -1;
	}

	printf("SetPersistentIpAddress success\n");

	return 0;
}
