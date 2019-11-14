# -*- coding:utf-8 -*-
import os
import random
import math
import cv2 as cv
import caffe as ca


#############################################
#     使用时放在原始数据分文件夹同级目录          #
#     该脚本用于生成train.txt 和 test.txt      #
#     并且自动制作lmdb数据集，放在caffe/examples/TSR_LMDB      #
##############################################




# PathList =["00/" , "01/" , "02/" , "03/" ,
#            "04/" , "05/","06/" , "07/" ,
#            "08/" ,"09/" ,  "10/" , "11/",
#            "12/" , "13/" , "14/","15/"]
#整理好所有交通标志类别的文件夹名称列表
PathList = []
#####获取子文件夹列表
pwd = os.getcwd()
dirs=sorted(os.listdir(pwd))
for subDir in dirs:
    #排除非文件夹，还有pycharm的配置文件夹.idea
    if os.path.isdir(subDir) and subDir != '.idea':
        PathList.append(subDir+"/")


#获取caffe的根目录
caffePythonRoot = ca.__file__
caffeRoot = caffePythonRoot[:-25]
caffeDataRoot = caffeRoot + "/data/"

#制作的数据集路径
Out_Path = caffeDataRoot + "TSR_Data"
#训练图像的路径
trainPath = Out_Path + "/train"
#测试图像的路径
testPath = Out_Path + "/val"
#训练图像所占比例 7:3
train_rate = 0.7
#主程序
if __name__ == "__main__":
    #所有类别的训练和测试图像名称列表
    TrainImageList = []
    TestImageList = []
    #所有类别的训练和测试图像路径列表
    TrainPathList = []
    TestPathList = []
    # 所有类别的训练和测试图像标签列表
    TrainLabelList =[]
    TestLabelList =[]

    #标签序号，与文件夹同步增加
    LableIndex = 0
    for path in PathList:
        #循环遍历文件夹
        images_names = os.listdir(path)
        images_list = []
        #获取文件夹内的图片列表
        for img in images_names:
            images_list.append(img.split('.')[0])

        #随机打乱列表
        random.shuffle(images_list)
        #获取图像个数，即为标签个数
        annotation_num = len(images_list)
        #按照比例计算训练图片数量。训练：测试 = 7 : 3
        train_num = int(math.ceil(annotation_num * train_rate))

        #取打乱顺序的图像的前70%图像作为训练集，剩下的为测试集
        train = images_list[0:train_num]
        test = images_list[train_num:]
        #对测试集图像排序
        train = sorted(train)
        test = sorted(test)
        TmpTrainPath = [path] * train_num
        TmpTestPath = [path ] * (annotation_num-train_num)
        #标签
        TmpTrainLabel =[LableIndex]* train_num
        TmpTestLabel = [LableIndex]* (annotation_num-train_num)

        LableIndex = LableIndex + 1
        #保存到整个的训练集和测试集的列表中，还有对应的路径列表
        TrainImageList += train
        TestImageList += test
        #保存对应的路径
        TrainPathList += TmpTrainPath
        TestPathList += TmpTestPath
        #保存对应的标签
        TrainLabelList += TmpTrainLabel
        TestLabelList += TmpTestLabel
    #判断输出路径是否存在
    if not os.path.exists(Out_Path):
        os.makedirs(Out_Path)
    if not os.path.exists(trainPath):
        os.makedirs(trainPath)
    if not os.path.exists(testPath):
        os.makedirs(testPath)
    #写train.txt
    fout = open(os.path.join(Out_Path, "train.txt"), 'w')
    for i in range(len(TrainImageList)):
        fout.write(str(TrainImageList[i]) + ".jpg" + " " + str(TrainLabelList[i]) + "\n")
	    #将多个分文件夹中的训练图像，全部放入train文件夹
        src = cv.imread("./"+str(TrainPathList[i]) + str(TrainImageList[i]) + ".jpg")
        cv.imwrite(trainPath+"/"+str(TrainImageList[i]) + ".jpg",src)
    fout.close()
    #写val.txt
    fout = open(os.path.join(Out_Path, "val.txt"), 'w')
    for i in range(len(TestImageList)):
        fout.write(str(TestImageList[i]) + ".jpg" + " " + str(TestLabelList[i]) + "\n")
	    #将多个分文件夹中的测试图像，全部放入test文件夹
        src = cv.imread("./" + str(TestPathList[i]) + str(TestImageList[i]) + ".jpg")
        cv.imwrite(testPath + "/" + str(TestImageList[i]) + ".jpg",src)
    fout.close()
    # 将生成lmdb的脚本复制到caffe/data/下面
    os.system("cp lmdb.sh " + Out_Path + "/lmdb.sh")
    os.system("cd "+ caffeRoot+";"+"./data/TSR_Data/lmdb.sh")
