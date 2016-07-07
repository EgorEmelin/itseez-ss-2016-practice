#pragma once
#include <memory>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/video.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Tracker {
 public:
  static std::shared_ptr<Tracker> CreateTracker(const std::string &name);
  virtual bool Init(const cv::Mat &frame, const cv::Rect &roi) = 0;
  virtual cv::Rect Track(const cv::Mat &frame) = 0;
};

class MedianFlowTracker : public Tracker {
public:
	virtual bool Init(const cv::Mat &frame, const cv::Rect &roi)
	{
		frame.copyTo(frame_);
		position_ = roi;
		return true;
	};

	virtual cv::Rect Track(const cv::Mat &frame)
	{
		Mat prev_frame;
		Mat cadr = frame(position_);
		frame_.copyTo(prev_frame);
		vector<KeyPoint> features;
		vector<uchar> status, err;
		

	//goodFeaturesToTrack(cadr, features, 50, 0.5, 5);
	//calcOpticalFlowPyrLK(prev_frame, frame_, features_, features, status, err);

		goodFeaturesToTrack(frame, features, 50, 0.5, 5);

		calcOpticalFlowPyrLK(frame_, frame, features_, features, status, err);

		for (int i = 0, erased = 0; i < status.size(); i++)
			if (status[i] != 1) 
			{
				features_.erase(features_.begin() + i - erased);
				features.erase(features.begin() + i - erased);
				erased++;
			}

		vector<float> diffx, diffy;
		for (int i = 0; i < features.size(); i++)
		{
			Point2f oldPoint = features_.at(i).pt;
			Point2f newPoint = features.at(i).pt;

			diffx.push_back(newPoint.x - oldPoint.x);
			diffy.push_back(newPoint.y - oldPoint.y);
		}

		sort(diffx.begin(), diffx.end());
		sort(diffy.begin(), diffy.end());

		position_.x += diffx.at(diffx.size() / 2);
		position_.y += diffy.at(diffy.size() / 2);

		frame_ = frame;
		features_.resize(features.size());
		copy(features.begin(), features.end(), features_.begin());



		return Rect (position_) ;

	};

protected:
	cv::Rect position_;
	cv::Mat frame_;
	vector<KeyPoint> features_;
};


