#include<stdio.h>
#include<pthread.h>
#include<iostream>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include <unistd.h>
#include<stdlib.h>
#define TIMER 5
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480
#define IMAGE_WIDTH 200
#define IMAGE_HEIGHT 200
  
struct data{
  int end, capture;
  IplImage *img, *dst, *picture, *picture2;
};

int calc_node_num()
{
  int node_num;
  char filename[32];
  for(node_num=0;;node_num++){
    sprintf(filename,"%03d.bmp",node_num);  //bmp,jpg
    if(access(filename,0)!=0){break;}
  }
  return node_num;
}

void *capturingmovie( void *arg )
{
  struct data *d = (struct data*)arg;
  d->dst = cvCreateImage( cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U, 3);
  CvCapture *capture = cvCreateCameraCapture (0);
  int c;
  CvRect roi;
  d->end=0;
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
  cvNamedWindow ("Capture", CV_WINDOW_AUTOSIZE);

  roi.x=(CAMERA_WIDTH-IMAGE_WIDTH)/2;
  roi.y=(CAMERA_HEIGHT-IMAGE_HEIGHT)/2;
  //  roi.y=0;
  roi.width=IMAGE_WIDTH;
  roi.height=IMAGE_HEIGHT;
  while (1) {
    d->img = cvQueryFrame (capture);
    //cvResize(d->img,d->dst,CV_INTER_CUBIC);
    cvSetImageROI (d->img, roi);
    cvCopy (d->img, d->dst);
    cvShowImage ("Capture", d->dst);
    c = cvWaitKey (2);
    if (c == '\x1b')
      break;
  }
  d->end=1;
  cvReleaseCapture (&capture);
  cvDestroyWindow ("Capture");
}

int main(int argc, char **argv){
  pthread_t capturingmovie_t;
  struct data d;
  int node_num;
  char filename[64];
  pthread_create( &capturingmovie_t , NULL , capturingmovie, &d );
  for(int i=0;i<TIMER*100;i++){
    usleep(10000);
    if(d.end==1)
      return -1;
  }
  node_num = calc_node_num();
  sprintf(filename,"%03d.bmp",node_num);//bmp,jpg
  cvSaveImage(filename,d.dst);
}
