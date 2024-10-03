#include <cstdint>
typedef int FXPT2DOT30;
 
typedef struct {
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
} CIEXYZ;
 
typedef struct {
    CIEXYZ  ciexyzRed; 
    CIEXYZ  ciexyzGreen; 
    CIEXYZ  ciexyzBlue; 
} CIEXYZTRIPLE;

#pragma pack(push, 1)
typedef struct{
    uint8_t blue;
    uint8_t GREAN;
    uint8_t RED;
} BITMAPRGB;

#pragma pack(push, 1)
typedef struct {
    uint16_t   bfType;
    uint32_t   bfSize;
    uint32_t   bfReserved;
    uint32_t   bfOffBits;
} BITMAPFILEHEADER;

#pragma pack(push, 1)
typedef struct {
    uint32_t    biSize;
    uint32_t    biWidth;
    uint32_t    biHeight;
    uint16_t    biPlanes;
    uint16_t    biBitCount;
    uint32_t    biCompression;
    uint32_t    biSizeImage;
    uint32_t    biXPelsPerMeter;
    uint32_t    biYPelsPerMeter;
    uint32_t    biClrUsed;
    uint32_t    biClrImportant;
} BITMAPINFOHEADER;