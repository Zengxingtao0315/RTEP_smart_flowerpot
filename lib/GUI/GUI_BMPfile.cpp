/*****************************************************************************
* | File      	:   GUI_BMPfile.h
* | Author      :   Xingtao Zeng
* | Function    :   Hardware underlying interface
* | Info        :   
*****************************************************************************/
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
extern "C" {
	#include "Debug.h"
}

#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <cstdlib>	//exit()
#include <cstring> //memset()
#include <cmath> //memset()
#include <cstdio>
#include <iostream>


 void BMP::open(const char *path){
	if((fp = fopen(path, "rb")) == NULL) {
        Debug("Cann't open the file!\n");
        exit(0);
    }
	fseek(fp, 0, SEEK_SET);
    fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 14
    fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 50
    std::cout << "pixel = " << bmpInfoHeader.biWidth << " * " << bmpInfoHeader.biHeight << std::endl;

    UWORD Image_Width_Byte = bmpInfoHeader.biWidth * 2;
    UWORD Bmp_Width_Byte = bmpInfoHeader.biWidth * 2;
    UBYTE Image[Image_Width_Byte * bmpInfoHeader.biHeight];
    memset(Image, 0xFF, Image_Width_Byte * bmpInfoHeader.biHeight);
 }
 
 bool BMP::is_65K_color()
{
    int readbyte = bmpInfoHeader.biBitCount;
    std::cout << "biBitCount = " << readbyte << std::endl;
    if (readbyte != 16) {
        Debug("Bmp image is not a 65K-color bitmap!\n");
        return false;
    }
    return true;
}

void BMP::readImage()
{
	UWORD x, y;
    UBYTE Rdata;
    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);
    
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {//Total display column
        for(x = 0; x < Bmp_Width_Byte; x++) {//Show a line in the line
            if(fread((char *)&Rdata, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
            Image[x + (bmpInfoHeader.biHeight-1 - y)*Image_Width_Byte] =  Rdata;
        }
    }
    fclose(fp);
}

void BMP::refresh(UWORD Xstart, UWORD Ystart) {
	UWORD color;
	
    std::cout << "bmpInfoHeader.biWidth = " << bmpInfoHeader.biWidth << std::endl;
	std::cout << "bmpInfoHeader.biHeight = " << bmpInfoHeader.biHeight << std::endl;
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {
        for(x = 0; x < bmpInfoHeader.biWidth; x++) {
            if(x > paint.Width || y > paint.Height) {
                break;
            }
			color = 0;
            color |= Image[x*2 + y*bmpInfoHeader.biWidth*2];
			color |= Image[x*2 + y*bmpInfoHeader.biWidth*2 + 1] << 8;
            Paint.SetPixel(Xstart + x, Ystart + y, color);
        }
    }
    return 0;
	
}
 
