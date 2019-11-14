#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "MVGigE.h"

#define CV_SHOW  1

#ifdef CV_SHOW 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp> 

using namespace cv;
#endif


int main(int argc, char *argv[])
{
	int iCams;
	HANDLE hCam;
	uint32_t dwWidth;
	uint32_t dwHeight;
	uint32_t dwScpd = 0;
	uint32_t dwScps = 0;
	uint32_t dwExp = 0;
	uint32_t dwSeconds = 5;
	timeval tStart, tEnd;

	if (argc == 1)
	{
		printf("usage: -e ExposureTime -d PacketDelay -s PacketSize  -t CaptureTime\n");
	}

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-e") == 0)
		{
			i++;
			dwExp = atoi(argv[i]);
		}
		else if (strcmp(argv[i], "-d") == 0)
		{
			i++;
			dwScpd = atoi(argv[i]);
		}
		else if (strcmp(argv[i], "-s") == 0)
		{
			i++;
			dwScps = atoi(argv[i]);
		}
		else if (strcmp(argv[i], "-t") == 0)
		{
			i++;
			dwSeconds = atoi(argv[i]);
		}
		else
		{
			printf("Unrecognized argument: %s\n", argv[i]);
			return 0;
		}
	}

	MVGetNumOfCameras(&iCams);

	if (iCams == 0)
	{
		printf("no cameras\n");
		return 0;
	}
	else
	{
		printf("found %d cameras \n", iCams);
	}

	if (MVOpenCamByIndex(0, &hCam) != MVST_SUCCESS)
	{
		printf("can't open camera\n");
		return 0;
	}
	else
	{
		printf("camera opened\n");
	}

	MVGetWidth(hCam, &dwWidth);
	MVGetHeight(hCam, &dwHeight);

	printf("Image Width:%d  Height%d\n", dwWidth, dwHeight);

	if (dwScpd != 0)
	{
		MVSetPacketDelay(hCam, dwScpd);
	}

	if (dwScps != 0)
	{
		MVSetPacketSize(hCam, dwScps);
	}

	if (dwExp != 0)
	{
		MVSetExposureTime(hCam, dwExp);
	}

	MVSetTriggerMode(hCam, TriggerMode_On);
	MVSetTriggerSource(hCam, TriggerSource_Software);

	printf("start capture\n");

#ifdef CV_SHOW 
	namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
	Mat mImg;
	mImg.create(dwHeight, dwWidth, CV_8UC1);
#endif
	MVStartGrab(hCam);

	MVReadBuff stReadBuff; 
	stReadBuff.pData = (uint8_t *)malloc(dwWidth * dwHeight);

	gettimeofday(&tStart, NULL);

	for (; ;)
	{
		MVTriggerSoftware(hCam);

		while (MVRead(hCam, &stReadBuff) != MVST_SUCCESS)
		{
			;
		}

#ifdef CV_SHOW 
		memcpy(mImg.data, stReadBuff.pData, dwWidth * dwHeight);
		imshow("MyWindow", mImg);
		waitKey(1);
#else
		printf("frame Id:%d  gray value:%d \n", stReadBuff.wBlockId, stReadBuff.pData[0]);
#endif

		gettimeofday(&tEnd, NULL);

		if ((tEnd.tv_sec - tStart.tv_sec) >= dwSeconds)
		{
			int frame = stReadBuff.wBlockId/dwSeconds;
			printf("frame:%d \n"  , frame);
			break;
		}
		
		
	}

	MVStopGrab(hCam);
	MVCloseCam(hCam);
	free(stReadBuff.pData);

	return 0;
}
