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


	unsigned char pGetIp[4], pGetSubNetmask[4], pGetGateWay[4];
	size_t nGetIpLen = 4, nGetSubNetmaskLen = 4, nGetGateWayLen = 4;

	int ret = MVGetPersistentIpAddress(hCam, pGetIp, &nGetIpLen, pGetSubNetmask,
		 &nGetSubNetmaskLen, pGetGateWay, &nGetGateWayLen);

	if (ret != MVST_SUCCESS)
	{
		printf("GetPersistentIpAddress error : %d\n", ret);
		return -1;
	}

	printf("GetPersistentIpAddress : [ip]%d:%d:%d:%d  [sub-netmask]%d:%d:%d:%d [gateWay]%d:%d:%d:%d \n ", pGetIp[0], pGetIp[1], pGetIp[2], pGetIp[3], pGetSubNetmask[0], pGetSubNetmask[1], pGetSubNetmask[2], pGetSubNetmask[3], pGetGateWay[0], pGetGateWay[1],
pGetGateWay[2], pGetGateWay[3]);

	return 0;
}
