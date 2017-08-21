#include <stdio.h>
#include <highgui.h>
#include <cv.h>

int main(int argc, char **argv)
{
    IplImage *after_img ,*before_img;
    CvScalar pixel1, pixel2;
    CvCapture* capture = cvCreateCameraCapture(CAP_PARAM);
    float B,G,R,s;

    // if (capture) after_img=cvQueryFrame(capture);
    printf("step1\n");
    before_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    after_img = cvLoadImage (argv[2], CV_LOAD_IMAGE_GRAYSCALE);

    // cvReleaseCapture (&capture);
    // capture = cvCreateCameraCapture(CAP_PARAM);
    // if (capture) after_img=cvQueryFrame(capture);
   // after_img = cvLoadImage (argv[2], CV_LOAD_IMAGE_COLOR);
    
    cvNamedWindow("after_image",1);
    cvNamedWindow("before_image",1);
    cvShowImage("after_image",after_img);    
    cvShowImage("before_image",before_img); 
    while (1){   
      // if (capture) after_img=cvQueryFrame(capture);
      for (int i = 0; i < after_img->height; i++)
	{
	  for (int j = 0; j < after_img->width; j++)
	    {
	      pixel1 = cvGet2D(after_img, i, j);
	      pixel2 = cvGet2D(before_img, i, j);
	      s = pixel1.val[0] - pixel2.val[0];
	      printf("gray value=%f\n",s); 
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



/*

              pixel1 = cvGet2D(after_img, i, j);
	      pixel2 = cvGet2D(before_img, i, j);
	      B = pixel1.val[0] - pixel2.val[0];
	      G = pixel1.val[1] - pixel2.val[1];
	      R = pixel1.val[2] - pixel2.val[2];
	      printf("B=%f,G=%f,R=%f\n", B,G,R);
	      B = 0;
	      G = 0;
	      R = 0;
*/
