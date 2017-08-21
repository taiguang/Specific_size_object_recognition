#include <stdio.h>
#include <highgui.h>
#include <cv.h>

#define WIDTH  640
#define HEIGHT 480

int main(int argc, char **argv)
{
  int key=0,a=0, hist_size =120, hist_height = 200, k=0;
  float r=0, range[]={0,150};
  float* ranges[]={range};
  double area=0;
  CvCapture *capture = NULL;
  IplImage *frameImage;
  char before, after, result, nichika, histogram;
     
  IplImage *beforeImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *afterImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *differenceImage =cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *nichiImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  
  cvNamedWindow("before",1);
  cvNamedWindow("after",1);
  cvNamedWindow("result",1);
  cvNamedWindow("nichika",1);
  cvNamedWindow("histogram",1);

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
  }
  
  while(1){
    frameImage = cvQueryFrame( capture );
    cvCvtColor( frameImage, afterImage, CV_BGR2GRAY );
    cvAbsDiff( afterImage, beforeImage, differenceImage );

    cvShowImage( "before", beforeImage);
    cvShowImage( "after", afterImage );
    
    cvThreshold( differenceImage, nichiImage, 40, 255, CV_THRESH_BINARY);

    // cvErode( nichiImage, nichiImage, 0, 1); //腐蚀
    // cvDilate( nichiImage, nichiImage, 0, 1); //膨胀
    // cvErode( nichiImage, nichiImage, 0, 1); //腐蚀
    // cvDilate( nichiImage, nichiImage, 0, 1); //膨胀

    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *cont = NULL;
    cvFindContours( nichiImage, storage, &cont, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0,0));       
    for(;cont;cont = cont->h_next){
      area=fabs(cvContourArea(cont, CV_WHOLE_SEQ));
      cvDrawContours( nichiImage, cont, CV_RGB( 255, 255, 255), CV_RGB( 255, 255, 255), -1, 1, 8, cvPoint(0, 0));
      if(area>0){
	printf("area=%f\n", area);
      }     
    }
    printf("\n");
    
    //ヒストグラム
    //创建一维直方图，统计图像在[30 150]像素的均匀分布  
    CvHistogram* gray_hist = cvCreateHist (1,&hist_size,CV_HIST_TREE,ranges,1); 
    //计算灰度图像的一维直方图  
    cvCalcHist (&differenceImage,gray_hist,0,NULL);  
    int scale = 1;  
    //创建一张一维直方图的“图”，横坐标为灰度级，纵坐标为像素个数（*scale）  
    IplImage* hist_image = cvCreateImage (cvSize (hist_size*scale, hist_height),8,1);  
    cvZero (hist_image);  
    //统计直方图中的最大直方块  
    float max_value = 0;  
    cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);  
    //分别将每个直方块的值绘制到图中  
    for(int i=0; i<hist_size; i++)  
      {  
        float bin_val = cvQueryHistValue_1D(gray_hist,i); //像素i的个数
	//		if(bin_val>0){
	//	printf("%d=%f\n", i, bin_val);} 
	int intensity = cvRound(bin_val);  //要绘制的高度   
        cvRectangle(hist_image, 
		    cvPoint(i*scale,hist_height-1),  
		    cvPoint((i+1)*scale - 1, 
			    hist_height - intensity), 
		    CV_RGB(255,255,255), 2, 8, 0);  
      } 	
    //    printf("\n");
    cvWaitKey(0);
    
    cvShowImage( "histogram", hist_image);
    cvShowImage( "result", differenceImage);
    cvShowImage( "nichika", nichiImage);
    
    //   if(area>10){
    //	 break;}
    
    key = cvWaitKey(10);
    if(key==0x1b){
      break;
    }else if(key==0x61){
      frameImage = cvQueryFrame( capture );
      cvCvtColor( frameImage, beforeImage, CV_BGR2GRAY );
    }
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
  cvDestroyWindow( "histogram" );

  return 0;
}

