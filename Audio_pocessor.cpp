#include "pch.h"
#include "Audio_pocessor.h"

Audio_pocessor::Audio_pocessor(const char * url)
{
	this->fmt_ctx = NULL;
	this->ofmt_ctx = NULL;

	//支持各种各样的输出文件格式，MP4，FLV，3GP等等
	this->output_fmt = NULL;

	//输入流
	this->in_stream = NULL;

	//输出流
	this->out_stream = NULL;

	//存储压缩数据
	this->packet;
	this->url = url;
	
}


Audio_pocessor::~Audio_pocessor()
{
}

void Audio_pocessor::operator()()
{
		//要拷贝的流
	int audio_stream_index = -1;
	//打开输入文件，关于输入文件的所有就保存到fmt_ctx中了
	int err_code = avformat_open_input(&(this->fmt_ctx), this->url, NULL, NULL);

	if (err_code < 0) {
		cout << "error decoding audio" << endl;
		return;
	}

	if (this->fmt_ctx->nb_streams < 2) {
		cout << "error decoding audio"<<endl;
		return;
	}

	//拿到文件中音频流
	this->in_stream = this->fmt_ctx->streams[1];
	//参数信息
	AVCodecParameters *in_codecpar = this->in_stream->codecpar;

	//找到最好的音频流
	audio_stream_index = av_find_best_stream(this->fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	if (audio_stream_index < 0) {
		cout << "error decoding audio" << endl;
		return;
	}
	// 输出上下文
	this->ofmt_ctx = avformat_alloc_context();

	//根据目标文件名生成最适合的输出容器
	this->output_fmt = av_guess_format(NULL, "temp.aac", NULL);
	if (!output_fmt) {
		cout << "error decoding audio" << endl;
		return;
	}

	this->ofmt_ctx->oformat = this->output_fmt;

	//新建输出流
	this->out_stream = avformat_new_stream(this->ofmt_ctx, NULL);
	if (!this->out_stream) {
		cout << "error decoding audio" << endl;
		return;
	}
	// 将参数信息拷贝到输出流中，我们只是抽取音频流，并不做音频处理，所以这里只是Copy
	if ((err_code = avcodec_parameters_copy(this->out_stream->codecpar, in_codecpar)) < 0) {
		
	}
	// 初始化AVIOContext, 文件操作由它完成
		if ((err_code = avio_open(&this->ofmt_ctx->pb, "temp.aac", AVIO_FLAG_WRITE)) < 0) {
			cout << "error decoding audio" << endl;
			return;
		}
		// 初始化 AVPacket， 我们从文件中读出的数据会暂存在其中
		av_init_packet(&this->packet);
		this->packet.data = NULL;
		this->packet.size = 0;

		// 写头部信息
		if (avformat_write_header(this->ofmt_ctx, NULL) < 0) {
			cout << "error decoding audio" << endl;
			return;
		}


		//每读出一帧数据
		while (av_read_frame(this->fmt_ctx, &this->packet) >= 0) {
			if (this->packet.stream_index == audio_stream_index) {
				//时间基计算，音频pts和dts一致
				this->packet.pts = av_rescale_q_rnd(this->packet.pts, this->in_stream->time_base, this->out_stream->time_base, AV_ROUND_ZERO);
				this->packet.dts = this->packet.pts;
				this->packet.duration = av_rescale_q(this->packet.duration, this->in_stream->time_base, this->out_stream->time_base);
				this->packet.pos = -1;
				this->packet.stream_index = 0;
				//将包写到输出媒体文件
				av_interleaved_write_frame(this->ofmt_ctx, &this->packet);
				//减少引用计数，避免内存泄漏
				av_packet_unref(&this->packet);
			}
		}

		//写尾部信息
		av_write_trailer(this->ofmt_ctx);

		//最后别忘了释放内存
		avformat_close_input(&this->fmt_ctx);
		avio_close(this->ofmt_ctx->pb);
}
