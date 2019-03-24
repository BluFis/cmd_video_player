#include "pch.h"
#include "videosteam.h"


videoStream * videostream::create() {
	videoStream *head;
	head = new videoStream();
	return head;
}
int  videostream::length(videoStream *head)
{
	int n = 0;
	videoStream *p = head;
	while (p != NULL)
	{
		p = p->next;
		n++;
	}
	return n;
}
videoStream  * videostream::getLastNode(videoStream *head) {
	videoStream *p = head;
	if (!p->next) {
		return p;
	}
	while (p->next != NULL)
	{
		p = p->next;
	}
	return p;
}
videoStream  * videostream::insertFrame(videoStream *head, cv::Mat frame) {
	videoStream *p = getLastNode(head);
	videoStream * nextFrame=new videoStream(frame,p->index+1);
	p->next = nextFrame;
	return head;
}
cv::Mat videostream::readFrame(videoStream *head) {
	videoStream *p = head->next;
	return p->frame;
}
videoStream  * videostream::deleteFrame(videoStream *head) {
	videoStream *p = head;
	if (head->next == NULL) {
		return head;
	}
	if (head->next->next == NULL) {
		free(head->next);
		head->next = NULL;
		return head;
	}
	videoStream *q = head->next;
	p->next = p->next->next;
	free(q);
	return p;
}

videoStream::videoStream(cv::Mat cv, int index)
{
	this->frame = cv;
	this->next = NULL;
	this->totalFrame = 0;
	this->index = index;
}

videoStream::videoStream()
{
	this->index = 0;
	this->totalFrame = 0;
	this->next = NULL;
	this->frame = NULL;
}
