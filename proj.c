// Project - Code Optimization
// System Programming, DGIST, Prof. Yeseong Kim
// 201811090 JUHO SONG
// YOU WILL TURN IN THIS FILE.
// Read the provided instruction carefully.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmplib.h"
#include "proj.h"

// This implementation is simply copied from "main.c".
// Your job is to modify and optimize it for better performance!

int symmetric = 0; //case of symmetric

static Pixel convolution(
        Pixel* input, int x, int y, int width, float* filter) {
    double r = 0;
    double g = 0;
    double b = 0;

    int start = (y-1)*width + x-1;
    
    float f1 = filter[0];
    float f2 = filter[1];
    float f3 = filter[4];

    r = input[start+1+width].r*f3 + (input[start].r + input[start+2].r + input[start+width*2].r + input[start+width*2+2].r) * f1 +
        (input[start+width*2+1].r + input[start+width+2].r + input[start+width].r + input[start+1].r) * f2;


    g = input[start+1+width].g*f3 + (input[start].g + input[start+2].g + input[start+width*2].g + input[start+width*2+2].g) * f1 +
        (input[start+width*2+1].g + input[start+width+2].g + input[start+width].g + input[start+1].g) * f2;

    b = input[start+1+width].b*f3 + (input[start].b + input[start+2].b + input[start+width*2].b + input[start+width*2+2].b) * f1 +
        (input[start+width*2+1].b + input[start+width+2].b + input[start+width].b + input[start+1].b) * f2;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    
    Pixel p;
    memset(&p, 0, sizeof(p));

    p.r = (unsigned char)r;
    p.g = (unsigned char)g;
    p.b = (unsigned char)b;

    return p;
}

static Pixel convolution_asym(
        Pixel* input, int x, int y, int width, float* filter) {
    double r = 0;
    double g = 0;
    double b = 0;

    int mid = (y-1)*width + x;
    int filter_start = 0;
    
    for (int dy = -1; dy < 2; ++dy) {

        float f1 = filter[filter_start];
        float f2 = filter[filter_start+1];
        float f3 = filter[filter_start+2];  

        Pixel p1 = input[mid-1];
        r += (p1.r * f1);
        g += (p1.g * f1);
        b += (p1.b * f1);

        Pixel p2 = input[mid];      
        r += (p2.r * f2);
        g += (p2.g * f2);
        b += (p2.b * f2);

        Pixel p3 = input[mid+1];
        r += (p3.r * f3);
        g += (p3.g * f3);
        b += (p3.b * f3);

        mid += width;
        filter_start += 3;
    }
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    
    Pixel p;
    memset(&p, 0, sizeof(p));

    p.r = (unsigned char)r;
    p.g = (unsigned char)g;
    p.b = (unsigned char)b;

    return p;
}

void filter_optimized(void* args[]) {
    unsigned int width = *(unsigned int*)args[0];
    unsigned int height = *(unsigned int*)args[1];
    unsigned int p_w = width+2; //padding width
    unsigned int p_h = height+2; //padding height
    Pixel* input = args[2];
    Pixel* output = args[3];

    float* filter = args[4];
    if (filter[0] == filter[2] && filter[2] == filter[6] && filter[6] == filter[8]) {
        if (filter[1] == filter[3] && filter[3] == filter[5] && filter[5] == filter[7]) {
            symmetric = 1;
        }
    }


    //zero padding 2*2 for every column and row
    Pixel* pad = (Pixel*)malloc(p_w*p_h*sizeof(Pixel));
    memset(pad, 0, p_w*p_h*sizeof(Pixel));    
    for (int j = 0; j < height; j++){
        for(int i =0; i < width; i++){
            pad[((j+1)*(p_w))+i+1] = input[j*width+i];
        }
    }

    int z = 0;
    if (symmetric) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; x += 32) {
                int y_cor = y+1;
                output[z] = convolution(pad, x+1, y_cor, p_w, filter);
                output[z+1] = convolution(pad, x+2, y_cor, p_w, filter);
                output[z+2] = convolution(pad, x+3, y_cor, p_w, filter);
                output[z+3] = convolution(pad, x+4, y_cor, p_w, filter);
                output[z+4] = convolution(pad, x+5, y_cor, p_w, filter);
                output[z+5] = convolution(pad, x+6, y_cor, p_w, filter);
                output[z+6] = convolution(pad, x+7, y_cor, p_w, filter);
                output[z+7] = convolution(pad, x+8, y_cor, p_w, filter);
                output[z+8] = convolution(pad, x+9, y_cor, p_w, filter);
                output[z+9] = convolution(pad, x+10, y_cor, p_w, filter);
                output[z+10] = convolution(pad, x+11, y_cor, p_w, filter);
                output[z+11] = convolution(pad, x+12, y_cor, p_w, filter);
                output[z+12] = convolution(pad, x+13, y_cor, p_w, filter);
                output[z+13] = convolution(pad, x+14, y_cor, p_w, filter);
                output[z+14] = convolution(pad, x+15, y_cor, p_w, filter);
                output[z+15] = convolution(pad, x+16, y_cor, p_w, filter);
                output[z+16] = convolution(pad, x+17, y_cor, p_w, filter);
                output[z+17] = convolution(pad, x+18, y_cor, p_w, filter);
                output[z+18] = convolution(pad, x+19, y_cor, p_w, filter);
                output[z+19] = convolution(pad, x+20, y_cor, p_w, filter);
                output[z+20] = convolution(pad, x+21, y_cor, p_w, filter);
                output[z+21] = convolution(pad, x+22, y_cor, p_w, filter);
                output[z+22] = convolution(pad, x+23, y_cor, p_w, filter);
                output[z+23] = convolution(pad, x+24, y_cor, p_w, filter);
                output[z+24] = convolution(pad, x+25, y_cor, p_w, filter);
                output[z+25] = convolution(pad, x+26, y_cor, p_w, filter);
                output[z+26] = convolution(pad, x+27, y_cor, p_w, filter);
                output[z+27] = convolution(pad, x+28, y_cor, p_w, filter);
                output[z+28] = convolution(pad, x+29, y_cor, p_w, filter);
                output[z+29] = convolution(pad, x+30, y_cor, p_w, filter);
                output[z+30] = convolution(pad, x+31, y_cor, p_w, filter);
                output[z+31] = convolution(pad, x+32, y_cor, p_w, filter);
                z +=32;
            }
        }
    }
    else {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; x += 32) {
                int y_cor = y+1;
                output[z] = convolution_asym(pad, x+1, y_cor, p_w, filter);
                output[z+1] = convolution_asym(pad, x+2, y_cor, p_w, filter);
                output[z+2] = convolution_asym(pad, x+3, y_cor, p_w, filter);
                output[z+3] = convolution_asym(pad, x+4, y_cor, p_w, filter);
                output[z+4] = convolution_asym(pad, x+5, y_cor, p_w, filter);
                output[z+5] = convolution_asym(pad, x+6, y_cor, p_w, filter);
                output[z+6] = convolution_asym(pad, x+7, y_cor, p_w, filter);
                output[z+7] = convolution_asym(pad, x+8, y_cor, p_w, filter);
                output[z+8] = convolution_asym(pad, x+9, y_cor, p_w, filter);
                output[z+9] = convolution_asym(pad, x+10, y_cor, p_w, filter);
                output[z+10] = convolution_asym(pad, x+11, y_cor, p_w, filter);
                output[z+11] = convolution_asym(pad, x+12, y_cor, p_w, filter);
                output[z+12] = convolution_asym(pad, x+13, y_cor, p_w, filter);
                output[z+13] = convolution_asym(pad, x+14, y_cor, p_w, filter);
                output[z+14] = convolution_asym(pad, x+15, y_cor, p_w, filter);
                output[z+15] = convolution_asym(pad, x+16, y_cor, p_w, filter);
                output[z+16] = convolution_asym(pad, x+17, y_cor, p_w, filter);
                output[z+17] = convolution_asym(pad, x+18, y_cor, p_w, filter);
                output[z+18] = convolution_asym(pad, x+19, y_cor, p_w, filter);
                output[z+19] = convolution_asym(pad, x+20, y_cor, p_w, filter);
                output[z+20] = convolution_asym(pad, x+21, y_cor, p_w, filter);
                output[z+21] = convolution_asym(pad, x+22, y_cor, p_w, filter);
                output[z+22] = convolution_asym(pad, x+23, y_cor, p_w, filter);
                output[z+23] = convolution_asym(pad, x+24, y_cor, p_w, filter);
                output[z+24] = convolution_asym(pad, x+25, y_cor, p_w, filter);
                output[z+25] = convolution_asym(pad, x+26, y_cor, p_w, filter);
                output[z+26] = convolution_asym(pad, x+27, y_cor, p_w, filter);
                output[z+27] = convolution_asym(pad, x+28, y_cor, p_w, filter);
                output[z+28] = convolution_asym(pad, x+29, y_cor, p_w, filter);
                output[z+29] = convolution_asym(pad, x+30, y_cor, p_w, filter);
                output[z+30] = convolution_asym(pad, x+31, y_cor, p_w, filter);
                output[z+31] = convolution_asym(pad, x+32, y_cor, p_w, filter);
                z +=32;
            }
        }
    }


    free(pad);    
}

