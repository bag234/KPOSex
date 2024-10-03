#include "stdio.h"
#include <fstream>
#include <iostream>
#include <cstdint>
#include "bmpfunc.h"
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
    std::fstream fh;
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER info;

    fh.open("./test.bmp", std::fstream::in | std::fstream::binary);
    fh.read((char*)&header, sizeof(BITMAPFILEHEADER));
    fh.read((char*)&info, sizeof(BITMAPINFOHEADER));

    printf("Type: %x\n", header.bfType);
    printf("Size: %x\n", header.bfSize);
    printf("Reversed : %x\n", header.bfReserved);
    printf("offbits: %x\n", header.bfOffBits);

    printf("Size: %d\n", info.biSize);
    printf("Width: %d\n", info.biWidth);
    printf("Heigh: %d\n", info.biHeight);
    printf("Planes: %x\n", info.biPlanes);
    printf("BIT Count: %x\n", info.biBitCount);
    printf("Compresion: %x\n", info.biCompression);
    printf("Size Image: %d\n", info.biSizeImage);
    printf("X PEls: %x\n", info.biXPelsPerMeter);
    printf("Y Pels: %x\n", info.biYPelsPerMeter);
    printf("CLR Used: %x\n", info.biClrUsed);
    printf("CLR Imp: %x\n", info.biClrImportant);

    vector<BITMAPRGB> pixels(info.biHeight * info.biWidth);

    BITMAPRGB rgb_pixel;
    for(int i = 0; i < info.biHeight * info.biWidth; i++){
        fh.read((char*)&rgb_pixel, sizeof(BITMAPRGB));
        pixels.push_back(rgb_pixel);
    }

    return 0;
}
