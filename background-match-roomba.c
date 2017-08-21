#include <stdio.h>
#include <highgui.h>
#include <cv.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <termio.h>
#include "roombacomm.h"

#define WIDTH  640
#define HEIGHT 480

int main(int argc, char **argv)
{
  if(argc !=2){
    printf("Input device file.\n");
    return -1;
  }

  int key=0, fd=0;
  float r=0;
  double area=0;
  CvScalar s;
  CvCapture *capture = NULL; 
  char before, after, result, nichika;
  unsigned char buf[21];
  unsigned char start[] = {128};
  unsigned char safe[] = {131};
  unsigned char clean[] = {135};
  
  IplImage *frameImage; 
  IplImage *beforeImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *afterImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *differenceImage =cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *nichiImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);

  fd = setup_serial(argv[1],115200);//open device
  usleep(50000);
  write_serial(fd,start,1);//start
  usleep(50000);

  cvNamedWindow("before",1);
  cvNamedWindow("after",1);
  cvNamedWindow("result",1);
  cvNamedWindow("nichika",1);
  
  if( (capture = cvCreateCameraCapture(0)) == NULL){
    printf("no camera\n");
    return 0;
  } 
  
  while(1){
    frameImage = cvQueryFrame( capture );
    cvCvtColor( frameImage, beforeImage, CV_BGR2GRAY );
    key = cvWaitKey(10);
    if(key==0x61){
      break;}
  } //キー`a`を押して背景画像を読み込む

  while(1){
    frameImage = cvQueryFrame( capture );
    cvCvtColor( frameImage, afterImage, CV_BGR2GRAY );
    cvAbsDiff( afterImage, beforeImage, differenceImage );
    //背景差分    

    cvShowImage( "before", beforeImage);
    cvShowImage( "after", afterImage );
    //背景画像、観測画像の表示    

    cvThreshold( differenceImage, nichiImage, 40, 255, CV_THRESH_BINARY); //二値化
    cvErode( nichiImage, nichiImage, 0, 1); //収縮
    cvDilate( nichiImage, nichiImage, 0, 1); //膨胀
    cvDilate( nichiImage, nichiImage, 0, 1); //膨胀
     
    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *cont = NULL;
    cvFindContours( nichiImage, storage, &cont, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    for(;cont;cont = cont->h_next){
      area=fabs(cvContourArea(cont, CV_WHOLE_SEQ));
      cvDrawContours( nichiImage, cont, CV_RGB(255,255,255), CV_RGB(255,255,255), 2, 1, 8, cvPoint(0, 0));
      if(area>10){
	printf("area=%f\n", area);
      }      
    } //ラベリング
    
    cvShowImage( "result", differenceImage);
    cvShowImage( "nichika", nichiImage);
    //差分画像と二値化結果を表示
    
      if(area>10){
	write_serial(fd,clean,1);
	usleep(10000000);}//start
    //ラベリング面積が10以上だったらロボットが動く
    
    key = cvWaitKey(10);
    if(key==0x1b){
      break;
    } //キー`ESC`を押すと終了
    else if(key==0x61){
      frameImage = cvQueryFrame( capture );
      cvCvtColor( frameImage, beforeImage, CV_BGR2GRAY );
    } //キー`a`を押して背景画像更新
    r=0;
  }
  
  cvReleaseCapture( &capture );
  cvReleaseImage( &beforeImage);
  cvReleaseImage( &afterImage );
  cvReleaseImage( &differenceImage );
  cvReleaseImage( &nichiImage );
  
  cvDestroyWindow( "before" );
  cvDestroyWindow( "after" );
  cvDestroyWindow( "result" );
  cvDestroyWindow( "nichika" );
    
  write_serial(fd,start,1); //cange to passive
  close_serial(fd);
  
  return 0;
 } 
