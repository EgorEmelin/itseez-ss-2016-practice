#include <iostream>
#include <string>
#include "tracking.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
    "{ v video        |        | video to process         }"
    "{ h ? help usage |        | print help message       }";



int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  MedianFlowTracker tracker;
  VideoCapture cap("/Users/iss2016/Documents/summer_school_emelin/itseez-ss-2016-practice/test/test_data/video/logo.mp4");
  Mat frames;


  const string kSrcWindowName = "Destination image";
  const int kWaitKeyDelay = 30;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);
  Rect dts(200,250 , 150, 150);
  for (;;)
  {
	  cap >> frames;
	  tracker.Init(frames, dts);
	  Mat Picture = frames.clone();
	  rectangle(Picture, tracker.Track(frames), CV_RGB(0, 255, 0), 3);
	  imshow(kSrcWindowName, Picture);
	  if (waitKey(kWaitKeyDelay) >= 0) break; 
  }

  waitKey();
  return 0;
}


