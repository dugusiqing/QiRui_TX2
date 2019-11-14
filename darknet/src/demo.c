#include "network.h"
#include "detection_layer.h"
#include "region_layer.h"
#include "cost_layer.h"
#include "utils.h"
#include "parser.h"
#include "box.h"
#include "image.h"
#include "demo.h"
#include <sys/time.h>

#define DEMO 1

#ifdef OPENCV

static char **demo_names;
static image **demo_alphabet;
static int demo_classes;
//全局变量
static network *net;
//图像缓冲循环队列
static image buff [3];
static image buff_letter[3];
static int buff_index = 0;
//全局的相机句柄
static void * cap;
static float fps = 0;
static float demo_thresh = 0;
static float demo_hier = .5;
static int running = 0;
//常量，3
static int demo_frame = 3;
static int demo_index = 0;
//全局
static float **predictions;
static float *avg;
static int demo_done = 0;
static int demo_total = 0;
//计时变量
double demo_time;

detection *get_network_boxes(network *net, int w, int h, float thresh, float hier, int *map, int relative, int *num);

//记录所有的输出个数
int size_network(network *net)
{
    int i;
    int count = 0;
    for(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        //Region层的输出，YOLO层的输出，Detetion层的输出
        if(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            count += l.outputs;
        }
    }
    return count;
}

void remember_network(network *net)
{
    int i;
    int count = 0;
    for(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        if(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(predictions[demo_index] + count, net->layers[i].output, sizeof(float) * l.outputs);
            count += l.outputs;
        }
    }
}

detection *avg_predictions(network *net, int *nboxes)
{
    int i, j;
    int count = 0;
    fill_cpu(demo_total, 0, avg, 1);
    for(j = 0; j < demo_frame; ++j){
        axpy_cpu(demo_total, 1./demo_frame, predictions[j], 1, avg, 1);
    }
    for(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        if(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(l.output, avg + count, sizeof(float) * l.outputs);
            count += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    return dets;
}

//采用的是循环中开线程，提前缓存下一帧，然后处理当前帧，显示前一帧处理结果。

//反复循环 0 1 2 0 1 2....0 1 2. 直到视频或者摄像头停止

//初始状态：buff[0] = buff[1] = buff[2]
//第一次循环时 buff_index = 1
//fetch_in_thread，更新了buff[1]和buff_letter[1]
//detect_in_thread，处理buff_letter[0]，更新了buff[0]，用来显示。
//display_in_thread，显示了buff[2]

//第二次循环时 buff_index = 2
//fetch_in_thread，更新了buff[2]和buff_letter[2]
//detect_in_thread，处理buff_letter[1]，更新了buff[1]，用来显示。
//display_in_thread，显示了buff[0]

//第三次循环时 buff_index = 0
//fetch_in_thread，更新了buff[0]和buff_letter[0]
//detect_in_thread，处理buff_letter[2]，更新了buff[2]，用来显示。
//display_in_thread，显示了buff[1]

//第四次循环时 buff_index = 1
//fetch_in_thread，更新了buff[1]和buff_letter[1]
//detect_in_thread，处理buff_letter[0]，更新了buff[0]，用来显示。
//display_in_thread，显示了buff[2]


//消费者
void *detect_in_thread(void *ptr)
{
    running = 1;
    float nms = .4;
    
    layer l = net->layers[net->n-1];
    //第一次循环时，处理的是第0帧 buff[0]
    float *X = buff_letter[(buff_index+2)%3].data;
    
    network_predict(net, X);

    remember_network(net);
    detection *dets = 0;
    int nboxes = 0;
    dets = avg_predictions(net, &nboxes);

    if (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n",fps);
    printf("Objects:\n\n");
    //要显示的图像是buff[0]
    image display = buff[(buff_index+2) % 3];
    //在buff[0]上画出框
    draw_detections(display, dets, nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes);
    //释放检测结果内存
    free_detections(dets, nboxes);
    demo_index = (demo_index + 1)%demo_frame;
    running = 0;
    return 0;
}

//采集线程，生产者
void *fetch_in_thread(void *ptr)
{
    //释放buff[1]
    free_image(buff[buff_index]);
    //获取一帧，填充到buff[1]

    /*可以借鉴一下，传入相机采集对象，然后触发一下，采集一帧，cap是全局的对象*/
    buff[buff_index] = get_image_from_stream(cap);
    //如果数据为空，停止循环
    if(buff[buff_index].data == 0) {
        demo_done = 1;
        return 0;
    }
    //把新的buff[1]，转换成待检测的buff_letter[1]。
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    return 0;
}

//消费者
void *display_in_thread(void *ptr)
{
    int c = show_image(buff[(buff_index + 1)%3], "Demo", 1);
    if (c != -1) c = c%256;
    if (c == 27) 
    {
        demo_done = 1;
        return 0;
    } 
    else if (c == 82) 
    {
        demo_thresh += .02;
    } 
    else if (c == 84) 
    {
        demo_thresh -= .02;
        if(demo_thresh <= .02) 
        demo_thresh = .02;
    } 
    else if (c == 83) 
    {
        demo_hier += .02;
    } 
    else if (c == 81) 
    {
        demo_hier -= .02;
        if(demo_hier <= .0) 
        demo_hier = .0;
    }
    return 0;
}

void *display_loop(void *ptr)
{
    while(1){
        display_in_thread(0);
    }
}

void *detect_loop(void *ptr)
{
    while(1){
        detect_in_thread(0);
    }
}
//cfg 网络配置文件 
//weightfile 
/*
char *cfg ：网络配置文件，输入
char *weightfile ：网络权重文件，输入
float thresh：检测框的类别置信度阈值，
int cam_index：相机序号
const char *filename：待检测文件名
char **names：待检测类别标签映射文件
int classes：待检测类别数
int delay：跳过帧数，默认是0
char *prefix：前缀，默认是空
int avg_frames：帧平均，默认是3
float hier：层次阈值，默认0.5
int w：图像宽度 ，用不到
int h：图像高度，用不到
int frames：帧率，用不到
int fullscreen：用不到
*/
void demo(char *cfgfile, char *weightfile, float thresh, int cam_index, const char *filename, char **names, int classes, int delay, char *prefix, int avg_frames, float hier, int w, int h, int frames, int fullscreen)
{
    //demo_frame = avg_frames;
    image **alphabet = load_alphabet();
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    printf("Demo\n");
    //加载网络
    net = load_network(cfgfile, weightfile, 0);
    //设置单张图像
    set_batch_network(net, 1);
    //线程句柄
    pthread_t detect_thread;
    pthread_t fetch_thread;

    srand(2222222);

    int i;

    demo_total = size_network(net);
    //声明一个数组，数组有三个元素，每个元素是float型的指针，数组的名字叫做predictions
    //相当于pridctions是一个图像数组
    predictions = calloc(demo_frame, sizeof(float*));
    for (i = 0; i < demo_frame; ++i){
        predictions[i] = calloc(demo_total, sizeof(float));
    }

    avg = calloc(demo_total, sizeof(float));

    if(filename){
        printf("video file: %s\n", filename);
        //打开视频文件
        cap = open_video_stream(filename, 0, 0, 0, 0);
    }else{
        //打开摄像头
        cap = open_video_stream(0, cam_index, w, h, frames);
    }
    //打开失败
    if(!cap) error("Couldn't connect to webcam.\n");
    
    //第一帧
    buff[0] = get_image_from_stream(cap);
    //第一帧
    buff[1] = copy_image(buff[0]);
    //第一帧
    buff[2] = copy_image(buff[0]);
    
    //做成网络需要的输入图像格式
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    int count = 0;
    //创建一个窗口
    if(!prefix){
        make_window("Demo", 1352, 1013, fullscreen);
    }


    demo_time = what_time_is_it_now();
    
    //在循环中开线程，而不是线程中循环。
    while(!demo_done){
        //buff_index  始终为 0 ，1 ，2  
        buff_index = (buff_index + 1) %3;
        //第一次循环 buff_index = 1  
        //第二次循环 buff_index = 2
        //第三次循环 buff_index = 0

        //开线程，先开采集线程，再开检测线程
        if(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        if(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        if(!prefix){
            //显示帧率
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            //回调显示结果，这个是主线程
            display_in_thread(0);
        }else{
            //保存
            char name[256];
            sprintf(name, "%s_%08d", prefix, count);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        //等待检测线程完成，再进行下一轮循环
        ++count;
    }
}

/*
   void demo_compare(char *cfg1, char *weight1, char *cfg2, char *weight2, float thresh, int cam_index, const char *filename, char **names, int classes, int delay, char *prefix, int avg_frames, float hier, int w, int h, int frames, int fullscreen)
   {
   demo_frame = avg_frames;
   predictions = calloc(demo_frame, sizeof(float*));
   image **alphabet = load_alphabet();
   demo_names = names;
   demo_alphabet = alphabet;
   demo_classes = classes;
   demo_thresh = thresh;
   demo_hier = hier;
   printf("Demo\n");
   net = load_network(cfg1, weight1, 0);
   set_batch_network(net, 1);
   pthread_t detect_thread;
   pthread_t fetch_thread;

   srand(2222222);

   if(filename){
   printf("video file: %s\n", filename);
   cap = cvCaptureFromFile(filename);
   }else{
   cap = cvCaptureFromCAM(cam_index);

   if(w){
   cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH, w);
   }
   if(h){
   cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT, h);
   }
   if(frames){
   cvSetCaptureProperty(cap, CV_CAP_PROP_FPS, frames);
   }
   }

   if(!cap) error("Couldn't connect to webcam.\n");

   layer l = net->layers[net->n-1];
   demo_detections = l.n*l.w*l.h;
   int j;

   avg = (float *) calloc(l.outputs, sizeof(float));
   for(j = 0; j < demo_frame; ++j) predictions[j] = (float *) calloc(l.outputs, sizeof(float));

   boxes = (box *)calloc(l.w*l.h*l.n, sizeof(box));
   probs = (float **)calloc(l.w*l.h*l.n, sizeof(float *));
   for(j = 0; j < l.w*l.h*l.n; ++j) probs[j] = (float *)calloc(l.classes+1, sizeof(float));

   buff[0] = get_image_from_stream(cap);
   buff[1] = copy_image(buff[0]);
   buff[2] = copy_image(buff[0]);
   buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
   buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
   buff_letter[2] = letterbox_image(buff[0], net->w, net->h);
   ipl = cvCreateImage(cvSize(buff[0].w,buff[0].h), IPL_DEPTH_8U, buff[0].c);

   int count = 0;
   if(!prefix){
   cvNamedWindow("Demo", CV_WINDOW_NORMAL); 
   if(fullscreen){
   cvSetWindowProperty("Demo", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
   } else {
   cvMoveWindow("Demo", 0, 0);
   cvResizeWindow("Demo", 1352, 1013);
   }
   }

   demo_time = what_time_is_it_now();

   while(!demo_done){
buff_index = (buff_index + 1) %3;
if(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
if(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
if(!prefix){
    fps = 1./(what_time_is_it_now() - demo_time);
    demo_time = what_time_is_it_now();
    display_in_thread(0);
}else{
    char name[256];
    sprintf(name, "%s_%08d", prefix, count);
    save_image(buff[(buff_index + 1)%3], name);
}
pthread_join(fetch_thread, 0);
pthread_join(detect_thread, 0);
++count;
}
}
*/
#else
void demo(char *cfgfile, char *weightfile, float thresh, int cam_index, const char *filename, char **names, int classes, int delay, char *prefix, int avg, float hier, int w, int h, int frames, int fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif