import numpy as np
from libcpp cimport bool

cdef extern from "stdio.h":
	cdef extern from "./MVGigE/GigECamera_Types.h":
		ctypedef int BOOL;
		ctypedef unsigned char uint8_t;
		ctypedef unsigned short uint16_t;
		ctypedef unsigned long uint32_t;
		ctypedef unsigned long long uint64_t;

		ctypedef struct MVCamInfo:
			uint8_t mIpAddr[4];
			uint8_t mEthernetAddr[6];
			char mMfgName[32];
			char mModelName[32];
			char mSerialNumber[16];	
			char mUserDefinedName[16];	
			uint8_t m_IfIp[4];
			uint8_t m_IfMAC[6];

		ctypedef struct MVReadBuff:
			uint8_t*  pData;
			uint16_t  wBlockId;
			uint32_t dwWidth;
			uint32_t dwHeight;

		ctypedef void* HANDLE
		ctypedef enum MVSTATUS_CODES:
			pass
		ctypedef enum TriggerSourceEnums:
			pass
		ctypedef enum TriggerModeEnums:
			pass
		ctypedef enum TriggerActivationEnums:
			pass
		ctypedef enum LineSourceEnums:
			pass
		ctypedef enum UserSetSelectorEnums:
			pass
		ctypedef enum ExposureAutoEnums:
			pass
		ctypedef enum GainAutoEnums:
			pass	
		ctypedef enum BalanceWhiteAutoEnums:
			pass	
		ctypedef enum MV_PixelFormatEnums:
			pass	
			
cdef extern from "./MVGigE/MVGigE.h":
	ctypedef void (*MVStreamCB) (uint16_t, uint8_t*, uint32_t, uint32_t)
	extern MVSTATUS_CODES MVRead(HANDLE hCam, MVReadBuff* pBuffer);
	extern MVSTATUS_CODES MVUpdateCameraList();
	extern MVSTATUS_CODES MVGetNumOfCameras(int* pNumCams);
	extern MVSTATUS_CODES MVGetCameraInfo(unsigned char  idx,  MVCamInfo*  pCamInfo);
	extern MVSTATUS_CODES MVOpenCamByIndex(unsigned char  idx,  HANDLE*  hCam);
	extern MVSTATUS_CODES MVOpenCamByUserDefinedName(char*  name,  HANDLE*  hCam);
	extern MVSTATUS_CODES MVCloseCam(HANDLE  hCam);
	extern MVSTATUS_CODES MVStartGrab(HANDLE  hCam,  MVStreamCB  StreamCB);
	extern MVSTATUS_CODES MVStopGrab(HANDLE  hCam);
	extern MVSTATUS_CODES MVTriggerSoftware(HANDLE  hCam);

	extern MVSTATUS_CODES MVForceIp( const char* pMacAddress, const char* pIpAddress,  const char* pSubnetMask,  const char* pDefaultGateway);
	extern MVSTATUS_CODES MVEnumerateAllDevices(int *pDevCnt);
	extern MVSTATUS_CODES MVGetDevInfo(unsigned char idx,MVCamInfo *pCamInfo);

	extern MVSTATUS_CODES MVGetWidth(HANDLE hCam,unsigned int *pWidth);
	extern MVSTATUS_CODES MVSetWidth(HANDLE hCam,unsigned int nWidth);

	extern MVSTATUS_CODES MVGetHeight(HANDLE hCam,unsigned int *pHeight);
	extern MVSTATUS_CODES MVSetHeight(HANDLE hCam,unsigned int nHeight);

	extern MVSTATUS_CODES MVSetPacketDelay(HANDLE hCam,unsigned int nDelay);
	extern MVSTATUS_CODES MVGetPacketDelay(HANDLE hCam,unsigned int* pDelay_us);

	extern MVSTATUS_CODES MVSetPacketSize(HANDLE hCam, unsigned int nSize);
	extern MVSTATUS_CODES MVGetPacketSize(HANDLE hCam, unsigned int* pPacketSize);

	extern MVSTATUS_CODES MVSetTriggerMode( HANDLE hCam,TriggerModeEnums mode );
	extern MVSTATUS_CODES MVGetTriggerMode(HANDLE hCam, TriggerModeEnums* pMode);

	extern MVSTATUS_CODES MVSetTriggerSource( HANDLE hCam,TriggerSourceEnums source );
	extern MVSTATUS_CODES MVGetTriggerSource(HANDLE hCam, TriggerSourceEnums* pSource);

	extern MVSTATUS_CODES MVSetTriggerActivation( HANDLE hCam,TriggerActivationEnums act );
	extern MVSTATUS_CODES MVGetTriggerActivation(HANDLE hCam, TriggerActivationEnums* pAct);

	extern MVSTATUS_CODES MVSetStrobeSource( HANDLE hCam,LineSourceEnums source );
	extern MVSTATUS_CODES MVGetStrobeSource(HANDLE hCam, LineSourceEnums* pSource);

	extern MVSTATUS_CODES MVSetStrobeInvert( HANDLE hCam,unsigned char bInvert );
	extern MVSTATUS_CODES MVGetStrobeInvert(HANDLE hCam, unsigned char* pInvert);

	extern MVSTATUS_CODES MVSetExposureTime( HANDLE hCam,double nExp_us );
	extern MVSTATUS_CODES MVGetExposureTime(HANDLE hCam, double* pExposuretime);

	extern MVSTATUS_CODES MVSetGain(HANDLE hCam, double fGain);
	extern MVSTATUS_CODES MVGetGain(HANDLE hCam, double* pGain);

	extern MVSTATUS_CODES MVGetTriggerDelay(HANDLE hCam, uint32_t* pDelay_us);
	extern MVSTATUS_CODES MVSetTriggerDelay(HANDLE hCam, uint32_t nDelay_us);

	extern MVSTATUS_CODES MVLoadUserSet(HANDLE hCam, UserSetSelectorEnums userset);
	extern MVSTATUS_CODES MVSaveUserSet(HANDLE hCam, UserSetSelectorEnums userset);

	extern MVSTATUS_CODES MVGetDefaultUserSet(HANDLE hCam, UserSetSelectorEnums* pUserset);
	extern MVSTATUS_CODES MVSetDefaultUserSet(HANDLE hCam, UserSetSelectorEnums userset);

	extern MVSTATUS_CODES MVGetExposureAuto(HANDLE hCam, ExposureAutoEnums* pExposureAuto);
	extern MVSTATUS_CODES MVSetExposureAuto(HANDLE hCam, ExposureAutoEnums ExposureAuto);

	extern MVSTATUS_CODES MVGetGainAuto(HANDLE hCam, GainAutoEnums* pGainAuto);
	extern MVSTATUS_CODES MVSetGainAuto(HANDLE hCam, GainAutoEnums GainAuto);

	extern MVSTATUS_CODES MVGetBalanceWhiteAuto(HANDLE hCam, BalanceWhiteAutoEnums* pBalanceWhiteAuto);
	extern MVSTATUS_CODES MVSetBalanceWhiteAuto(HANDLE hCam, BalanceWhiteAutoEnums BalanceWhiteAuto);

	extern MVSTATUS_CODES MVGetWhiteBalance(HANDLE hCam, double* pRed, double* pGreen, double* pBlue);
	extern MVSTATUS_CODES MVSetWhiteBalance(HANDLE hCam, double fRed, double fGreen, double fBlue);

	extern MVSTATUS_CODES MVGetGamma(HANDLE hCam, double* pGamma);
	extern MVSTATUS_CODES MVSetGamma(HANDLE hCam, double fGamma);

	extern void MVBayerNearestNeighbor(const unsigned char* bayer, unsigned char* rgb, int sx, int sy, int tile);	

	extern MVSTATUS_CODES MVGetPixelFormat(HANDLE hCam, MV_PixelFormatEnums* pPixelFormat);
	extern MVSTATUS_CODES MVSetPixelFormat( HANDLE hCam,MV_PixelFormatEnums PixelFormat);
	
cdef extern from "stdio.h":
	extern int printf(const char *format, ...) 
	
