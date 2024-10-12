#include <cstdint>
#include <vector>

#define NO_RLE_BMP 0
#define RLE_BMP 1
#define FORMAT_FILE_BMP 19778
#define AVABLE_FORMAT 40
#define END_RLE_FILE 128

#pragma pack(push, 1)
typedef struct{
    uint8_t BLUE;
    uint8_t GREAN;
    uint8_t RED;
} BITMAPRGB;

#pragma pack(push, 1)
typedef struct {
    uint16_t   bfType;
    uint32_t   bfSize;
    uint32_t   bfReserved;
    uint32_t   bfOffBits;
} bagBITMAPFILEHEADER;

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
} bagBITMAPINFOHEADER;

typedef struct
{
    bagBITMAPFILEHEADER   bmFile;
    bagBITMAPINFOHEADER   bmInfo;
    std::vector<BITMAPRGB>  bmPixels;
} BITMAPIMAGE;




int load_file(char* path, BITMAPIMAGE* image);

uint8_t* bag_convert(std::vector<BITMAPRGB> image);

void save_toRLE(char* path ,BITMAPIMAGE image);

char* get_err_file(int err_code);