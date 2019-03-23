#include "pch.h"
#include "FrameProsser.h"

FrameProsser::FrameProsser(string url, videoStream * vs, int width, int height, int fps)
{
	this->url = url;
	this->vs = vs;
	this->fps = fps;
	this->width = width;
	this->height = height;
}

FrameProsser::FrameProsser(string url, videoStream * vs)
{
	this->url = url;
	this->vs = vs;
	this->fps = 8;
	this->width = 160;
	this->height = 45;
	cv::VideoCapture capture(this->url);
	this->cap = capture;
}

void FrameProsser::operator()()
{
	
	if (!this->cap.isOpened()) {
		cout << "fail toopen!" << endl;
		return;
	}
	long frameToStart = 1;
	this->cap.set(cv::CAP_PROP_POS_FRAMES, frameToStart);
	//设置结束帧  
	int frameToStop = this->getTotalFrame();
	if (frameToStop < frameToStart)
	{
		cout << "结束帧小于开始帧，程序错误，即将退出！" << endl;
		return;
	}
	
	this->vs->height = this->height;
	this->vs->width = this->width;
	long currentFrame = frameToStart;
	cv::Mat frame;
	const int jiange = int(this->cap.get(cv::CAP_PROP_FPS) / this->fps);
	this->vs->totalFrame = frameToStop/jiange;
	int kernel_size = 3;
	cv::Mat kernel = cv::Mat::ones(kernel_size, kernel_size, CV_8UC1) / (float)(kernel_size*kernel_size);
	while (currentFrame!=frameToStop)
	{
		if (!this->cap.read(frame))
		{
			cout << "读取视频失败" << endl;
			return;
		}
		if (currentFrame%jiange == 0) {
			//读取下一帧  
			
			cv::Size dsize = cv::Size(this->width, this->height);
			cv::Mat image2 = cv::Mat(dsize, CV_8UC1);
			resize(frame, image2, dsize);
			videostream().insertFrame(this->vs, image2);
		}
		currentFrame++;
	}
	//关闭视频文件  
	this->cap.release();
}

int FrameProsser::getTotalFrame()
{
	long totalFrameNumber = long(this->cap.get(cv::CAP_PROP_FRAME_COUNT));
	return totalFrameNumber;
}
