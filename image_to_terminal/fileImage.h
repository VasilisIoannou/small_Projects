#ifndef _FILEIMAGE_H__
#define _FILEIMAGE_H__

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include <unistd.h>

#include "image.h"
#include "screen.h"

typedef struct ImageFile{
    char* file_name;
    int image_number;
    Image* image;
}ImageFile;

typedef struct ImageFileNode{
    struct ImageFile* imgfile;
    struct ImageFileNode* next;
}ImageFileNode;

typedef struct ImageBundle{
    //Store the head
    ImageFileNode* head;
    //Has room to add feratures later
}ImageBundle;

ImageFile* init_ImageFile();
void setFileNames(ImageBundle* ib ,const char *dir_path);
void setImages(ImageBundle* ib);
void free_ImageBundle(ImageBundle* ib);
void drawImage(Image* image);
ImageBundle* init_ImageBundle();
void addImageFileNode(ImageBundle* ib,ImageFile* new);
void drawImageAt(ImageBundle* ib,int n);
void printFileNames(ImageBundle* ib);

#endif
