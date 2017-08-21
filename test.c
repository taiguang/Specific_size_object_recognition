#include <stdio.h>
#include <highgui.h>
#include <cv.h>

#define WIDTH  640
#define HEIGHT 480

int main(int argc, char **argv){

  int key;
  IplImage *image,*dst_img;
  CvCapture *capture = NULL;
  char Image,dst_Image;

  //  image = cvLoadImage (argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  if( (capture = cvCreateCameraCapture(0)) == NULL){
    printf("no Camera\n");
    return 0;
  }
  image = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  dst_img = cvCreateImage( cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U,1);
  
  cvNamedWindow("Image",1); 
  cvNamedWindow("dst_Image",1); 
  
  IplImage *frameimage = cvQueryFrame( capture );
  cvCvtColor( frameimage, image, CV_BGR2GRAY );
  
  while(1){
    cvThreshold( image, dst_img, 128, 255, CV_THRESH_BINARY);
    
    cvShowImage("Image",image);
    cvShowImage("dst_Image",dst_img);
    
    key = cvWaitKey(10);
    if(key==0x1b){
      break;
    }else if(key==0x61){
      frameimage = cvQueryFrame( capture );
      cvCvtColor( frameimage, image, CV_BGR2GRAY );
    }
  }
  cvWaitKey(0);
  cvDestroyWindow("Image");
  cvDestroyWindow("dst_Image");
  cvReleaseImage(&image);
  cvReleaseImage(&dst_img);
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
