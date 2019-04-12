#include "Classifier.h"


Classifier::Classifier(State* _stateptr, Camera* _cameraptr)
  :stateptr(_stateptr),cameraptr(_cameraptr)
{
  Mat baseFrame(cameraptr->m_height,cameraptr->m_width,CV_8UC3,cameraptr->data);
  Mat baseGrey(cameraptr->m_height,cameraptr->m_width,CV_8UC1);
  Mat rollingFrame(cameraptr->m_height,cameraptr->m_width,CV_8UC3,cameraptr->data);
  Mat rollingGrey(cameraptr->m_height,cameraptr->m_width,CV_8UC1);
}


int Classifier::checkMatch(Mat& baseImage, Mat& rollingImage)
{
  int counter = 0;
  for (int r = 0; r < baseImage.rows; r++)
  {
    for (int c = 0; c < baseImage.cols; c++)
    {
      rollingImage.at<uint8_t>(r, c) = abs(baseImage.at<uint8_t>(r, c) - rollingImage.at<uint8_t>(r, c));
      counter = counter + rollingImage.at<uint8_t>(r, c);
    }
  }
  return counter;
}


void Classifier::classify()
{
  while(stateptr->getBuffer()!=0)
  {
    //StableBaseline
    cameraptr->capture();
    Mat baseFrame(cameraptr->m_height,cameraptr->m_width,CV_8UC3,cameraptr->data);
    cvtColor(baseFrame, baseGrey, COLOR_RGB2GRAY);

    cameraptr->capture();
    Mat rollingFrame(cameraptr->m_height,cameraptr->m_width,CV_8UC3,cameraptr->data);
    cvtColor(rollingFrame,rollingGrey,COLOR_RGB2GRAY);

    baseClear = checkMatch(baseGrey, rollingGrey);

    //MainLoop
    while(true)
    {
      cameraptr->capture();
      Mat rollingFrame(cameraptr->m_height,cameraptr->m_width,CV_8UC3,cameraptr->data);
      cvtColor(rollingFrame,rollingGrey,COLOR_RGB2GRAY);

      //Updating Clear Threshold
      cCounter = checkMatch(baseGrey, rollingGrey);
      if(loopNumber<=25)
      {
        loopNumber++;
        if (baseClear<cCounter)
        {
          baseClear=cCounter;
        }
      }

      cvtColor(rollingFrame,rollingFrame,COLOR_RGB2HSV);
      inRange(rollingFrame, Scalar(20,90,0), Scalar(80,255,255), greenOut);
      gCounter = cv::sum(greenOut);
      inRange(rollingFrame, Scalar(0,50,0), Scalar(30,255,150), brownOut);
      bCounter = cv::sum(brownOut);

      printf("Classifying\n");
      if((gCounter[0]>=100000 && gCounter[0]>=bCounter[0]) ||(gCounter[0]>=10000 && gCounter[0]>=5*bCounter[0]))
      {
        decisionVector[0]=decisionVector[1];
        decisionVector[1]=decisionVector[2];
        decisionVector[2]=1;
      }
      else if((bCounter[0]>=100000 && bCounter[0]>=gCounter[0])||(bCounter[0]>=10000 && bCounter[0]>=5*gCounter[0]))
      {
        decisionVector[0]=decisionVector[1];
        decisionVector[1]=decisionVector[2];
        decisionVector[2]=2;
      }
      else if(cCounter>=baseClear*2)
      {
        decisionVector[0]=decisionVector[1];
        decisionVector[1]=decisionVector[2];
        decisionVector[2]=3;
      }
      else
      {
        decisionVector[0]=decisionVector[1];
        decisionVector[1]=decisionVector[2];
        decisionVector[2]=0;
      }
      if (decisionVector[1]==decisionVector[0] && decisionVector[2]==decisionVector[1] && decisionVector[0]!=0)
      {
        printf("WritingState %i\n", decisionVector[0]);
        stateptr->writeState(decisionVector[0]);
        stateptr->bufferDown();
        if(stateptr->getBuffer()!=0)
        {
          if (bool bufferOn == true){
            auto end = chrono::steady_clock::now();
          }
          auto start = chrono::steady_clock::now();
        }
      }
    }
  }
}
