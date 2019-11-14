#coding=utf-8
import MVGigE 
import time
import cv2
import datetime
import numpy as np

CAP_TIME = 0
open_state, cam = MVGigE.open(0)

if open_state == True:
	#MVGigE.set(cam, MVGigE.MVCAM_PROP_PACKET_DELAY, 5000);#设置包延时
	MVGigE.set(cam, MVGigE.MVCAM_PROP_PACKET_SIZE, 8996);#设置包大小	
	MVGigE.set(cam, MVGigE.MVCAM_PROP_EXPOSURE, 20000);#设置曝光
	MVGigE.set(cam, MVGigE.MVCAM_PROP_TRIG_MODE, MVGigE.TriggerMode_On);#设置打开触发
	MVGigE.set(cam, MVGigE.MVCAM_PROP_TRIG_SOURCE, MVGigE.TriggerSource_Software);#软触发
	#MVGigE.set(cam, MVGigE.MVCAM_PROP_TRIG_SOURCE, MVGigE.TriggerSource_Line1);#外触发

	w = MVGigE.get(cam, MVGigE.MVCAM_PROP_WIDTH)
	h = MVGigE.get(cam, MVGigE.MVCAM_PROP_HEIGHT)
	pixel_format = MVGigE.get(cam, MVGigE.MVCAM_PROP_PIXELFORMAT)
	rgb_image = np.zeros((h, w, 3), np.uint8)
	print('w %d; h %d;' % (w, h))
	if(MVGigE.start(cam)):
		frames = 0
		start_time = datetime.datetime.now()
	
		while(1):
			#MVGigE.TriggerSoftware(cam)
			#MVGigE.TriggerSoftware(cam)
			#read_state, image = MVGigE.read(cam)
			while (True):
				MVGigE.TriggerSoftware(cam)
				read_state, image = MVGigE.read(cam)
				if (read_state == True):
					print("[ID:%d] {cam_handle:%d}" % (frames,cam))
					mean = cv2.mean(image)[0]
					print("(Grey_valve:%0.2f)" % mean)
				#if read_state == True:
					#if pixel_format == MVGigE.PixelFormat_BayerBG8:
					#	MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_BG2RGB)
						#cv2.imwrite("img_%d.jpg" % frames, rgb_image)
					#	cv2.imshow('img', rgb_image)
					#elif pixel_format == MVGigE.PixelFormat_BayerRG8:
					#	MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_RG2RGB)	
						#cv2.imwrite("img_%d.jpg" % frames, rgb_image)
					#	cv2.imshow('img', rgb_image)
					#elif pixel_format == MVGigE.PixelFormat_BayerGB8:
					#	MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_GB2RGB)
						#cv2.imwrite("img_%d.jpg" % frames, rgb_image)
					#	cv2.imshow('img', rgb_image)
					#elif pixel_format == MVGigE.PixelFormat_BayerGR8:
					#	MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_GR2RGB)
					#	#cv2.imwrite("img_%d.jpg" % frames, rgb_image)
					#	cv2.imshow('img', rgb_image)
					#else:
						#cv2.imwrite("img_%d.jpg" % frames, image)
					#	cv2.imshow('img', image)

					#break
			frames += 1
			if(cv2.waitKey(1) & 0xFF == ord("q")):
				end_time = datetime.datetime.now()
				CAP_TIME = (end_time - start_time).seconds  
				print("frame rate: %d" % (frames / CAP_TIME))
				break	
		MVGigE.stop(cam)
	MVGigE.close(cam)
else:
	print('open camera error...')
