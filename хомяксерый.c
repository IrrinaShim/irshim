#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

unsigned char* colortogray(unsigned char* Image, int sizeV, int sizeH, int step) {
    int k = 0;
    unsigned char* grayImage = (unsigned char*)malloc(sizeV*sizeH*sizeof(unsigned char));
    if ( grayImage == NULL) {
            printf("Memory allocation error at color_to_gray(): %d, %d\n", sizeV, sizeH );
            return Image;
    }

    for (int i = 0; i < sizeV*sizeH*step; i = i + step) {
        grayImage[k] = 0.299*Image[i] + 0.587*Image[i + 1] + 0.114*Image[i + 2];
        k = k + 1;
    }
    return grayImage;

}
 
unsigned char*  graytobw( unsigned char* Image, int sizeV, int sizeH, int black, int white) {
    for (int i = 2; i < sizeH-1; i++) {
        for (int j = 2; j < sizeV-1; j++) {
            if (Image[sizeV*i+j] < black) Image[sizeV*i+j] = 0;
            if (Image[sizeV*i+j] > white) Image[sizeV*i+j] = 255;
        }
    }
    return Image;
} 

void dfs(int i, int j,  int k, int w, int h, int* c, unsigned char* Image){
    c[w*i+j] = k;
    if((i>=1) && (i<=h-1) && (j-2>=1) && (j-2<=w-1)){
        if((abs(Image[w*i+j]-Image[w*i+(j-2)]) <= 60)&&(c[w*i+(j-2)] == 0)){
            dfs(i, j-2, k, w, h, c, Image);
        }
    }
    if((i-2>=1) && (i-2<=h-1) && (j+1>=1) && (j+1<=w-1)){
        if((abs(Image[w*i+j]-Image[w*(i-2)+(j+1)])<=60)&&(c[w*(i-2)+(j+1)]==0)){
            dfs(i-2, j+1, k, w, h, c, Image);
        }
    }
    if((i+2>=1) && (i+2<=h-1) && (j+1>=1) && (j+1<=w-1)){
        if((abs(Image[w*i+j]-Image[w*(i+2)+(j+1)])<=60)&&(c[w*(i+2)+(j+1)]==0)){
            dfs(i+2, j+1, k, w, h, c, Image);
        }
    }
}

int main() {
    char* inputPath = "image.png";
    int iw, ih, n;

    unsigned char *idata = stbi_load(inputPath, &iw, &ih, &n, 0);
    if (idata == NULL) {
        printf("ERROR: can't read file %s\n", inputPath );
        return 1;
    }
        
    int i, j, k=0;
    unsigned char* odata = (unsigned char*)malloc(ih*iw*n*sizeof(unsigned char));
    unsigned char* newImage = (unsigned char*)malloc(ih*iw*sizeof(unsigned char));
    if ( (odata == NULL) || (newImage == NULL) ) {
            printf("Memory allocation error at main()" );
            return 1;
        }

    newImage = colortogray(idata, iw, ih, n);   
    
    

    int black = 100;
    int white = 160;
    newImage = graytobw(newImage, iw, ih, black, white);

    int* col = (int*) malloc(iw*ih*sizeof(int));
    for(i=0;i<iw*ih;i++){
        col[i] = 0;
    }
    for (i=1;i<=ih-1;i++){
        for (j=1;j<=iw-1;j++){
            if(col[iw*i+j]==0){
                k=k+5;;
                dfs(i, j, k, iw, ih, col, newImage);
            }
        }
    }

    
    for (i = 0; i < iw*ih; i++) {
        odata[i*n] = 10+col[i]+0.5*col[i-1];
        odata[i*n+1] = 46+col[i];
        odata[i*n+2] = 100+col[i];
        if (n == 4) odata[i*n+3] = 255;
    }
    
    
    char* outputPath = "myhampster.png";
    stbi_write_png(outputPath, iw, ih, n, odata, 0);
    
    free(newImage);
	free(odata);
    stbi_image_free(idata);
    
    return 0;
}
