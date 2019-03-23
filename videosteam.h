#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string.h>
using namespace std;
struct  videoStream {
	cv::Mat frame;
	struct videoStream *next;
	int totalFrame;
	int width;
	int height;
	int index;
	videoStream(cv::Mat cv,int index);
	videoStream();
};
class videostream
{
public:
public:
	videoStream *create();
	int length(videoStream *head);
	videoStream *getLastNode(videoStream *head);
	videoStream *insertFrame(videoStream *head, cv::Mat frame);
	cv::Mat readFrame(videoStream *head);
	videoStream *deleteFrame(videoStream *head);
};

