#include <stdio.h>

#include <iostream>

#include "XVideoWriter.h"

using namespace std;

int main() {
    char outFile[] = "rgbpcm.mp4";
    XVideoWriter *xw = XVideoWriter::get(0);
    cout << xw->init(outFile);
    getchar();
    return 0;
}