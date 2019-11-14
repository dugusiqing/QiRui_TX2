#!/usr/bin/env sh
# Create the imagenet lmdb inputs
# N.B. set the path to the imagenet train + val data dirs
set -e

EXAMPLE=examples/TSR_LMDB       #设置成生成目录
DATA=data/TSR_Data		#图像数据所在目录，目录下有train文件夹和test文件夹，以及train.txt和test.txt。俩文件夹内放的是图片，由python脚本生成
TOOLS=build/tools		#caffe编译成的lmdb数据集制作工具路径

TRAIN_DATA_ROOT=data/TSR_Data/train/	#设置train文件夹路径
VAL_DATA_ROOT=data/TSR_Data/val/	#设置val文件夹路径

# Set RESIZE=true to resize the images to 256x256. Leave as false if images have
# already been resized using another tool.
RESIZE=true
if $RESIZE; then
  RESIZE_HEIGHT=224		#设置成网络需要的输入分辨率
  RESIZE_WIDTH=224		#设置成网络需要的输入分辨率
else
  RESIZE_HEIGHT=0
  RESIZE_WIDTH=0
fi

if [ ! -d "$TRAIN_DATA_ROOT" ]; then
  echo "Error: TRAIN_DATA_ROOT is not a path to a directory: $TRAIN_DATA_ROOT"
  echo "Set the TRAIN_DATA_ROOT variable in create_imagenet.sh to the path" \
       "where the ImageNet training data is stored."
  exit 1
fi

if [ ! -d "$VAL_DATA_ROOT" ]; then
  echo "Error: VAL_DATA_ROOT is not a path to a directory: $VAL_DATA_ROOT"
  echo "Set the VAL_DATA_ROOT variable in create_imagenet.sh to the path" \
       "where the ImageNet validation data is stored."
  exit 1
fi

echo "Creating train lmdb..."
if [ ! -d $EXAMPLE ];then
   mkdir $EXAMPLE
else
   echo "文件夹已经存在"
fi
rm -rf $EXAMPLE/tsr_train_lmdb	#删除原来存在的文件夹，否则会无法创建
rm -rf $EXAMPLE/tsr_val_lmdb	#删除原来存在的文件夹，否则会无法创建
echo "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
GLOG_logtostderr=1 $TOOLS/convert_imageset \
    --resize_height=$RESIZE_HEIGHT \
    --resize_width=$RESIZE_WIDTH \
    --shuffle \
    $TRAIN_DATA_ROOT \
    $DATA/train.txt \
    $EXAMPLE/tsr_train_lmdb     #创建文件夹，保存train.lmdb
echo "222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222"
GLOG_logtostderr=1 $TOOLS/convert_imageset \
    --resize_height=$RESIZE_HEIGHT \
    --resize_width=$RESIZE_WIDTH \
    --shuffle \
    $VAL_DATA_ROOT \
    $DATA/val.txt \
    $EXAMPLE/tsr_val_lmdb	#创建文件夹，保存val.lmdb
echo "删除原始数据"
rm -rf $DATA
echo "Done."
