#define _DEFAULT_SOURCE  // Add this first!
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "image.h"

//This used stb_image.g (What is it?)

Image* init_Image(const char* name){
    Image *new_image = malloc(sizeof(Image));
    new_image->image_name = strdup(name);
    new_image->img = NULL;
    new_image->PixelList = NULL;
    return new_image;
}

void drawPixel(struct Pixel p,int x,int y){
    // Terminals don't do transparency. We calculate the visual result manually.
    // Formula: Output = (Foreground * alpha) + (Background * (1 - alpha))
    int a = p.A;
    
    float alpha = a / 255.0f;
    
    int bg_r = 0, bg_g = 0, bg_b = 0; // Assuming black background

    unsigned char r = p.R;
    unsigned char b = p.B;
    unsigned char g = p.G;
    
    int final_r = (int)(r * alpha + bg_r * (1.0f - alpha));
    int final_g = (int)(g * alpha + bg_g * (1.0f - alpha));
    int final_b = (int)(b * alpha + bg_b * (1.0f - alpha));

    printf("\033[%d;%dH\033[48;2;%d;%d;%dm  \033[0m", y, x * 2, final_r, final_g, final_b);
}

void loadImage(Image* image){

    printf("Image name: %s\n",image->image_name);
    
    image->img = stbi_load(image->image_name, &(image->width), &(image->height), &(image->channels), 4);

    if (image->img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", image->width, image->height, image->channels);
}

void setPixels(Image* image) {
    int total_pixels = image->width * image->height;
    
    image->PixelList = malloc(sizeof(Pixel) * total_pixels);
        
    for(int i = 0; i < total_pixels; i++) {
        image->PixelList[i].R = image->img[i * 4 + 0];
        image->PixelList[i].G = image->img[i * 4 + 1];
        image->PixelList[i].B = image->img[i * 4 + 2];
        image->PixelList[i].A = image->img[i * 4 + 3];
    }
}

void printPixels(Image* image) {
    // Just printing the first 50 pixels so we don't flood the terminal
    int limit = (image->width * image->height > 50) ? 50 : (image->width * image->height);
    
    for(int i = 0; i < limit; i++) {
        Pixel curr = image->PixelList[i];
        int x = i % image->width;
        int y = i / image->width;
        printf("Pixel(%d,%d): R=%u G=%u B=%u A=%u\n", x, y, curr.R, curr.G, curr.B, curr.A);
    }
}

void Image_destroy(Image* image) {
    if (image->image_name) free(image->image_name);
    if (image->img) stbi_image_free(image->img);
    if (image->PixelList) free(image->PixelList); // Free the whole block at once
    free(image);
}
