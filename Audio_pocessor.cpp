#include "pch.h"
#include "Audio_pocessor.h"

Audio_pocessor::Audio_pocessor(const char * url)
{
	this->fmt_ctx = NULL;
	this->ofmt_ctx = NULL;

	//֧�ָ��ָ���������ļ���ʽ��MP4��FLV��3GP�ȵ�
	this->output_fmt = NULL;

	//������
	this->in_stream = NULL;

	//�����
	this->out_stream = NULL;

	//�洢ѹ������
	this->packet;
	this->url = url;
	
}


Audio_pocessor::~Audio_pocessor()
{
}

void Audio_pocessor::operator()()
{
		//Ҫ��������
	int audio_stream_index = -1;
	//�������ļ������������ļ������оͱ��浽fmt_ctx����
	int err_code = avformat_open_input(&(this->fmt_ctx), this->url, NULL, NULL);

	if (err_code < 0) {
		cout << "error decoding audio" << endl;
		return;
	}

	if (this->fmt_ctx->nb_streams < 2) {
		cout << "error decoding audio"<<endl;
		return;
	}

	//�õ��ļ�����Ƶ��
	this->in_stream = this->fmt_ctx->streams[1];
	//������Ϣ
	AVCodecParameters *in_codecpar = this->in_stream->codecpar;

	//�ҵ���õ���Ƶ��
	audio_stream_index = av_find_best_stream(this->fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	if (audio_stream_index < 0) {
		cout << "error decoding audio" << endl;
		return;
	}
	// ���������
	this->ofmt_ctx = avformat_alloc_context();

	//����Ŀ���ļ����������ʺϵ��������
	this->output_fmt = av_guess_format(NULL, "temp.aac", NULL);
	if (!output_fmt) {
		cout << "error decoding audio" << endl;
		return;
	}

	this->ofmt_ctx->oformat = this->output_fmt;

	//�½������
	this->out_stream = avformat_new_stream(this->ofmt_ctx, NULL);
	if (!this->out_stream) {
		cout << "error decoding audio" << endl;
		return;
	}
	// ��������Ϣ������������У�����ֻ�ǳ�ȡ��Ƶ������������Ƶ������������ֻ��Copy
	if ((err_code = avcodec_parameters_copy(this->out_stream->codecpar, in_codecpar)) < 0) {
		
	}
	// ��ʼ��AVIOContext, �ļ������������
		if ((err_code = avio_open(&this->ofmt_ctx->pb, "temp.aac", AVIO_FLAG_WRITE)) < 0) {
			cout << "error decoding audio" << endl;
			return;
		}
		// ��ʼ�� AVPacket�� ���Ǵ��ļ��ж��������ݻ��ݴ�������
		av_init_packet(&this->packet);
		this->packet.data = NULL;
		this->packet.size = 0;

		// дͷ����Ϣ
		if (avformat_write_header(this->ofmt_ctx, NULL) < 0) {
			cout << "error decoding audio" << endl;
			return;
		}


		//ÿ����һ֡����
		while (av_read_frame(this->fmt_ctx, &this->packet) >= 0) {
			if (this->packet.stream_index == audio_stream_index) {
				//ʱ������㣬��Ƶpts��dtsһ��
				this->packet.pts = av_rescale_q_rnd(this->packet.pts, this->in_stream->time_base, this->out_stream->time_base, AV_ROUND_ZERO);
				this->packet.dts = this->packet.pts;
				this->packet.duration = av_rescale_q(this->packet.duration, this->in_stream->time_base, this->out_stream->time_base);
				this->packet.pos = -1;
				this->packet.stream_index = 0;
				//����д�����ý���ļ�
				av_interleaved_write_frame(this->ofmt_ctx, &this->packet);
				//�������ü����������ڴ�й©
				av_packet_unref(&this->packet);
			}
		}

		//дβ����Ϣ
		av_write_trailer(this->ofmt_ctx);

		//���������ͷ��ڴ�
		avformat_close_input(&this->fmt_ctx);
		avio_close(this->ofmt_ctx->pb);
}
