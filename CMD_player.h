#pragma once
#include"videosteam.h"
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string.h>
#define charsetL 10
using namespace std;
class CMD_player
{
public:
	CMD_player(videoStream * vs);
	char img_to_txt(int gray);
	void gotoxy();
	void operator()();
	videoStream * vs;
	videoStream * current;
	HWND hq;
	COORD pos;
	COORD processpos;
	COORD voice;
};

