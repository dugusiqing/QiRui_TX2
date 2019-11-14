#ifndef MVGIGE_H_INCLUDE_
#define MVGIGE_H_INCLUDE_

#include "GigECamera_Types.h"

#define DC1394_COLOR_FILTER_BGGR 0
#define DC1394_COLOR_FILTER_RGGB 1
#define DC1394_COLOR_FILTER_GRBG 2
#define DC1394_COLOR_FILTER_GBRG 3

/* 相机回调函数  
   参数依次是：帧ID， 图像指针， 图像宽度， 图像高度*/
typedef void (*CALLBACK_FUN) (uint16_t, uint8_t*, uint32_t, uint32_t, long);






#ifdef __cplusplus
extern "C" {
#endif

/*! 
 *  \brief	查找连接到计算机上的相机
 *  \retval 
 */
MVSTATUS_CODES  MVUpdateCameraList();

/*! 
 *  \brief	获取连接到计算机上的相机的数量
 *  \param [out]	pNumCams	相机数量
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES  MVGetNumOfCameras(int *pNumCams);

/*! 
 *  \brief	得到第idx个相机的信息。
 *  \param [in]	idx idx从0开始，按照相机的IP地址排序，地址小的排在前面。
 *  \param [out]	pCamInfo  相机的信息 (IP,MAC,SN,型号...) 
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES  MVGetCameraInfo(unsigned char idx, MVCamInfo *pCamInfo = NULL);

/*! 
 *  \brief	打开第idx个相机
 *  \param [in]	idx	idx从0开始，按照相机的IP地址排序，地址小的排在前面。
 *  \param [out]	hCam 如果成功,返回的相机句柄
 *  \retval MVST_INVALID_PARAMETER : idx取值不对
 *			MVST_ACCESS_DENIED		: 相机无法访问，可能正被别的软件控制
 *			MVST_ERROR				: 其他错误
 *			MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES  MVOpenCamByIndex(unsigned char idx, HANDLE *hCam);

/*! 
 *  \brief	打开指定UserDefinedName的相机
 *  \param [in]	name UserDefinedName。
 *  \param [out] hCam 如果成功,返回的相机句柄。如果失败，为NULL。
 *  \retval 
 *			MVST_ACCESS_DENIED		: 相机无法访问，可能正被别的软件控制
 *			MVST_ERROR				: 其他错误
 *			MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES  MVOpenCamByUserDefinedName(char *name, HANDLE *hCam);

/*! 
 *  \brief	关闭相机。断开和相机的连接。
 *  \param [in]	hCam 相机的句柄
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVCloseCam(HANDLE hCam);

/*!
 * \brief 搜索相机，包含不在同一网段的相机
 * \param [out] pDevCnt 相机数量指针
 * \retval
 */
MVSTATUS_CODES  MVEnumerateAllDevices(int *pDevCnt);

/*!
 * \brief	获取MVEnumerateAllDevices搜索到的相机的信息
 * \param [in] idx	相机序号
 * \param [out] pCamInfo	相机信息
 * \retval
 */
MVSTATUS_CODES  MVGetDevInfo(unsigned char idx, MVCamInfo *pCamInfo);

/*!
 * \brief 为相机设置ip地址
 * \param [in] pMacAddress  待设置ip相机的MAC地址
 * \param [in] pIpAddress	设置给相机的IP地址
 * \param [in] pSubnetMask	设置给相机的子网掩码
 * \param [in] pDefaultGateway 设置给相机的默认网关
 * \retval
 */
MVSTATUS_CODES MVForceIp( const char* pMacAddress, const char* pIpAddress,  const char* pSubnetMask,  const char* pDefaultGateway);

/*!
 * \brief	读取图像宽度
 * \param [in]	hCam		相机句柄
 * \param [out]	pWidth	图像宽度[像素]
 * \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetWidth(HANDLE hCam,unsigned int *pWidth);

/*!
 *  \brief 设置图像的宽度
 *  \param [in]	hCam		相机句柄
 *  \param [in]	nWidth	图像宽度，应该在宽度可设置范围之内，并且 = 最小宽度 + 步长 x 整数
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetWidth(HANDLE hCam, unsigned int nWidth);

/*!
 * \brief	读取图像高度
 * \param [in]	hCam		相机句柄
 * \param [out]	 pHeight	图像高度[像素]
 * \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetHeight(HANDLE hCam, unsigned int *pHeight);

/*!
 *  \brief 设置图像的高度
 *  \param [in]	hCam		相机句柄
 *  \param [in]	nHeight	图像高度，应该在高度可设置范围之内
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetHeight(HANDLE hCam,unsigned int nHeight);

/*!
*  \brief 读取水平方向偏移量。图像宽度设置到小于最大宽度时，可以调整水平偏移量，设置采集窗口的水平起始位置。
*  \param [in]	hCam
*  \param [out]	 pOffsetX	水平偏移量
*  \retval  	MVST_SUCCESS			: 成功
*/
MVSTATUS_CODES MVGetOffsetX(HANDLE hCam, unsigned int* pOffsetX);

/*!
*  \brief 读取水平方向偏移量取值范围。
*  \param [in]	hCam
*  \param [out]	 pOffsetXMin	水平偏移量最小值
*  \param [out]	 pOffsetXMax	水平偏移量最大值
*  \retval  	MVST_SUCCESS			: 成功
*/
MVSTATUS_CODES MVGetOffsetXRange(HANDLE hCam, unsigned int* pOffsetXMin, unsigned int* pOffsetXMax);

/*!
*  \brief 设置水平方向偏移量。图像宽度设置到小于最大宽度时，可以调整水平偏移量，设置采集窗口的水平起始位置。
*  \param [in]	hCam
*  \param [in]	nOffsetX 水平偏移量。应该在水平偏移量允许的范围之内。
*  \retval  	MVST_SUCCESS			: 成功
*/
MVSTATUS_CODES MVSetOffsetX(HANDLE hCam, unsigned int nOffsetX);

/*!
*  \brief 读取垂直方向偏移量。图像宽度设置到小于最大宽度时，可以调整垂直偏移量，设置采集窗口的垂直起始位置。
*  \param [in]	hCam
*  \param [out]	 pOffsetY	垂直偏移量
*  \retval  	MVST_SUCCESS			: 成功
*/
MVSTATUS_CODES MVGetOffsetY(HANDLE hCam, unsigned int* pOffsetY);

/*!
*  \brief 读取垂直方向偏移量取值范围。
*  \param [in]	hCam
*  \param [out]	 pOffsetYMin	垂直偏移量最小值
*  \param [out]	 pOffsetYMax	垂直偏移量最大值
*  \retval  	MVST_SUCCESS			: 成功
*/
MVSTATUS_CODES MVGetOffsetYRange(HANDLE hCam, unsigned int* pOffsetYMin, unsigned int* pOffsetYMax);

/*!
*  \brief 设置垂直方向偏移量。图像宽度设置到小于最大宽度时，可以调整垂直偏移量，设置采集窗口的垂直起始位置。
*  \param [in]	hCam
*  \param [in]	nOffsetY 垂直偏移量。应该在垂直偏移量允许的范围之内。
*  \retval  	MVST_SUCCESS			: 成功
*/
MVSTATUS_CODES MVSetOffsetY(HANDLE hCam, unsigned int nOffsetY);	

/*! 
 *  \brief	设置网络数据包之间的时间间隔。如果网卡或电脑的性能欠佳，无法处理高速到达的数据包，会导致丢失数据包，
			从而使图像不完整。可以通过增加数据包之间的时间间隔以保证图像传输。但是增加该值将增加图像的时间延迟，
			并有可能影像到帧率。
 *  \param [in]	hCam
 *  \param [in]	nDelay_us 时间间隔(单位:微秒)
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetPacketDelay(HANDLE hCam, unsigned int nDelay_us);

/*!
 * \brief	读取网络数据包间隔。
 * \param [in]	hCam				相机句柄
 * \param [out]	pDelay_us 数据包间隔时间，单位us
 *  \retval MVST_SUCCESS				: 成功
 */
MVSTATUS_CODES MVGetPacketDelay(HANDLE hCam, unsigned int* pDelay_us);

/*! 
 *  \brief	设置网络数据包的大小。
 *  \param [in]	hCam			相机句柄
 *  \param [in]	nPacketSize 网络数据包大小(单位:字节)。该大小必须小于网卡能够支持的最大巨型帧(Jumbo Frame)。
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetPacketSize(HANDLE hCam, unsigned int nPacketSize);

/*!
 * \brief	读取网络数据包大小
 * \param [in]	hCam		相机句柄
 * \param [out]	pPacketSize 数据包大小
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetPacketSize(HANDLE hCam, unsigned int* pPacketSize);

/*! 
 *  \brief	读取图像
 *  \param [in]	hCam 相机的句柄
 *  \param [in]	callbackFun 回调函数，类型为CALLBACK_FUN类型
 *  \param [in]	nUserVal	用户数据，传递到回调函数的形参
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVStartGrab(HANDLE hCam, CALLBACK_FUN callbackFun = NULL, long nUserVal = 0);

/*!
 *  \brief 停止采集图像
 *  \param [in]	hCam	相机句柄
 *  \retval  MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVStopGrab(HANDLE hCam);

/*!
 *  \brief 读取图像（在未使用回调函数的情况下，可使用此函数读取图像信息）
 *  \param [in]	hCam	相机句柄
 *  \param [in]	pBuffer	图像指针
 *  \retval  MVST_SUCCESS : 接收到新图像，返回成功
 *           MVST_ERROR ：未接收到新图像，或pData未NULL时，返回错误
 */
MVSTATUS_CODES  MVRead(HANDLE hCam, MVReadBuff* pBuffer);

/*! 
 *  \brief	设置触发模式
 * \param [in]	hCam		相机句柄
 *  \param [in]	mode	触发模式
	TriggerMode_Off：相机工作在连续采集模式，
	TriggerMode_On:相机工作在触发模式，需要有外触发信号或软触发指令才拍摄
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetTriggerMode(HANDLE hCam, TriggerModeEnums mode);

/*!
 * \brief    读取触发模式
 * \param [in]	hCam		相机句柄
 * \param [out]	pMode	触发模式  TriggerMode_Off,TriggerMode_On
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetTriggerMode(HANDLE hCam, TriggerModeEnums* pMode);

/*! 
 *  \brief	设置触发源
 *  \param [in]	hCam		相机句柄
 *  \param [in]	source 触发源
				TriggerSource_Software：通过\c MVTriggerSoftware()函数触发。
				TriggerSource_Line1：通过连接的触发线触发。
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetTriggerSource(HANDLE hCam, TriggerSourceEnums source);

/*!
 * \brief    读取触发源
 * \param [in]	hCam		相机句柄
 * \param [out]	pSource	触发源，软触发或外触发
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetTriggerSource(HANDLE hCam, TriggerSourceEnums* pSource);

/*! 
 *  \brief	当使用触发线触发时,设置是上升沿触发还是下降沿触发
 *  \param [in]	hCam
 *  \param [in]	act 上升沿或下降沿
				TriggerActivation_RisingEdge: 上升沿触发
				TriggerActivation_FallingEdge: 下降沿触发
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetTriggerActivation(HANDLE hCam, TriggerActivationEnums act);

/*!
 * \brief    读取触发极性
 * \param [in]	hCam		相机句柄
 * \param [out]	pAct
				TriggerActivation_RisingEdge: 上升沿触发
				TriggerActivation_FallingEdge: 下降沿触发
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetTriggerActivation(HANDLE hCam, TriggerActivationEnums* pAct);

/*! 
 *  \brief	发出软件触发指令
 *  \param [in]	hCam		相机句柄
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVTriggerSoftware(HANDLE hCam);

/*! 
 *  \brief	闪光同步信号源
 *  \param [in]	hCam
 *  \param [in]	source
				LineSource_Off：关闭闪光同步
				LineSource_ExposureActive：曝光的同时闪光
				LineSource_Timer1Active：由定时器控制
				LineSource_UserOutput0：由用户通过指令控制
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetStrobeSource(HANDLE hCam, LineSourceEnums source);

/*!
 * \brief	读取闪光同步信号源
 * \param [in]	hCam		相机句柄
 * \param [out]	pSource	闪光同步信号源
 * \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetStrobeSource(HANDLE hCam, LineSourceEnums* pSource);

/*! 
 *  \brief	闪光同步是否反转，即闪光同步有效时输出高电平还是低电平。
 *  \param [in]	hCam		相机句柄
 *  \param [in]	bInvert
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetStrobeInvert(HANDLE hCam, unsigned char bInvert);

/*!
 * \brief	读取闪光同步是否反转
 * \param [in]	hCam		相机句柄
 * \param [out]	pInvert
 * \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetStrobeInvert(HANDLE hCam, unsigned char* pInvert);

/*!
 * \brief	设置曝光时间
 * \param [in]	hCam		相机句柄
 * \param [in]	nExp_us 曝光时间 单位为us
 * \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetExposureTime(HANDLE hCam, double nExp_us);

/*! 
 *  \brief	读取当前曝光时间
 *  \param [in]	hCam
 *  \param [in]	pExposuretime	单位us
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetExposureTime(HANDLE hCam, double* pExposuretime);

/*!
 * \brief 设置静态IP地址
 * \param [in] hCam
 * \param [in] pIpAddress	"192.168.0.9"
 * \param [in] pSubnetMask	"255.255.255.0"
 * \param [in] pDefaultGateway	"0.0.0.0"
 * \retval
 */
MVSTATUS_CODES MVSetPersistentIpAddress(HANDLE hCam, const char* pIpAddress, 
	const char* pSubnetMask, const char* pDefaultGateway);
	
/*!
 * \brief 获取相机的静态IP设置
 * \param [in] hCam
 * \param [out] pIpAddress
 * \param [out] pIpAddressLen	pIpAddress缓冲区长度
 * \param [out] pSubnetMask
 * \param [out] pSubnetMaskLen	pSubnetMask缓冲区长度
 * \param [out] pDefaultGateway
 * \param [out] pDefaultGatewayLen	pDefaultGateway缓冲区长度
 * \retval
 */
MVSTATUS_CODES MVGetPersistentIpAddress( HANDLE hCam, unsigned char* pIpAddress, 
	size_t* pIpAddressLen, unsigned char* pSubnetMask, size_t* pSubnetMaskLen, 
	unsigned char* pDefaultGateway, size_t* pDefaultGatewayLen);

/*!
 * \brief    读取当前增益值
 * \param [in]	hCam	相机句柄
 * \param [out]	 pGain
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetGain(HANDLE hCam, double* pGain);

/*!
 * \brief    设置增益
 * \param [in]	hCam	相机句柄
 * \param [in]	 fGain	增益
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetGain(HANDLE hCam, double fGain);

/*!
 * \brief	读取触发延时
 * \param [in]	hCam		相机句柄
 * \param [out]	pDelay_us	触发延时,单位us
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetTriggerDelay(HANDLE hCam, uint32_t* pDelay_us);

/*! 
 *  \brief	设置相机接到触发信号后延迟多少微秒后再开始曝光。
 *  \param [in]	hCam		相机句柄
 *  \param [in]	nDelay_us
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetTriggerDelay(HANDLE hCam, uint32_t nDelay_us);

/*!
 *  \brief	读取并应用某组用户预设的参数
 *  \param [in]	hCam		相机句柄
 *  \param [in]	userset
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVLoadUserSet(HANDLE hCam, UserSetSelectorEnums userset);

/*!
 *  \brief	将当前相机的参数保存到用户设置中。
 *  \param [in]	hCam		相机句柄
 *  \param [in]	userset
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSaveUserSet(HANDLE hCam, UserSetSelectorEnums userset);

/*!
 *  \brief	设置相机上电开机时默认读取并应用哪一组用户设置
 *  \param [in]	hCam		相机句柄
 *  \param [in]	userset
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetDefaultUserSet(HANDLE hCam, UserSetSelectorEnums userset);

/*!
 *  \brief	读取相机上电开机时默认读取并应用哪一组用户设置
 *  \param [in]	hCam		相机句柄
 *  \param [out]	pUserset	用户设置
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetDefaultUserSet(HANDLE hCam, UserSetSelectorEnums* pUserset);

/*!
 *  \brief 获取当前自动曝光模式
 *  \param [in]	hCam	相机句柄
 *  \param [out]	pExposureAuto	当前自动曝光模式
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetExposureAuto(HANDLE hCam, ExposureAutoEnums* pExposureAuto);

/*!
 *  \brief 设置自动曝光模式
 *  \param [in]	hCam	相机句柄
 *  \param [in]	ExposureAuto	自动曝光模式
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetExposureAuto(HANDLE hCam, ExposureAutoEnums ExposureAuto);

/*!	
 *  \brief 获取当前自动增益模式
 *  \param [in]	hCam	相机句柄
 *  \param [out]	pGainAuto	当前自动增益模式的
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetGainAuto(HANDLE hCam, GainAutoEnums* pGainAuto);
	
/*!
 *  \brief 设置当前自动增益模式
 *  \param [in]	hCam	相机句柄
 *  \param [in]	GainAuto	自动增益模式
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetGainAuto(HANDLE hCam, GainAutoEnums GainAuto);

/*!
 *  \brief 获取当前自动白平衡模式
 *  \param [in]	hCam	相机句柄
 *  \param [out]	pBalanceWhiteAuto	当前自动白平衡模式
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetBalanceWhiteAuto(HANDLE hCam, BalanceWhiteAutoEnums* pBalanceWhiteAuto);

/*!
 *  \brief 设置自动白平衡模式
 *  \param [in]	hCam	相机句柄
 *  \param [in]	BalanceWhiteAuto	自动白平衡模式
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetBalanceWhiteAuto(HANDLE hCam, BalanceWhiteAutoEnums BalanceWhiteAuto);

/*!
 * \brief    读取当前白平衡系数
 * \param [in]	hCam		相机句柄
 * \param [out]	 pRed	红色平衡系数
 * \param [out]	 pGreen 绿色平衡系数
 * \param [out]	 pBlue	蓝色平衡系数
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetWhiteBalance(HANDLE hCam, double* pRed, double* pGreen, double* pBlue);

/*!
 * \brief    设置白平衡系数
 * \param [in]	hCam		相机句柄
 * \param [in]	 fRed		红色平衡系数
 * \param [in]	 fGreen	绿色平衡系数
 * \param [in]	 fBlue	蓝色平衡系数
 *  \retval MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetWhiteBalance(HANDLE hCam, double fRed, double fGreen, double fBlue);

/*!
 *  \brief 获取当前伽马值
 *  \param [in]	hCam	相机句柄
 *  \param [out]	 pGamma	当前伽马值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetGamma(HANDLE hCam, double* pGamma);
/*!
 *  \brief 获取伽马值可设置的范围
 *  \param [in]	hCam	相机句柄
 *  \param [out]	 pGammaMin	伽马最小值
 *  \param [out]	 pGammaMax	伽马最大值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetGammaRange(HANDLE hCam, double* pGammaMin, double* pGammaMax);
/*!
 *  \brief 设置伽马值
 *  \param [in]	hCam	相机句柄
 *  \param [in]	 fGamma	伽马值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVSetGamma(HANDLE hCam, double fGamma);
	
/*!
 * \brief    读取图像的像素格式
 * \param [in]	hCam		相机句柄
 * \param [out]	pPixelFormat
 * \retval  	MVST_SUCCESS			: 成功
 */
MVSTATUS_CODES MVGetPixelFormat(HANDLE hCam, MV_PixelFormatEnums* pPixelFormat);

/*!
 * \brief    设置图像的像素格式
 * \param [in]	hCam		相机句柄
 * \param [out]	PixelFormat
 * \retval  	MVST_SUCCESS			: 成功
 */	
MVSTATUS_CODES MVSetPixelFormat( HANDLE hCam,MV_PixelFormatEnums PixelFormat );
	 
/*！
 * \brief   bayer图像转RGB图  
 * \param [in]： bayer 图像指针， 
 * \param [in]： sx bayer图像宽度， 
 * \param [in]：sy bayer图像高度
 * \param [in]：tile 图像格式
 * \param [out]：RGB图像指针  
*/
void MVBayerNearestNeighbor(const unsigned char* bayer, unsigned char* rgb, int sx, int sy, int tile);

/*！
 * \brief   bayer图像转RGB图  
 * \param [in]： bayer 图像指针， 
 * \param [in]： sx bayer图像宽度， 
 * \param [in]：sy bayer图像高度
 * \param [in]：tile 图像格式
 * \param [out]：RGB图像指针  
*/
void MVBayerBilinear(const unsigned char* bayer, unsigned char * rgb, int sx, int sy, int tile);

/*！
 * \brief   bayer图像转RGB图  
 * \param [in]： bayer 图像指针， 
 * \param [in]： sx bayer图像宽度， 
 * \param [in]：sy bayer图像高度
 * \param [in]：tile 图像格式
 * \param [out]：RGB图像指针  
*/
void MVBayerSimple(const unsigned char * bayer, unsigned char * rgb, int sx, int sy, int tile);

#ifdef __cplusplus
}
#endif

#endif //MVGIGE_H_INCLUDE_
