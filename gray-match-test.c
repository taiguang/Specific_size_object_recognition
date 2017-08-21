#include <stdio.h>
#include <highgui.h>
#include <cv.h>

#define WIDTH  640
#define HEIGHT 480

int main(int argc, char **argv)
{
  int key=0, num=0;
  float max=128,min=128,c=0;
  CvScalar a,b;
  CvCapture *capture = NULL;
  IplImage *frameImage;
  char before, after, result, nichika;

  IplImage *beforeImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *afterImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *differenceImage =cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  IplImage *nichiImage = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);

  cvNamedWindow("before",1);
  cvNamedWindow("after",1);
  cvNamedWindow("result",1);
  cvNamedWindow("nichika",1);

  if( (capture = cvCreateCameraCapture(0)) == NULL){
    printf("no camera\n");
    return 0;
  }
  
  frameImage = cvQueryFrame( capture );
  cvCvtColor( frameImage, beforeImage, CV_BGR2GRAY );
    
  while(1){
    frameImage = cvQueryFrame( capture );
    cvCvtColor( frameImage, afterImage, CV_BGR2GRAY );
    cvSub( afterImage, beforeImage, differenceImage ); 
    
    cvShowImage( "before", beforeImage);
    cvShowImage( "after", afterImage );
    
    for(int i = 0 ; i < differenceImage->height; i++){
      for(int j = 0 ; j < differenceImage->width; j++){	  
	a = cvGet2D( afterImage,i,j);
	   b = cvGet2D( beforeImage,i,j);
	   c = a.val[0] - b.val[0] + 128 ;
	   ((uchar *)(differenceImage->imageData + i*differenceImage->widthStep))[j] = c;
	   if(c>max){
	     max=c;}
	   else if(c<min){
	     min=c;}
      }
    }
    
    cvShowImage( "result", differenceImage);
    cvShowImage( "nichika", nichiImage);
    
    printf("MAX=%f\t MIN=%f\n",max,min);
    
    key = cvWaitKey(10);
    if(key==0x1b){
	 break;
    }else if(key==0x61){
      frameImage = cvQueryFrame( capture );
      cvCvtColor( frameImage, beforeImage, CV_BGR2GRAY );
	 max=128;
	 min=128;
	 c=128;
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
  
  return 0;
}



//差分の結果は全部 + だ//

/*a = cvGet2D( beforeImage,i,j);
  b = cvGet2D( afterImage,i,j);
  f.val[0] = b.val[0] - a.val[0];
  if(f.val[0]<0){
  printf("%f\n",f.val[0]);}
*/
