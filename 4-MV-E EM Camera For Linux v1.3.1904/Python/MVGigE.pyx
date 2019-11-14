# _*_ coding:utf-8 _*_ 
from MVGigE_def cimport *
import numpy as np
cimport numpy as np
from libc.stdlib cimport calloc, free

PixelFormat_Mono8    = 0x01080001	   #!<8Bit灰度
PixelFormat_BayerBG8 = 0x0108000B	   #!<8Bit Bayer图,颜色模式为BGGR
PixelFormat_BayerRG8 = 0x01080009	   #!<8Bit Bayer图,颜色模式为RGGB
PixelFormat_BayerGB8 = 0x0108000A	   #!<8Bit Bayer图,颜色模式为GBRG
PixelFormat_BayerGR8 = 0x01080008      #!<8Bit Bayer图,颜色模式为GRBG
	
TriggerMode_Off = 0 #  !<触发模式关，即FreeRun模式，相机连续采集
TriggerMode_On = 1		

TriggerSource_Software = 0  #!<触发模式下，由软触发(软件指令)来触发采集
TriggerSource_Line1 = 2

TriggerActivation_RisingEdge = 0   #!<上升沿触发
TriggerActivation_FallingEdge = 1

LineSource_Off=0                 #!<关闭
LineSource_ExposureActive=5      #!<和曝光同时
LineSource_Timer1Active=6        #!<由定时器控制
LineSource_UserOutput0=12 	

	
def UpdateCameraList():
	return MVUpdateCameraList();

def GetNumOfCameras():
	cdef int pNumCams;
	MVUpdateCameraList();
	MVGetNumOfCameras(&pNumCams);
	return pNumCams;

def GetCameraInfo(idx):
	cdef MVCamInfo stCamInfo;
	MVGetCameraInfo(idx, &stCamInfo);

	info={}
	info["mIpAddr"] = [stCamInfo.mIpAddr[0], stCamInfo.mIpAddr[1],  \
		stCamInfo.mIpAddr[2], stCamInfo.mIpAddr[3]]
	info["mEthernetAddr"] = [stCamInfo.mEthernetAddr[0], stCamInfo.mEthernetAddr[1],  \
		stCamInfo.mEthernetAddr[2], stCamInfo.mEthernetAddr[3],  \
		stCamInfo.mEthernetAddr[4], stCamInfo.mEthernetAddr[5]]
	info["mMfgName"] = stCamInfo.mMfgName
	info["mModelName"] = stCamInfo.mModelName
	info["mSerialNumber"] = stCamInfo.mSerialNumber
	info["mUserDefinedName"] = stCamInfo.mUserDefinedName
	info["m_IfIp"] = [stCamInfo.m_IfIp[0], stCamInfo.m_IfIp[1],  \
		stCamInfo.m_IfIp[2], stCamInfo.m_IfIp[3]]
	info["m_IfMAC"] = [stCamInfo.m_IfMAC[0], stCamInfo.m_IfMAC[1],  \
		stCamInfo.m_IfMAC[2], stCamInfo.m_IfMAC[3],  \
		stCamInfo.m_IfMAC[4], stCamInfo.m_IfMAC[5]]

	return info;

def GetNumOfDevices():
	cdef int dwDevices;
	MVEnumerateAllDevices(&dwDevices);
	return dwDevices;

def GetDevInfo(idx):
	cdef MVCamInfo stCamInfo;
	MVGetDevInfo(idx, &stCamInfo);

	info={}
	info["mIpAddr"] = [stCamInfo.mIpAddr[0], stCamInfo.mIpAddr[1],  \
		stCamInfo.mIpAddr[2], stCamInfo.mIpAddr[3]]
	info["mEthernetAddr"] = [stCamInfo.mEthernetAddr[0], stCamInfo.mEthernetAddr[1],  \
		stCamInfo.mEthernetAddr[2], stCamInfo.mEthernetAddr[3],  \
		stCamInfo.mEthernetAddr[4], stCamInfo.mEthernetAddr[5]]
	info["mMfgName"] = stCamInfo.mMfgName
	info["mModelName"] = stCamInfo.mModelName
	info["mSerialNumber"] = stCamInfo.mSerialNumber
	info["mUserDefinedName"] = stCamInfo.mUserDefinedName
	info["m_IfIp"] = [stCamInfo.m_IfIp[0], stCamInfo.m_IfIp[1],  \
		stCamInfo.m_IfIp[2], stCamInfo.m_IfIp[3]]
	info["m_IfMAC"] = [stCamInfo.m_IfMAC[0], stCamInfo.m_IfMAC[1],  \
		stCamInfo.m_IfMAC[2], stCamInfo.m_IfMAC[3],  \
		stCamInfo.m_IfMAC[4], stCamInfo.m_IfMAC[5]]

	return info;

def ForceIp(pMacAddress, pIpAddress, pSubnetMask,  pDefaultGateway):
	#print('ForceIp', pMacAddress.encode('utf-8'))
	#cdef unsigned char macs[6];
	#mac = pMacAddress.split(',') 
	#print('mac:', mac)
	#for i in range(6):
	#	pMacAddress = int(mac[i])
	#print('mac:', mac)
	return MVForceIp(pMacAddress, pIpAddress, pSubnetMask,  pDefaultGateway);

def read(cam):
	cdef long icam = <long>cam;
	cdef HANDLE hCam = <HANDLE>icam;
	cdef MVReadBuff stReadBuff;
	cdef unsigned int w, h;

	if (MVGetWidth(hCam, &w) != 0):
		return False, None
	if (MVGetHeight(hCam, &h) != 0):
		return False, None

	stReadBuff.pData = <uint8_t *>calloc(w * h, 1)

	if (MVRead(hCam, &stReadBuff) == 0):
		ImgData = np.frombuffer((<uint8_t*>stReadBuff.pData)[:w * h], dtype=np.uint8)
		free(stReadBuff.pData)
		if (np.size(ImgData) != 0):
			ImgData = ImgData.reshape((h, -1));
			return True, ImgData
		else:
			return False, None
			
	else:
		free(stReadBuff.pData)
		return False, None
	
def open(idx):
	cdef HANDLE hCam;

	r = MVOpenCamByIndex(idx, &hCam);

	if r != 0:
		return False, None
	
	return True, <long>hCam
		
def start(cam):
	cdef long icam = <long>cam;
	cdef HANDLE hCam = <HANDLE>icam;
	
	if (MVStartGrab(hCam, NULL) != 0):
		return False
	else:
		return True

def stop(cam):
	cdef long icam = <long>cam;
	cdef HANDLE hCam = <HANDLE>icam;

	if (MVStopGrab(hCam) != 0):
		return False

	return True
	
def close(cam):
	cdef long icam = <long>cam;
	cdef HANDLE hCam = <HANDLE>icam;

	if (MVCloseCam(hCam) != 0):
		return False

	return True

MVCAM_PROP_EXPOSURE = 0
MVCAM_PROP_TRIG_MODE = 1
MVCAM_PROP_TRIG_SOURCE = 2
MVCAM_PROP_TRIG_ACTIVATION = 3
MVCAM_PROP_STROBE_SOURCE = 4
MVCAM_PROP_STROBE_INVERT = 5
MVCAM_PROP_PACKET_SIZE = 6
MVCAM_PROP_PACKET_DELAY = 7
MVCAM_PROP_WIDTH = 8
MVCAM_PROP_HEIGHT = 9
MVCAM_PROP_PIXELFORMAT = 10

def set(cam, set_type, set_value):
	cdef long icam = <long>cam;
	cdef HANDLE hCam = <HANDLE>icam;

	if set_type == 	MVCAM_PROP_EXPOSURE:
		if (MVSetExposureTime(hCam, set_value) != 0):
			return False
	elif set_type == MVCAM_PROP_TRIG_MODE:
		if (MVSetTriggerMode(hCam, set_value) != 0):
			return False
	elif set_type == MVCAM_PROP_TRIG_SOURCE:
		if (MVSetTriggerSource(hCam, set_value) != 0):
			return False
	elif set_type == MVCAM_PROP_TRIG_ACTIVATION:
		if (MVSetTriggerActivation(hCam, set_value) != 0):
			return False
	elif set_type == MVCAM_PROP_STROBE_SOURCE:
		if (MVSetStrobeSource(hCam, set_value) != 0):
			return False
	elif set_type == MVCAM_PROP_STROBE_INVERT:
		if (MVSetStrobeInvert(hCam, set_value) != 0):
			return False
	elif set_type == MVCAM_PROP_PACKET_SIZE:
		if (MVSetPacketSize(hCam, set_value) != 0):
			return False
	elif set_type == MVCAM_PROP_PACKET_DELAY:
		if (MVSetPacketDelay(hCam, set_value) != 0):
			return False
	elif set_type == MVCAM_PROP_WIDTH:
		if (MVSetWidth(hCam, set_value) != 0):
			return False
	elif set_type == MVCAM_PROP_HEIGHT:
		if (MVSetHeight(hCam, set_value) != 0):
			return False
	elif set_type == MVCAM_PROP_PIXELFORMAT:
		if (MVSetPixelFormat(hCam, set_value) != 0):
			return False
	else:
		return False
	
	return True

COLOR_BAYER_BG2RGB = 0
COLOR_BAYER_GR2RGB = 2#2
COLOR_BAYER_GB2RGB = 3
COLOR_BAYER_RG2RGB = 1#1

def cvtColor(np.ndarray[unsigned char, ndim=2, mode="c"] in_array not None, np.ndarray[unsigned char, ndim=3, mode="c"] out_array not None, cvtType):
	MVBayerNearestNeighbor(<unsigned char*> np.PyArray_DATA(in_array.reshape(1,in_array.size)), <unsigned char*> np.PyArray_DATA(out_array.reshape(1, out_array.size)), in_array.shape[1], in_array.shape[0], cvtType)	


def get(cam, set_type):
	cdef long icam = <long>cam
	cdef HANDLE hCam = <HANDLE>icam
	cdef unsigned int dwValue
	cdef unsigned char byValue
	cdef double fValue
	cdef TriggerModeEnums eTrigMode
	cdef TriggerSourceEnums eTrigSource
	cdef TriggerActivationEnums eTrigActivation
	cdef LineSourceEnums eLineSource
	cdef MV_PixelFormatEnums ePixelFormat
		
	if set_type == MVCAM_PROP_EXPOSURE:
		MVGetExposureTime(hCam, &fValue)
		return fValue
	elif set_type == MVCAM_PROP_TRIG_MODE:
		MVGetTriggerMode(hCam, &eTrigMode)
		return eTrigMode
	elif set_type == MVCAM_PROP_TRIG_SOURCE:
		MVGetTriggerSource(hCam, &eTrigSource)
		return eTrigSource
	elif set_type == MVCAM_PROP_TRIG_ACTIVATION:
		MVGetTriggerActivation(hCam, &eTrigActivation)
		return eTrigActivation
	elif set_type == MVCAM_PROP_STROBE_SOURCE:
		MVGetStrobeSource(hCam, &eLineSource)
		return eLineSource
	elif set_type == MVCAM_PROP_STROBE_INVERT:
		MVGetStrobeInvert(hCam, &byValue)
		return byValue
	elif set_type == MVCAM_PROP_PACKET_SIZE:
		MVGetPacketSize(hCam, &dwValue)
	elif set_type == MVCAM_PROP_PACKET_DELAY:
		MVGetPacketDelay(hCam, &dwValue)
	elif set_type == MVCAM_PROP_WIDTH:
		MVGetWidth(hCam, &dwValue)
	elif set_type == MVCAM_PROP_HEIGHT:
		MVGetHeight(hCam, &dwValue)
	elif set_type == MVCAM_PROP_PIXELFORMAT:
		MVGetPixelFormat(hCam, &ePixelFormat)
		return ePixelFormat
	else:
		dwValue = 0
	
	return dwValue

def TriggerSoftware(cam):
	cdef long icam = <long>cam;
	cdef HANDLE hCam = <HANDLE>icam;
	return MVTriggerSoftware(hCam);

	
