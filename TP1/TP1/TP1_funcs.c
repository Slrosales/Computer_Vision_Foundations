#include "TP1_funcs.h"
#include "../Utils/Util.h"
#include <stdio.h>
#include <stdlib.h>


// TODO: split the image into three channels
void split_channels(ppm_file image,
                    ppm_file *image_r, ppm_file *image_g, ppm_file *image_b) {

    // Define ppm_file for red channel
    image_r -> rows = image.rows;
    image_r -> cols = image.cols;
    image_r -> magic_number = image.magic_number;
    image_r -> maxval = image.maxval;
    
    // Define ppm_file for green channel
    image_g -> rows = image.rows;
    image_g -> cols = image.cols;
    image_g -> magic_number = image.magic_number;
    image_g -> maxval = image.maxval;
    
    
    // Define ppm_file for blue channel
    image_b -> rows = image.rows;
    image_b -> cols = image.cols;
    image_b -> magic_number = image.magic_number;
    image_b -> maxval = image.maxval;
    
    image_r -> pixmap = (pixel *) malloc(image.cols * image.rows * sizeof(pixel));
    image_g -> pixmap = (pixel *) malloc(image.cols * image.rows * sizeof(pixel));
    image_b -> pixmap = (pixel *) malloc(image.cols * image.rows * sizeof(pixel));

    // image_r -> pixmap[0].red = image.pixmap[0].red;
    

    for (size_t i = 0; i < image.rows; i++){
        for (size_t j = 0; j < image.cols; j++){
            // image_r -> pixmap[i * image.cols + j].red = image.pixmap[i * image.cols + j].red;
            // image_r -> pixmap[i * image.cols + j].green = image.pixmap[i * image.cols + j].green;
            // image_r -> pixmap[i * image.cols + j].blue = image.pixmap[i * image.cols + j].blue;


            image_r -> pixmap[i * image.cols + j].red = image.pixmap[i * image.cols + j].red;
            image_r -> pixmap[i * image.cols + j].green = 0;
            image_r -> pixmap[i * image.cols + j].blue = 0;
            
            image_g -> pixmap[i * image.cols + j].red = 0;
            image_g -> pixmap[i * image.cols + j].green = image.pixmap[i * image.cols + j].green;
            image_g -> pixmap[i * image.cols + j].blue = 0;
            
            image_b -> pixmap[i * image.cols + j].red = 0;
            image_b -> pixmap[i * image.cols + j].green = 0;
            image_b -> pixmap[i * image.cols + j].blue = image.pixmap[i * image.cols + j].blue;
        }
    }
    

}

// TODO: convert rgb to grayscale
void rgb_to_gray(ppm_file image, pgm_file *image_gray, float brightness){
    
    // Define values for gray scale image
    image_gray -> rows = image.rows;
    image_gray -> cols = image.cols;
    image_gray -> magic_number = image.magic_number;
    image_gray -> maxval = image.maxval;
    
    int conversion = 0;
    image_gray -> graymap = (gray *) malloc(image.cols * image.rows * sizeof(gray));

    for (size_t i = 0; i < image.rows; i++){
        for (size_t j = 0; j < image.cols; j++){
            conversion = 0.299 * image.pixmap[i * image.cols + j].red + 0.587 * image.pixmap[i * image.cols + j].green + 0.114 * image.pixmap[i * image.cols + j].blue;
            image_gray -> graymap[i * image.cols + j] = (float)conversion * brightness;
        }
    }
}

void ppm_to_pam(ppm_file image, pam_file *image_pam, pgm_file *image_gray) {
    int i, j;

    // Metadata
    image_pam->magic_number = '7';
    image_pam->rows = image.rows;
    image_pam->cols = image.cols;
    image_pam->depth = 4;   // RGB + Alpha
    image_pam->maxval = image.maxval;
    image_pam->tuple_type = "RGB_ALPHA";

    // Memory allocation
    image_pam->map = (char *) malloc(image.cols * image.rows * image_pam->depth * sizeof(char));
    if (!image_pam->map) {
        pm_error("Memory allocation failed");
    }

    // Copy data
    for (i = 0; i < image.rows; i++) {
        for (j = 0; j < image.cols; j++) {
            image_pam->map[(i * image.cols + j) * image_pam->depth + 0] = image.pixmap[i * image.cols + j].red;
            image_pam->map[(i * image.cols + j) * image_pam->depth + 1] = image.pixmap[i * image.cols + j].green;
            image_pam->map[(i * image.cols + j) * image_pam->depth + 2] = image.pixmap[i * image.cols + j].blue;
            if (image_gray != NULL)
                image_pam->map[(i * image.cols + j) * image_pam->depth + 3] = image_gray->graymap[i * image.cols + j]; // Alpha channel from PGM
            else {
                image_pam->map[(i * image.cols + j) * image_pam->depth + 3] = 255; // Alpha channel
            }
        }
    }
}