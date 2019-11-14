#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "MVGigE.h"
#include "GigECamera_Types.h"

//#define CV_SHOW  1

#ifdef CV_SHOW 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp> 

using namespace cv;
#endif

#pragma pack( push, 1 ) 
typedef struct tagBITMAPFILEHEADER {
        uint16_t    bfType;
        uint32_t   bfSize;
        uint16_t    bfReserved1;
        uint16_t    bfReserved2;
        uint32_t   bfOffBits;
} BITMAPFILEHEADER;
#pragma pack( pop )

#pragma pack( push, 1 ) 
typedef struct tagBITMAPINFOHEADER{
        uint32_t      biSize;
        int       biWidth;
        int       biHeight;
        uint16_t       biPlanes;
        uint16_t       biBitCount;
        uint32_t      biCompression;
        uint32_t      biSizeImage;
        int       biXPelsPerMeter;
        int       biYPelsPerMeter;
        uint32_t      biClrUsed;
        uint32_t      biClrImportant;
} BITMAPINFOHEADER;
#pragma pack( pop )

int WriteBitmapFileOf24Bits(char  *filename, int width, int height, unsigned char *imageData)
{
      FILE    *filePtr;   // file pointer
      BITMAPFILEHEADER bitmapFileHeader; // bitmap file header
      BITMAPINFOHEADER bitmapInfoHeader; // bitmap info header
      // open file for writing binary mode
      filePtr = fopen(filename, "wb");
      if (!filePtr)
            return 0;
      // define the bitmap file header
      bitmapFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+4*((width*24+31)/32) * height;
      bitmapFileHeader.bfType = 0x4D42;
      bitmapFileHeader.bfReserved1 = 0;
      bitmapFileHeader.bfReserved2 = 0;
      bitmapFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
 
      // define the bitmap information header
      bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
      bitmapInfoHeader.biPlanes = 1;
      bitmapInfoHeader.biBitCount = 24;      // 24-bit
      bitmapInfoHeader.biCompression = 0;    // no compression
      bitmapInfoHeader.biSizeImage = 4*((width*24+31)/32) * height;//the length of real bitmap data in Bytes
      bitmapInfoHeader.biXPelsPerMeter = 0;
      bitmapInfoHeader.biYPelsPerMeter = 0;
      bitmapInfoHeader.biClrUsed = 0;
      bitmapInfoHeader.biClrImportant = 0;
      bitmapInfoHeader.biWidth = width;      // bitmap width
      bitmapInfoHeader.biHeight = -height;      // bitmap height
   
      //write the bitmap file
      fwrite(&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), filePtr);
      fwrite(&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), filePtr);
      fwrite(imageData, 1, bitmapInfoHeader.biSizeImage, filePtr);
      fclose(filePtr);
      return 1;
}

int main(int argc, char *argv[])
{
	int iCams;
	HANDLE hCam;
	MV_PixelFormatEnums PixelFormat;
	uint32_t dwWidth;
	uint32_t dwHeight;
	uint32_t dwScpd = 0;
	uint32_t dwScps = 0;
	uint32_t dwExp = 0;

	if (argc == 1)
	{
		printf("usage: -e ExposureTime -d PacketDelay -s PacketSize \n");
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

	if (MVOpenCamByIndex(0,&hCam) !=  MVST_SUCCESS)
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
	MVGetPixelFormat(hCam, &PixelFormat);
	
	

	printf("%d x %d\n", dwWidth, dwHeight);

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

	usleep(1000000);
	MVSetTriggerMode(hCam, TriggerMode_Off);

	printf("start capture\n");

#ifdef CV_SHOW 
	namedWindow("bayer", CV_WINDOW_AUTOSIZE);
	namedWindow("rgb", CV_WINDOW_AUTOSIZE);
	Mat mImg, mImageRgb;
	mImg.create(dwHeight, dwWidth, CV_8UC1);
	mImageRgb.create(dwHeight, dwWidth, CV_8UC3);
#endif
	MVStartGrab(hCam);
	MVReadBuff stReadBuff; 
	stReadBuff.pData = (uint8_t *)malloc(dwWidth * dwHeight);
	uint8_t * pDataRGB = (uint8_t *)malloc(dwWidth * dwHeight * 3);
	while (MVRead(hCam, &stReadBuff) !=  MVST_SUCCESS);
	
		
#ifdef CV_SHOW 
	memcpy(mImg.data, stReadBuff.pData, dwWidth * dwHeight);

	
	imshow("bayer", mImg);
	imshow("rgb", mImageRgb);
	waitKey(0);
#else
	if (PixelFormat == PixelFormat_BayerGR8)
	{
		MVBayerNearestNeighbor(stReadBuff.pData, pDataRGB,  dwWidth, dwHeight,  DC1394_COLOR_FILTER_GRBG);
		WriteBitmapFileOf24Bits((char*)"bayer.bmp", dwWidth, dwHeight, pDataRGB);
		//printf("frame Id:%d  gray value:%d \n", stReadBuff.wBlockId, stReadBuff.pData[900000]);
	}
	else if (PixelFormat == PixelFormat_BayerBG8)
	{
		MVBayerNearestNeighbor(stReadBuff.pData, pDataRGB,  dwWidth, dwHeight,  DC1394_COLOR_FILTER_BGGR);
		WriteBitmapFileOf24Bits((char*)"bayer.bmp", dwWidth, dwHeight, pDataRGB);
		//printf("frame Id:%d  gray value:%d \n", stReadBuff.wBlockId, stReadBuff.pData[900000]);
	}
	else if (PixelFormat == PixelFormat_BayerRG8)
	{
		MVBayerNearestNeighbor(stReadBuff.pData, pDataRGB,  dwWidth, dwHeight,  DC1394_COLOR_FILTER_RGGB);
		WriteBitmapFileOf24Bits((char*)"bayer.bmp", dwWidth, dwHeight, pDataRGB);
		//printf("frame Id:%d  gray value:%d \n", stReadBuff.wBlockId, stReadBuff.pData[900000]);
	}
	else if (PixelFormat == PixelFormat_BayerGB8)
	{
		MVBayerNearestNeighbor(stReadBuff.pData, pDataRGB,  dwWidth, dwHeight,  DC1394_COLOR_FILTER_GBRG);
		WriteBitmapFileOf24Bits((char*)"bayer.bmp", dwWidth, dwHeight, pDataRGB);
		//printf("frame Id:%d  gray value:%d \n", stReadBuff.wBlockId, stReadBuff.pData[900000]);
	}
	else 
	{
		//imwrite((char*)"bayer.bmp", dwWidth, dwHeight, stReadBuff.pData);
		printf("frame Id:%d  gray value:%d \n", stReadBuff.wBlockId, stReadBuff.pData[900000]);
		
	}
	
	
#endif
	
	MVStopGrab(hCam);
	MVCloseCam(hCam);
	free(stReadBuff.pData);
	free(pDataRGB);

	return 0;
}
