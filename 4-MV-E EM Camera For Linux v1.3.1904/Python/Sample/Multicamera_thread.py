#coding=utf-8
import MVGigE 
import threading
import datetime
import time
import numpy as np
#import cv2
import sys

'''
@ 线程类
@ 继承于 threading.Thread 类， 使上层调用者可以手动开启 结束线程
'''
class Capture_Thread(threading.Thread): 
    
	'''
	@ 构造函数
	@ in fun      线程循环执行的函数 该函数有两个输入参数  （回调函数， 整数）
	@    paras    线程执行函数的输入参数

	'''
	def __init__(self, fun, paras): 
		threading.Thread.__init__(self) 
		self.fun = fun 
		self.thread_stop = False
		self.cam_handle = paras[0]
		self.pixel_format = paras[1]
		self.run_stop = False
		self.start_time = datetime.datetime.now()
		self.rec_frames = 0
		self.last_block_id = 0
		self.last_img_mean = 0
		self.last_img_var = 0

	def run(self): 
		while not self.thread_stop: 
			ret, block_id, img_mean, img_var  = self.fun(self.cam_handle, self.pixel_format) 
			if not ret:
				time.sleep(0.01)
			else:
				if self.last_block_id > 0:
					if (self.last_block_id < 65535) and ((block_id - self.last_block_id) > 1):
						print('[%d]lose block %d - %d ' % (self.cam_handle, self.last_block_id + 1, block_id - 1))
					
					if (self.last_block_id == 65535) and (block_id > 1):
						print('[%d]lose block %d - %d ' % (self.cam_handle, 1, block_id - 1))

					#if abs(img_mean - self.last_img_mean) > 5:
					#	print('%d camera %d block mean_diff:%d' % (self.cam_handle, block_id, abs(img_mean - self.last_img_mean)))

					#if abs(img_var - self.last_img_var) > 10:
					#	print('%d camera %d block var_diff:%d' % (self.cam_handle, block_id, abs(img_var - self.last_img_var)))

				self.last_block_id = block_id
				self.last_img_mean = img_mean
				self.last_img_var = img_var
				
				self.rec_frames += 1
				if (self.rec_frames > 30):
					cur_time = datetime.datetime.now()
					print('[%d] rate: %f' % (self.cam_handle, self.rec_frames /  (cur_time - self.start_time).total_seconds()))
					self.rec_frames = 0
					self.start_time = datetime.datetime.now()
		self.run_stop  = True
		
	def stop(self): 
		self.thread_stop = True
		cur_time = datetime.datetime.now()
		print('[%d] rate: %f' % (self.cam_handle, self.rec_frames /  (cur_time - self.start_time).total_seconds()))
		while not self.run_stop:
			time.sleep(0.1)


'''
@线程图像接收处理函数
@paras 输入参数
'''
def thread_fun(cam_handle, pixel_format):

	block_id, image = MVGigE.read(cam_handle)

	if block_id >= 1 and block_id <= 65535:
		'''
		if g_mutex.acquire(False):
			print("[%d] locked" % cam_handle)
			h, w = image.shape
			rgb_image = np.zeros((h, w, 3), np.uint8)
			if pixel_format == MVGigE.PixelFormat_BayerBG8:
				MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_BG2RGB)
				cv2.imshow("CamHandle : %d"%cam_handle, rgb_image)
			elif pixel_format == MVGigE.PixelFormat_BayerRG8:
				MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_RG2RGB)
				cv2.imshow("CamHandle : %d"%cam_handle, rgb_image)
			elif pixel_format == MVGigE.PixelFormat_BayerGB8:
				MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_GB2RGB)
				cv2.imshow("CamHandle : %d"%cam_handle, rgb_image)
			elif pixel_format == MVGigE.PixelFormat_BayerGR8:
				MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_GR2RGB)
				cv2.imshow("CamHandle : %d"%cam_handle, rgb_image)
			else:
				cv2.imshow("CamHandle : %d"%cam_handle, image)

			print('[%d] %d' % (cam_handle, image[100][100]))
			cv2.waitKey(1)
			print("[%d] unlocked" % cam_handle)
			g_mutex.release()
		'''
		#image_tmp = image.reshape(-1)

		#print('[%d] %d' % (cam_handle, image[100][100]))
		return True, block_id, 0, 0#np.mean(image_tmp), np.var(image_tmp)
	return False, 0, 0, 0
			
def cam_set_para(cam):
	#MVGigE.set(cam, MVGigE.MVCAM_PROP_PACKET_DELAY, 5000);
	MVGigE.set(cam, MVGigE.MVCAM_PROP_PACKET_SIZE, 8996);	
	MVGigE.set(cam, MVGigE.MVCAM_PROP_EXPOSURE, 2000);
	MVGigE.set(cam, MVGigE.MVCAM_PROP_TRIG_MODE, MVGigE.TriggerMode_Off);
    
if __name__ == "__main__":
	capture_time = 10  #
	#print('times: ', sys.argv[1])
	cam_numbers = MVGigE.GetNumOfCameras()
	print("CamMum = %d" % cam_numbers)

	threads = []
	cam_handles = []
	cam_handles_tmp = []
	threads_tmp = []

	for index in range(cam_numbers):
		thread = -1
		open_state, cam_handle = MVGigE.open(index)
		if open_state:
			cam_set_para(cam_handle)
			pixel_format = MVGigE.get(cam_handle, MVGigE.MVCAM_PROP_PIXELFORMAT)
			thread = Capture_Thread(thread_fun, [cam_handle, pixel_format])
			thread.start()
			cam_handles_tmp.append(cam_handle)
			threads_tmp.append(thread)
		else:
			if cam_handle == MVGigE.MVST_ERROR:
				print("open error")
			elif cam_handle == MVGigE.MVST_INVALID_PARAMETER:
				print("incalied parameter")
			elif cam_handle == MVGigE.MVST_ACCESS_DENIED:
				print("access denied")
			elif cam_handle == MVGigE.MVST_INVALID_ID :
				print("invalid id")
			else:
				print("other error [%d]" % cam_handle)

	for index in range(len(cam_handles_tmp)):
		#open_state, cam_handle = MVGigE.open(index)
		#if open_state == True:
		cam_handle = cam_handles_tmp[index]
		thread = threads_tmp[index]

		if(MVGigE.start(cam_handle)):
			#cv2.namedWindow("CamHandle : %d"%cam_handle)
			
			
			threads.append(thread)

			cam_handles.append(cam_handle)
		else:
			MVGigE.close(cam_handle)
			if thread != -1:
				thread.stop()

	start_time = datetime.datetime.now()
	
	while 1:
		end_time = datetime.datetime.now()
		if (end_time - start_time).seconds > capture_time:
			break
		else:
			time.sleep(0.1)

	print("time end...")

	for index in range(len(threads)):
		if (threads[index] != -1):
			threads[index].stop()
			print('%d camera thread stop' % cam_handles[index])

	for index in range(len(cam_handles)):
		if (cam_handles[index] != 0):
			MVGigE.stop(cam_handles[index])
			MVGigE.close(cam_handles[index])
			print('%d cam close' % cam_handles[index])

	#cv2.destroyAllWindows()

