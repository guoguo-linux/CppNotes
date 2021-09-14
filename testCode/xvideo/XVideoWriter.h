#ifndef __XVIDEOWRITER_H
#define __XVIDEOWRITER_H

#include <string>

//避免在头文件引入ffmpeg头文件
//如果只是返回类的指针，并不需要该类的所有声明,只声明类名即可
class AVPacket;

enum XSAMPLEFMT {
    X_S16 = 1,
    X_FLATP = 8,
};

class XVideoWriter {
   public:
    // video input param
    int mInWidth = 848;
    int mInHeight = 480;
    int mInPixFmt = 30;  // AV_PIX_FMT_BGRA

    // video output param
    int mBitrate = 4000000;
    int mOutWidth = 848;
    int mOutHeight = 480;
    int mOutFps = 25;

    // audio output param
    int mABiterate = 64000;
    int mOutChannel = 2;
    int mOutSampleRate = 44100;
    XSAMPLEFMT mOutSampleFmt = X_FLATP;

    // audio input param
    int mInSampleRate = 44100;
    int mInChannel = 2;
    XSAMPLEFMT mInSampleFmt = X_S16;

   public:
    //纯虚函数在基类中不实现，在其派生类中实现
    virtual bool init(const char* file) = 0;
    virtual void close() = 0;
    virtual bool addVideoStream() = 0;
    virtual bool addAudioStream() = 0;
    virtual AVPacket* encodeVideo(const unsigned char* rgb) = 0;

    virtual bool writeHead() = 0;
    virtual bool writeFrame(AVPacket* pkt) = 0;
    virtual bool writeEnd() = 0;
    ~XVideoWriter();
    static XVideoWriter* get(unsigned short index = 0);  // create object

    std::string mFilename;

   protected:
    XVideoWriter();
};

#endif