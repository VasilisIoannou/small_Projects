#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include <unistd.h>

#include "image.h"
#include "screen.h"
#include "fileImage.h"

int main(int argc,char* argv[]){
    if(argc < 2){
        printf("Please insert an image\n");
        return 1;
    }

    const char* image_name = argv[1];

    /*
    Image* img = init_Image(image_name);
    loadImage(img);
    setPixels(img);

    clearScreen();
    drawImage(img);

    Image_destroy(img);
    */

    ImageBundle* ib = init_ImageBundle();    

    setFileNames(ib,image_name);
    setImages(ib);

    //printFileNames(ib);

    // Set screen
    clearScreen();

    //Draw Pixels
    int i=0;
    while(1){
        drawImageAt(ib,i);
        sleep_ms(1000);
        clearScreen();
        if(i==8){
            i=0;
        } else {
            i++;
        }
    }
    
}
