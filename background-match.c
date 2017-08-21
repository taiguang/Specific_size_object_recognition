#include <stdio.h>
#include <highgui.h>
#include <cv.h>

#define WIDTH  640
#define HEIGHT 480

int main(int argc, char **argv)
{
  int key=0, hist_size = 256, hist_height = 100;
  float r=0, range[]={3,128};
  float* ranges[]={range};
  double area=0;
  CvScalar s;
  CvCapture *capture = NULL;
  IplImage *frameImage;
  char before, after, result, nichika, labeling, histogram;
     
  IplImage *beforeImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *afterImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *differenceImage =cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *nichiImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *labelingImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,3);
  
  cvNamedWindow("before",1);
  cvNamedWindow("after",1);
  cvNamedWindow("result",1);
  cvNamedWindow("nichika",1);
  cvNamedWindow("labeling",1);
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
    
    for(int i = 0 ; i < differenceImage->height; i++){
      for(int j = 0 ; j < differenceImage->width; j++){
	s = cvGet2D( differenceImage, i,j);
	/*	if(30 > s.val[0] > 0){
	  s.val[0]=10;
	}else if(60 > s.val[0] > 30){
	  s.val[0]=20;
	}else if(90 > s.val[0] > 60){
	  s.val[0]=30;
	}else if(120 > s.val[0] > 90){
	  s.val[0]=40;
	}else if(s.val[0] > 120){
	  s.val[0]=50;
	  }*/
      } 
    }
//    cvThreshold( differenceImage, nichiImage, 0, 255, CV_THRESH_BINARY);
/*    cvZero(labelingImage);
    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *cont = NULL;
    cvFindContours( differenceImage, storage, &cont, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0,0));       
    for(;cont;cont = cont->h_next){
      area=fabs(cvContourArea(cont, CV_WHOLE_SEQ));
      printf("area=%f\n", area);     
      //      cvDrawContours( labelingImage, cont, CV_RGB( 255, 0, 0), CV_RGB( 255, 0, 0), 2, 1, 8, cvPoint(0, 0));
      CvScalar color = CV_RGB(rand()&255, rand()&255, rand()&255);    
      cvDrawContours( labelingImage, cont, color, color, 1, 1, 8, cvPoint(0, 0));
      }*/
  
    //创建一维直方图，统计图像在[0 255]像素的均匀分布  
    CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1); 
    //计算灰度图像的一维直方图  
    cvCalcHist(&differenceImage,gray_hist,0,NULL);  
    //归一化直方图  
    cvNormalizeHist(gray_hist,1.0); 
    int scale = 1;  
    //创建一张一维直方图的“图”，横坐标为灰度级，纵坐标为像素个数（*scale）  
    IplImage* hist_image = cvCreateImage (cvSize(hist_size*scale,hist_height),8,1);  
    cvZero(hist_image);  
    //统计直方图中的最大直方块  
    float max_value = 0;  
    cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);  
    //分别将每个直方块的值绘制到图中  
    for(int i=0;i<hist_size;i++)  
      {  
        float bin_val = cvQueryHistValue_1D(gray_hist,i); //像素i的概率  
	if(bin_val>r){
	  r=bin_val;
	  printf("%d/n",i);}
        int intensity = cvRound(bin_val*hist_height/max_value);  //要绘制的高度 
        cvRectangle(hist_image, cvPoint(i*scale,hist_height-1),  cvPoint((i+1)*scale - 1, hist_height - intensity), CV_RGB(255,255,255));    
      } 
 

    cvShowImage( "histogram", hist_image);
    cvShowImage( "result", differenceImage);
    // cvShowImage( "nichika", nichiImage);
    // cvShowImage( "labeling", labelingImage);
    //       printf("MAX=%f\n",r);
    
    //       if(area>1){
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
  cvReleaseImage( &labelingImage );


  cvDestroyWindow( "before" );
  cvDestroyWindow( "after" );
  cvDestroyWindow( "result" );
  cvDestroyWindow( "nichika" );
  cvDestroyWindow( "labeling" );
  cvDestroyWindow( "histogram" );

  return 0;
}

















/*    while (1){   
      // if (capture) after_img=cvQueryFrame(capture);
      for (int i = 0; i < after_img->height; i++)
	{
	  for (int j = 0; j < after_img->width; j++)
	    {
	      pixel1 = cvGet2D(after_img, i, j);
	      pixel2 = cvGet2D(before_img, i, j);
	      B = pixel1.val[0] - pixel2.val[0];
	      G = pixel1.val[1] - pixel2.val[1];
	      R = pixel1.val[2] - pixel2.val[2];
	      printf("B=%f,G=%f,R=%f\n", B,G,R);
	      B = 0;
	      G = 0;
	      R = 0;
	    }
	}
      cvWaitKey(0);
 

    }

    
    cvWaitKey(0);
    cvDestroyWindow("after_image");
    cvDestroyWindow("before_image");
    cvReleaseImage(&before_img);
    cvReleaseImage(&after_img);
    
    //  cvReleaseCapture (&capture);
    return 0;
}

*/
