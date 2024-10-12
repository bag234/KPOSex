#include "stdio.h"
#include <fstream>
#include <iostream>
#include <cstdint>
#include "bmpfunc.h"
#include <vector>

using namespace std;

int calc_leveling(int width){
    int  leveling = 8 - ((width * 3) % 8);
    if(leveling > 2)
        return 0;
    return leveling;
}

bool eq_BRG(BITMAPRGB BGR1, BITMAPRGB BGR2){
    if (BGR1.BLUE != BGR2.BLUE)
        return false;
    if (BGR1.RED != BGR2.RED)
        return false;
    if (BGR1.GREAN != BGR2.GREAN)
        return false;
    return true;
}

int is_Equlas_Images(BITMAPIMAGE img1, BITMAPIMAGE img2){
    if(img1.bmInfo.biHeight != img2.bmInfo.biHeight || img1.bmInfo.biWidth != img2.bmInfo.biWidth)
        return -1;  // images not equlsa size <-- rewrite
    
    int seq = 0; 

    for(int i = 0; i < img1.bmPixels.size(); i++){
        if(!eq_BRG(img1.bmPixels[i], img2.bmPixels[i]))
            seq++;
    }

    return seq;
}

vector<BITMAPRGB> load_rle_file(fstream& fh){
    vector<BITMAPRGB> pixeles; 
    uint8_t sv_byte;
    BITMAPRGB rgb_pixel;

    printf("[IS RLE FILE]\n");

    while(!fh.eof()){
        fh.read((char*) &sv_byte, sizeof(uint8_t));
        // printf("[SB]: %d\n", sv_byte);
        
        if(sv_byte == END_RLE_FILE){
            break;
        }

        if (sv_byte > 127){
            fh.read((char*)&rgb_pixel, sizeof(BITMAPRGB));
            // printf("[BYTE RLE]: (%x, %x, %x)\n", rgb_pixel.BLUE, rgb_pixel.GREAN, rgb_pixel.RED);
            for (int i = 0; i < (sv_byte - 128); i++)
                pixeles.push_back(rgb_pixel);
        }
        else {
            // printf("[BYTE NO %d] ", sv_byte);
            for (int i = 0; i < (sv_byte); i++){
                fh.read((char*)&rgb_pixel, sizeof(BITMAPRGB));
                // printf("(%x, %x, %x), ", rgb_pixel.BLUE, rgb_pixel.GREAN, rgb_pixel.RED);
                pixeles.push_back(rgb_pixel);
            }
            // printf("[END NO]\n");
        }
    }

    printf("[NO RLE FILE]\n");

    return pixeles;
}

int load_file(char* path, BITMAPIMAGE* image){
    fstream fh;
    bagBITMAPFILEHEADER header;
    bagBITMAPINFOHEADER info;
    uint64_t size = 0;

    fh.open(path, fstream::in | fstream::binary);

    if(!fh.is_open()){
        return 1;
    }

    fh.read((char*)&header, sizeof(bagBITMAPFILEHEADER));
    fh.read((char*)&info, sizeof(bagBITMAPINFOHEADER));

    if(header.bfType != FORMAT_FILE_BMP)
        return 10;
    if(info.biSize != AVABLE_FORMAT)
        return 20;

    size = info.biHeight * info.biWidth;

    vector<BITMAPRGB> pixels(size);
    BITMAPRGB rgb_pixel;

    if(info.biCompression != RLE_BMP && info.biCompression != NO_RLE_BMP)
        return 30;

    if(info.biCompression == RLE_BMP){
        pixels = load_rle_file(fh);
    }
    else for(int i = 0, w_i = 0; i < size; i++, w_i++){
        if (w_i == info.biWidth){
            w_i = 0;
            fh.seekg(calc_leveling(info.biWidth), ios::cur);
        }
        fh.read((char*)&rgb_pixel, sizeof(BITMAPRGB));
        pixels[i] = rgb_pixel;
    }

    fh.close();

    image->bmFile = header;
    image->bmInfo = info;
    image->bmPixels = pixels;

    return 0;
}

uint8_t* bag_convert(vector<BITMAPRGB> image){
    int size = image.size();
    uint8_t*  c_im = new uint8_t[(size * 3)];
    for(int i = 0, m_i = 3; i < size; i++, m_i+=3){
        c_im[m_i - 3] =  image[i].BLUE;
        c_im[m_i - 2] =  image[i].GREAN;
        c_im[m_i - 1] =  image[i].RED;
    }
    return c_im;
}

void save_RLEPixel(fstream& fh, bool isRLE, vector<BITMAPRGB> no_rle, BITMAPRGB pixel, uint8_t count){
    if (isRLE){
        count = 128 + count + 1;
        fh.write((char*) &count, sizeof(uint8_t));
        fh.write((char*) &pixel, sizeof(BITMAPRGB));
    }
    else{
        count = no_rle.size();
        fh.write((char*) &count, sizeof(uint8_t));
        for (int j = 0; j < no_rle.size(); j++){
            fh.write((char*) &no_rle[j], sizeof(BITMAPRGB));
        }
    }
}

void save_toRLE(char* path ,BITMAPIMAGE image){ //rewrite to save file and sequentially save file bytes 
    fstream fh;
    image.bmInfo.biCompression = RLE_BMP;

    fh.open(path, fstream::out | fstream::binary);
    fh.write((char*) &(image.bmFile), sizeof(bagBITMAPFILEHEADER) );
    fh.write((char*) &(image.bmInfo), sizeof(bagBITMAPINFOHEADER) );

    BITMAPRGB save = image.bmPixels[0];
    uint8_t count = 0;
    bool rle = eq_BRG(save, image.bmPixels[1]);
    vector<BITMAPRGB> no_rle; 
    
    for (int i = 1; i < image.bmPixels.size(); i++){
        if(eq_BRG(save, image.bmPixels[i])){
            if(!rle){
                save_RLEPixel(fh, rle, no_rle, save, count);
                count = 1; 
                no_rle.clear();
            }
            else
                count++;
        }
        else{
            if(rle){
                save_RLEPixel(fh, rle, no_rle, save, count);
                count = 0; 
                no_rle.clear();
            }
            else{
                count++;
                no_rle.push_back(save);
            }
                
        }

        if(count > 126){
            save_RLEPixel(fh, rle, no_rle, save, count);
            count = 0; 
            no_rle.clear();
        }

        rle = eq_BRG(save, image.bmPixels[i]);
        save = image.bmPixels[i];
    }
    save_RLEPixel(fh, rle, no_rle, save, count);
    printf("[END RLE SAVE FILE]");
    count = END_RLE_FILE;
    fh.write((char*) &count, sizeof(uint8_t)); // end byte
    fh.flush();
    fh.close();
}

char* get_err_file(int err_code){
    switch (err_code){
        case 0:
            return "[FILE OPEN OK]";
        case 1:
            return "[FILE NOT FIND]";
        case 10:
            return "[FILE NOT BMP]";
        case 20:
            return "[FILE NOT Support VERSION]";
        case 30:
            return "[ERROR RLE FORMAT]";
        
        default:
            return "[SOME ERROR]";
    }
}

void print_pixel(vector<BITMAPRGB>  bmPixels){
    for(int i = 0; i < bmPixels.size(); i++){
        printf("RGB %d: (%x, %x, %x)\n", i, bmPixels[i].RED, bmPixels[i].GREAN, bmPixels[i].BLUE);
    }
}

void print_info(bagBITMAPFILEHEADER header, bagBITMAPINFOHEADER info){
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
}

int main(int argc, char const *argv[])
{
    BITMAPIMAGE image, image2; 
    char* path = "./f.bmp";
    char* path2 = "./f.rle";

    load_file(path, &image);
    load_file(path2, &image2);

    printf("{IMAGE SEQUNTSE} %d\n", is_Equlas_Images(image, image2));
  
    // save_toRLE(path2, image);
    
    return 0;
}
