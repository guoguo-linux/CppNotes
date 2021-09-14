#include <stdio.h>

#include <iostream>

#include "XVideoWriter.h"

using namespace std;

int main() {
    char outFile[] = "rgbpcm.mp4";
    char rgbFile[] = "test.rgb";
    char pcmFile[] = "test.pcm";
    XVideoWriter *xw = XVideoWriter::get(0);
    cout << xw->init(outFile);
    cout << xw->addVideoStream();
    xw->addAudioStream();

    FILE *fp = fopen(rgbFile, "rb");
    if (!fp) {
        cout << "open" << rgbFile << "failed!" << endl;
        getchar();  // for debug
        return -1;
    }
    int size = xw->mInHeight * xw->mInWidth * 4;
    unsigned char *rgb = new unsigned char[size];

    //写入头信息
    xw->writeHead();

    for (;;) {
        int len = fread(rgb, 1, size, fp);
        if (len < 0) {
            break;
        }

        //传入rgb转为yuv，并编码返回packet
        AVPacket *pkt = xw->encodeVideo(rgb);
        if (pkt) {
            cout << ".";
        } else {
            cout << "-";
            continue;
        }
        //取出packet后写入(封装)
        if (xw->writeFrame(pkt)) {
            cout << "+";
        }
    }
    xw->writeEnd();
    delete rgb;
    rgb = NULL;
    cout
        << "\n----------------------------- end -------------------------------"
        << endl;
    // rgb to yuv

    getchar();
    return 0;
}