#pragma once
#include <stdio.h>
#include <windows.h>  
#include <conio.h> 
#include <iostream>
#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavutil/log.h>
#include <libavformat/avformat.h>

};
using namespace std;
class Audio_pocessor
{
public:
	Audio_pocessor(const char* url);
	~Audio_pocessor();
	void operator()();
	AVFormatContext *fmt_ctx = NULL;
	AVFormatContext *ofmt_ctx = NULL;
	AVOutputFormat *output_fmt = NULL;
	AVStream *in_stream = NULL;
	AVStream *out_stream = NULL;
	AVPacket packet;
	const char * url;
};

