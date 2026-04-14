#ifndef _IMAGE_H__
#define _IMAGE_H__

typedef struct Pixel{
    unsigned char R,G,B,A;
}Pixel;

typedef struct Image{
    unsigned char *img;
    int width,height,channels;
    struct Pixel* PixelList;

    char* image_name;
}Image;

Image* init_Image(const char* name);

void loadImage(Image* image);
void drawPixel(struct Pixel p,int x,int y);
void setPixels(Image* image) ;
void printPixels(Image* image) ;
void Image_destroy(Image* image) ;

#endif
