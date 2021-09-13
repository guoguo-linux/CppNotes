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
    AVFormatContext* mInputContetx = NULL;  //封装mp4上下文
    AVFormatContext* mOutputContetx = NULL;
    void close() {
        if (mInputContetx) {
            avformat_close_input(&mInputContetx);
        }
    }
    bool init(const char* file) {
        close();

        //初始化封装文件上下文
        avformat_alloc_output_context2(&mInputContetx, NULL, NULL, file);
        if (!mInputContetx) {
            cerr << "avformat_alloc_output_context2 failed!" << endl;
            return false;
        }
        return true;
    }

    bool AddVideoStream() {
        if (!mInputContetx) {
            return false;
        }
        // 1. 创建视频编码器
        AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (!codec) {
            cerr << "avcodec_find_encoder failed !" << endl;
            return false;
        }
        return true;
    }
};

/**
 * @brief:
 * 工厂模式，所有的对象在cpp中创建，每次get去获取对象，若对象由用户生产，容易产生管理问题
 * 而且该类可能会被多个类同时访问。如果由用户创建，代码耦合度会很大
 * @param: index
 * @return: XVideoWriter*
 */

XVideoWriter* XVideoWriter::get(unsigned short index) {
    static CXVideoWriter wrs[65535];
    return &wrs[index];
}

XVideoWriter::~XVideoWriter() {}