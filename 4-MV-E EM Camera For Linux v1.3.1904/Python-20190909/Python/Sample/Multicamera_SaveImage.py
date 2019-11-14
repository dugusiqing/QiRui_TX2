#coding=utf-8
import MVGigE 
import numpy as np
import cv2
#执行例子后会显示第一个相机采集到的图，按任意键显示下一个相机的图
def cam_set_para(cam):
    #MVGigE.set(cam, MVGigE.MVCAM_PROP_PACKET_DELAY, 5000);
    #MVGigE.set(cam, MVGigE.MVCAM_PROP_PACKET_SIZE, 8996);    
    MVGigE.set(cam, MVGigE.MVCAM_PROP_EXPOSURE, 9000);
    MVGigE.set(cam, MVGigE.MVCAM_PROP_TRIG_MODE, MVGigE.TriggerMode_Off);
    MVGigE.set(cam, MVGigE.MVCAM_PROP_TRIG_SOURCE, MVGigE.TriggerSource_Software);
iCams = MVGigE.GetNumOfCameras()
print("CamMum = %d" % iCams)
CAM_NUMBER = iCams
cam = np.zeros(CAM_NUMBER)

for index in range(CAM_NUMBER):
    open_state, cam[index] = MVGigE.open(index)
    
    if open_state == True:
        pixel_format = MVGigE.get(cam[index], MVGigE.MVCAM_PROP_PIXELFORMAT)
        print("%d  %d " % (index, cam[index]))
        cam_set_para(cam[index])

        w = MVGigE.get(cam[index], MVGigE.MVCAM_PROP_WIDTH)
        h = MVGigE.get(cam[index], MVGigE.MVCAM_PROP_HEIGHT)
        
        print('[%d] w %d; h %d;' % (index, w, h))
        rgb_image = np.zeros((h, w, 3), np.uint8)
        
        if(MVGigE.start(cam[index])):
            while(1):
                read_state, image = MVGigE.read(cam[index])
                Title = ["CamID : %d"%index]
                if read_state == True:
                    rgb_image = np.zeros((h, w, 3), np.uint8)
                    if pixel_format == MVGigE.PixelFormat_BayerBG8:
                        MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_BG2RGB)
                        cv2.imwrite("img_%d.jpg" % index, rgb_image)
                        cv2.imshow("CamID : %d"%index,rgb_image)
                    elif pixel_format == MVGigE.PixelFormat_BayerRG8:
                        MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_RG2RGB)
                        cv2.imwrite("img_%d.jpg" % index, rgb_image)
                        cv2.imshow("CamID : %d"%index,rgb_image)
                    elif pixel_format == MVGigE.PixelFormat_BayerGB8:
                        MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_GB2RGB)
                        cv2.imwrite("img_%d.jpg" % index, rgb_image)
                        cv2.imshow("CamID : %d"%index,rgb_image)
                    elif pixel_format == MVGigE.PixelFormat_BayerGR8:
                        MVGigE.cvtColor(image, rgb_image, MVGigE.COLOR_BAYER_GR2RGB)
                        cv2.imwrite("img_%d.jpg" % index, rgb_image)
                        cv2.imshow("CamID : %d"%index,rgb_image)
                    else:
                        cv2.imwrite("img_%d.jpg" % index, image)
                        cv2.imshow("CamID : %d"%index,image)
                    break    
                
            MVGigE.stop(cam[index])
            cv2.waitKey(0)
            
    else:
        print('open camera error...')


for index in range(CAM_NUMBER):
    MVGigE.close(cam[index])
