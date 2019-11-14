#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "MVGigE.h"


int main(int argc, char *argv[])
{
	int iCams;
	MVCamInfo stCamInfo;

	if (argc != 4 )
	{
		printf("./forceIp [ip] [sub-netmask] [gateway]\n");
		//return -1;
	}

	MVEnumerateAllDevices(&iCams);

	printf("find %d Devices\n", iCams);

	for (int i = 0; i < iCams; i++)
	{
		MVGetDevInfo(i, &stCamInfo);
		printf("IP : %d %d.%d.%d.%d\n", i, stCamInfo.mIpAddr[0], 
			stCamInfo.mIpAddr[1], stCamInfo.mIpAddr[2],
			stCamInfo.mIpAddr[3]);

		printf("MAC : %x:%x:%x:%x:%x:%x\n", stCamInfo.mEthernetAddr[0], 
			stCamInfo.mEthernetAddr[1], stCamInfo.mEthernetAddr[2],
			stCamInfo.mEthernetAddr[3], stCamInfo.mEthernetAddr[4],
			stCamInfo.mEthernetAddr[5]);
	}

	if (argc != 4)
	{
		return -1;
	}

	 const char* pIp = argv[1];
	 const char* pSubnetMask = argv[2];
	 const char* pGateway = argv[3];
	if (iCams > 0)
	{
		MVForceIp((const char *)stCamInfo.mEthernetAddr, 
			(const char *)pIp, (const char *)pSubnetMask,
			(const char *)pGateway);
	}

	MVGetNumOfCameras(&iCams);

	printf("find %d cameras can open\n", iCams);

	for (int i = 0; i < iCams; i++)
	{
		MVGetCameraInfo(i, &stCamInfo);
		printf("IP: %d %d.%d.%d.%d\n", i, stCamInfo.mIpAddr[0], 
			stCamInfo.mIpAddr[1], stCamInfo.mIpAddr[2],
			stCamInfo.mIpAddr[3]);
	}

	return 0;
}
