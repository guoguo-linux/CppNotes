#include "XVideoWriter.h"
extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/opt.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
}

#include <iostream>
using namespace std;

class CXVideoWriter : public XVideoWriter {
   public:
    void close() {
        if (mInputContext) {
            //传入指针的地址，指针在接口内部会被置空
            avformat_close_input(&mInputContext);
        }
        if (mVCodecCxt) {
            //先关闭编码器再清空间
            avcodec_close(mVCodecCxt);
            avcodec_free_context(&mVCodecCxt);
        }
        if (mACodecCxt) {
            avcodec_close(mACodecCxt);
            avcodec_free_context(&mACodecCxt);
        }
        if (mVideoSwsContext) {
            //传入指针，需要手动置空指针
            sws_freeContext(mVideoSwsContext);
            mVideoSwsContext = NULL;
        }

        if (mYuvFrame) {
            av_frame_free(&mYuvFrame);
        }

        if (mAudioSwrCtx) {
            swr_free(&mAudioSwrCtx);
        }

        if (mPcmFrame) {
            av_frame_free(&mPcmFrame);
        }
    }
    bool init(const char* file) {
        close();

        //初始化封装文件上下文
        avformat_alloc_output_context2(&mInputContext, NULL, NULL, file);
        if (!mInputContext) {
            cerr << "avformat_alloc_output_context2 failed!" << endl;
            return false;
        }
        mFilename = file;
        return true;
    }
    /**
     *@brief
     *@return
     */
    bool addVideoStream() {
        if (!mInputContext) {
            return false;
        }
        // 1. 创建视频编码器
        AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (!codec) {
            cerr << "avcodec_find_encoder failed !" << endl;
            return false;
        }
        mVCodecCxt = avcodec_alloc_context3(codec);
        if (!mVCodecCxt) {
            cerr << "avcodec_alloc_context3 failed!" << endl;
            return false;
        }

        //压缩后的比特率
        mVCodecCxt->bit_rate = mBitrate;
        mVCodecCxt->width = mOutWidth;
        mVCodecCxt->height = mOutHeight;
        //时间基数  {分子，分母}
        mVCodecCxt->framerate = {mOutFps, 1};
        mVCodecCxt->time_base = {1, mOutFps};

        mVCodecCxt->gop_size = 50;

        mVCodecCxt->max_b_frames = 0;
        mVCodecCxt->pix_fmt = AV_PIX_FMT_YUV420P;
        mVCodecCxt->codec_id = AV_CODEC_ID_H264;

        //设置编码参数 preset 预设值 速度越慢 编码质量越高
        av_opt_set(mVCodecCxt->priv_data, "preset", "superfast", 0);
        // AVCodecContext::extradata 中放置全局标头，而不是放到每个关键帧里
        mVCodecCxt->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

        int ret = avcodec_open2(mVCodecCxt, codec, NULL);
        if (ret != 0) {
            cerr << "avcodec_open2 failed!" << endl;
            return false;
        }
        cout << "open codec success!" << endl;

        //添加视频流到输出上下文
        mVideoStream = avformat_new_stream(mInputContext, codec);
        mVideoStream->codecpar->codec_tag = 0;
        //将编码上下文内的相关信息拷贝到封装参数
        avcodec_parameters_from_context(mVideoStream->codecpar, mVCodecCxt);

        av_dump_format(mInputContext, 0, mFilename.c_str(), true);

        //像素(尺寸)转换上下文  rgb to yuv
        mInPixFmt = AV_PIX_FMT_BGRA;
        mVideoSwsContext = sws_getCachedContext(
            mVideoSwsContext, mInWidth, mInHeight, (AVPixelFormat)mInPixFmt,
            mOutWidth, mOutHeight, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL,
            NULL);
        if (!mVideoSwsContext) {
            cerr << "sws_getCachedContext failed!" << endl;
            return false;
        }

        if (!mYuvFrame) {
            mYuvFrame = av_frame_alloc();
            mYuvFrame->format = AV_PIX_FMT_YUV420P;
            mYuvFrame->width = mOutWidth;
            mYuvFrame->height = mOutHeight;
            mYuvFrame->pts = 0;
            int ret = av_frame_get_buffer(mYuvFrame, 32);
            if (ret != 0) {
                cerr << "av_frame_get_buffer failed!" << endl;
                return false;
            }
        }

        return true;
    }

    /**
     * @brief 将rgb转yuv和编码压缩在一个函数完成，也可以将用两个线程完成
     * @param rgb 传入原始的rgb数据
     * @return 返回编码后的包数据
     **/
    AVPacket* encodeVideo(const unsigned char* rgb) {
        AVPacket* packet = NULL;
        uint8_t* indata[AV_NUM_DATA_POINTERS] = {0};
        indata[0] = (uint8_t*)rgb;
        int inSize[AV_NUM_DATA_POINTERS] = {0};
        inSize[0] = mInWidth * 4;
        // rgb to yuv
        int height = sws_scale(mVideoSwsContext, indata, inSize, 0, mInHeight,
                               mYuvFrame->data, mYuvFrame->linesize);
        if (height < 0) {
            return NULL;
        }
        cout << height << "|";

        mYuvFrame->pts = mVideoPts;
        mVideoPts++;
        // encode
        int ret = avcodec_send_frame(mVCodecCxt, mYuvFrame);
        if (ret != 0) {
            return NULL;
        }
        packet = av_packet_alloc();
        ret = avcodec_receive_packet(mVCodecCxt, packet);
        if (ret != 0 || packet->size <= 0) {
            av_packet_free(&packet);
            return NULL;
        }

        //将解码器中的时基换算为视频流中的时基
        av_packet_rescale_ts(packet, mVCodecCxt->time_base,
                             mVideoStream->time_base);

        return packet;
    }

    /**
     *@brief 写入文件头信息
     *@return 写成功就返回true 失败返回false
     */
    virtual bool writeHead() {
        if (!mInputContext) return false;

        //打开IO
        int ret =
            avio_open(&mInputContext->pb, mFilename.c_str(), AVIO_FLAG_WRITE);
        if (ret != 0) {
            cerr << "avio_open failed!" << endl;
            return false;
        }
        cout << "write head success!" << endl;
        //写入封装头
        ret = avformat_write_header(mInputContext, NULL);
        if (ret != 0) {
            cerr << "avformat_write_header failed!" << endl;
            return false;
        }
        return true;
    }

    /**
     * @brief 写入帧信息
     * 在内部会调用av_interleaved_write_frame,会主动释放packet，即使写失败
     * @return 写成功就返回true 失败返回false
     */
    virtual bool writeFrame(AVPacket* pkt) {
        if (!mInputContext || !pkt || pkt->size <= 0) return false;

        // av_write_frame 内部不会释放packet
        int ret = av_interleaved_write_frame(mInputContext, pkt);
        if (ret != 0) {
            cerr << "av_interleaved_write_frame failed!" << endl;
            return false;
        }
        return true;
    }

    /**
     * @brief 写入文件尾部索引信息
     * @return 写成功就返回true 失败返回false
     */
    virtual bool writeEnd() {
        if (!mInputContext || !mInputContext->pb) return false;

        //写入尾部索引
        int ret = av_write_trailer(mInputContext);
        if (ret != 0) {
            cerr << "av_write_trailer failed!" << endl;
            return false;
        }
        // 关闭输入IO
        ret = avio_close(mInputContext->pb);
        if (ret != 0) {
            cerr << "avio_close failed!" << endl;
            return false;
        }
        cout << "WriteEnd success!" << endl;
        return true;
    }

    /**
     * @brief
     * @return
     */
    virtual bool addAudioStream() {
        if (!mInputContext) return false;
        // 1.找到音频编码
        AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
        if (!codec) {
            cerr << "avcodec_find_encoder failed!" << endl;
            return false;
        }

        // 2.创建并打开音频编码器
        mACodecCxt = avcodec_alloc_context3(codec);
        if (!mACodecCxt) {
            cerr << "avcodec_alloc_context3 failed!" << endl;
            return false;
        }

        mACodecCxt->bit_rate = mABiterate;
        mACodecCxt->sample_rate = mOutSampleRate;
        mACodecCxt->sample_fmt = (AVSampleFormat)mOutSampleFmt;
        mACodecCxt->channels = mOutChannel;
        mACodecCxt->channel_layout = av_get_default_channel_layout(mOutChannel);
        mACodecCxt->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

        int ret = avcodec_open2(mACodecCxt, codec, NULL);
        if (ret != 0) {
            avcodec_free_context(&mACodecCxt);
            cerr << "avcodec_open2 failed!" << endl;
            return false;
        }
        cout << "avcodec open aac success!" << endl;

        // 3. 新增音频流
        mAudioStream = avformat_new_stream(mInputContext, NULL);
        if (!mAudioStream) {
            cerr << "avformat_new_stream failed!" << endl;
            return false;
        }
        mAudioStream->codecpar->codec_tag = 0;
        avcodec_parameters_from_context(mAudioStream->codecpar, mACodecCxt);

        av_dump_format(mInputContext, 0, mFilename.c_str(), 1);

        //音频重采样上下文创建
        mAudioSwrCtx = swr_alloc_set_opts(
            mAudioSwrCtx, mACodecCxt->channel_layout, mACodecCxt->sample_fmt,
            mACodecCxt->sample_rate,                    //输出格式
            av_get_default_channel_layout(mInChannel),  //输入格式
            (AVSampleFormat)mInSampleFmt, mInSampleRate, NULL, NULL);

        ret = swr_init(mAudioSwrCtx);
        if (ret != 0) {
            cerr << "swr_init failed!" << endl;
            return false;
        }

        //重采样后输出的AVFrame
        if (!mPcmFrame) {
            mPcmFrame = av_frame_alloc();
            mPcmFrame->format = mACodecCxt->sample_fmt;
            mPcmFrame->channels = mACodecCxt->channels;
            mPcmFrame->channel_layout = mACodecCxt->channel_layout;
            mPcmFrame->nb_samples = ;  //一个通道的一帧音频的样本数量
        }

        return true;
    }

   private:
    AVFormatContext* mInputContext = NULL;  //封装mp4上下文
    AVCodecContext* mVCodecCxt = NULL;      //视频编码器上下文
    AVCodecContext* mACodecCxt = NULL;      //音频编码器上下文
    AVStream* mVideoStream = NULL;          //视频流
    AVStream* mAudioStream = NULL;          //音频流
    SwsContext* mVideoSwsContext = NULL;    //像素(尺寸)转换上下文
    SwrContext* mAudioSwrCtx = NULL;        //音频重采样上下文
    AVFrame* mYuvFrame = NULL;              //输出的YUV空间
    AVFrame* mPcmFrame = NULL;              //重采样后输出的PCM
    int mVideoPts = 0;                      //视频pts
};

/**
 * @brief
 * 工厂模式，所有的对象在cpp中创建，每次get去获取对象，若对象由用户生产，容易产生管理问题
 * 而且该类可能会被多个类同时访问。如果由用户创建，代码耦合度会很大
 * @param index the number of XVideoWrite object
 * @return the point to XVideoWriter
 */
XVideoWriter* XVideoWriter::get(unsigned short index) {
    static CXVideoWriter wrs[65535];
    return &wrs[index];
}

XVideoWriter::~XVideoWriter() {}