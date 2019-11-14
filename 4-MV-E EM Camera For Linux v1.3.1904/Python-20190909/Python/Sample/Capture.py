#coding=utf-8
import MVGigE 
import time
import datetime
import cv2
import numpy as np
CAP_TIME = 30

open_state, cam = MVGigE.open(0)

if open_state == True:
	#MVGigE.set(cam, MVGigE.MVCAM_PROP_PACKET_DELAY, 5000);
	MVGigE.set(cam, MVGigE.MVCAM_PROP_PACKET_SIZE, 8996);	
	MVGigE.set(cam, MVGigE.MVCAM_PROP_EXPOSURE, 2000);
	MVGigE.set(cam, MVGigE.MVCAM_PROP_TRIG_MODE, MVGigE.TriggerMode_Off);
	
	w = MVGigE.get(cam, MVGigE.MVCAM_PROP_WIDTH)
	h = MVGigE.get(cam, MVGigE.MVCAM_PROP_HEIGHT)
	pixel_format = MVGigE.get(cam, MVGigE.MVCAM_PROP_PIXELFORMAT)
	rgb_image = np.zeros((h, w, 3), np.uint8)
	print('w %d; h %d;' % (w, h))
	per_image_Rmean = []
	per_image_Gmean = []
	per_image_Bmean = []

	if(MVGigE.start(cam)):
		frames = 0
		start_time = datetime.datetime.now()
	
		while(1):
			#MVGigE.TriggerSoftware(cam)
			
			while 1:
				read_state, image = MVGigE.read(cam)
				#print('w2 %d; h %d' % (w, h))
				#print(read_state)
				#print(pixel_format)
				#print(frames)
				if read_state == True:
					print("[ID:%d] {cam_handle:%d}" % (frames,cam))
					mean = cv2.mean(image)[0]
					print("(Grey_valve:%0.2f)" % mean)
					if pixel_format == MVGigE.PixelFormat_BayerBG8:
						MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_BG2RGB)
						#cv2.imshow('img', rgb_image)
						print(np.mean(rgb_image[:,:,0]))
					elif pixel_format == MVGigE.PixelFormat_BayerRG8:
						MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_RG2RGB)
						#cv2.imshow('img', rgb_image)
						
					elif pixel_format == MVGigE.PixelFormat_BayerGB8:
						MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_GB2RGB)
						#cv2.imshow('img', rgb_image)
					elif pixel_format == MVGigE.PixelFormat_BayerGR8:
						MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_GR2RGB)
						#cv2.imshow('img', rgb_image)
					else:
						#cv2.imshow('img', image)
						 print(np.mean(image[:,:,0]))
					break
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
