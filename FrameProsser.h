#pragma once
#include <string.h>
#include"videosteam.h"
#include <opencv2\opencv.hpp>
#include <thread>
using namespace std;
class FrameProsser
{
public:
	FrameProsser(string	url, videoStream * vs, int width, int height, int fps);
	FrameProsser(string	url, videoStream * vs);
	void operator()();
	int getTotalFrame();
	string url;
	videoStream * vs;
	int width;
	int height;
	int fps;
	cv::VideoCapture cap;
};

