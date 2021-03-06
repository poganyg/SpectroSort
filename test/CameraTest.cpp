#include "Camera.h"
#include "assert_print.h"
using namespace std;
using namespace cv;

/** This test assesses the functionality of the Camera class.*/

int main (int, char**)
{
  Camera camera;
  if ( camera.m_width > 64){assert_print("Image width too large. Consider lowering to reducing processing burden.");}
  if ( camera.m_height > 64){assert_print("Image height too large. Consider lowering to reducing processing burden.");}
  if ( camera.m_fps < 60){assert_print("Frames per second too low. Consider increasing for better classifier performance.");}
  if ( camera.m_brightness > 75){assert_print("Image brightness too high. Consider reducing for more reliable image analysis.");}

  camera.capture();
  int maxVal=0;
  int minVal=255;
  for (int i=0;i<camera.m_width*camera.m_height*3;i++)
  {
    if(maxVal<camera.tempData[i]){maxVal=camera.tempData[i];}
    if(minVal>camera.tempData[i]){minVal=camera.tempData[i];}
  }
  if(minVal==255){assert_print("Minimum value of tempData object following capture is 255. Error in camera operation.");} //CHECK THE SIZE OF THE tempData. IF THE MAX VALUE ==0 or MIN VALUE ==255 THERE IS A PROBLEM WITH CAPTURE.
  if(maxVal==0){assert_print("Maximum value of tempData object following capture is 0. Error in camera operation.");} //CHECK THE SIZE OF THE tempData. IF THE MAX VALUE ==0 or MIN VALUE ==255 THERE IS A PROBLEM WITH CAPTURE.
}
