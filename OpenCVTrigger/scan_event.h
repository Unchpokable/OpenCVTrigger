#ifndef SCAN_EVENT_H
#define SCAN_EVENT_H

#include <opencv2/opencv.hpp>

enum ScanEvent_Type
{
	SCANEVENT_DETECT,
	SCANEVENT_UNDEFINED
};

struct ScanEvent
{
	ScanEvent(cv::Mat frame, int diff, ScanEvent_Type type)
		: DetectFrame(frame), DetectDiff(diff), EventType(type)
	{
	}
	cv::Mat DetectFrame;
	int DetectDiff;
	ScanEvent_Type EventType;
};

#endif 
