#ifndef __XVIDEOWRITER_H
#define __XVIDEOWRITER_H

#include <string>

class XVideoWriter {
   public:
    virtual bool init(const char* file) = 0;
    virtual void close() = 0;
    virtual bool AddVideoStream();
    ~XVideoWriter();
    static XVideoWriter* get(unsigned short index = 0);  // create object

    std::string filename;

   protected:
    XVideoWriter();
};

#endif