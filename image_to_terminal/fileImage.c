
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>

#include <unistd.h>

#include "image.h"
#include "screen.h"
#include "fileImage.h"

ImageFile* init_ImageFile(){
    ImageFile* new_imagefile = malloc(sizeof(ImageFile));
    new_imagefile->file_name = NULL;
    new_imagefile->image = NULL;
    new_imagefile->image_number = -1;
    return new_imagefile;
}

void drawImage(Image* image){
    int i=0;

    const int const_x = 0;
    const int const_y = 20;

    for(int x=0;x<image->width;x++){
        for(int y=0;y<image->height;y++){
            drawPixel(image->PixelList[i],y+const_y,x+const_x);
            i++;
        }
    }
    printf("\n\n");
}

int find_number(const char* str) {
    if (str == NULL) return -1; // Safety check

    while (*str != '\0' && !isdigit(*str)) {
        str++;
    }

    if (*str == '\0') {
        return -1; 
    }

    return atoi(str);
}

void setFileNames(ImageBundle* ib ,const char *dir_path){
    struct dirent *entry;
    DIR *dp = opendir(dir_path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    int set_number;

    while ((entry = readdir(dp))) {
        // Skip the '.' (current) and '..' (parent) directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        //The picture must have a number in its name!
        set_number = find_number(entry->d_name);
    
        //Set the file names
        char path_buffer[1024]; 

        snprintf(path_buffer, sizeof(path_buffer), "%s%s", dir_path, entry->d_name);

        ImageFile* new_imgfile = init_ImageFile();
        new_imgfile->file_name = strdup(path_buffer);
        new_imgfile->image = NULL; //Null for now
        new_imgfile->image_number = set_number;
       
        addImageFileNode(ib,new_imgfile);
    }

    closedir(dp);

}


void setImages(ImageBundle* ib){
    if (ib == NULL || ib->head == NULL) return;
    ImageFileNode* current = ib->head;
    while (current != NULL) { 
        if (current->imgfile != NULL) {
            current->imgfile->image = init_Image(current->imgfile->file_name);
            
            loadImage(current->imgfile->image);
            setPixels(current->imgfile->image);
        }
        current = current->next;
    }
}

void free_ImageBundle(ImageBundle* ib) {
    
    if(ib == NULL) return;

    ImageFileNode* current = ib->head;
    ImageFileNode* nextNode;

    while (current != NULL) {
        nextNode = current->next;

        if (current->imgfile != NULL) {
            free(current->imgfile->file_name); 
            free(current->imgfile->image); 
            free(current->imgfile);
        }

        free(current);
        current = nextNode;
    }

    ib->head = NULL;
    free(ib);
}

ImageBundle* init_ImageBundle(){
    ImageBundle* new_ib = malloc(sizeof(ImageBundle));
    new_ib->head = NULL;
    return new_ib;
}


void addImageFileNode(ImageBundle* ib,ImageFile* new_imgfile){
    if (ib == NULL ) return;

    //Create new node
    ImageFileNode* newNode = (ImageFileNode*)malloc(sizeof(ImageFileNode));
    if (newNode == NULL) return;

    newNode->imgfile = new_imgfile;
    newNode->next = NULL;

    if (ib->head == NULL) {
        ib->head = newNode; // Update the actual head pointer
        return;
    }

    ImageFileNode* last = ib->head;
    while (last->next != NULL) {
        last = last->next;
    }

    last->next = newNode;
}

void drawImageAt(ImageBundle* ib,int n){
    ImageFileNode* current = ib->head;

    while(current != NULL){
        if(current->imgfile->image_number == n){
            drawImage(current->imgfile->image);
        }
        current = current->next;
    }
}

void printFileNames(ImageBundle* ib) {
    if (ib == NULL) {
        printf("in printfilenames NULL\n");
        return;
    }

    ImageFileNode* curr = ib->head;

    while (curr != NULL) {
        if (curr->imgfile != NULL && curr->imgfile->file_name != NULL) {
            printf("%s\n", curr->imgfile->file_name);
        } else {
            printf("[Warning: Node found with missing image data]\n");
        }

        curr = curr->next;
    }
}
