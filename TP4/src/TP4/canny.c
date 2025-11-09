#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include "canny.h"


// TODO: perform the Non-maximum suppression of the magnitudes using the angle direction (4 discretizations)
magnitude_angle_image directed_non_maximum_suppression(magnitude_angle_image magnitude_angle) {

    magnitude_angle_image output_image;

    float angle = 0.0f;
    int rows = magnitude_angle.rows;
    int cols = magnitude_angle.cols;
    output_image.rows = rows;
    output_image.cols = cols;

    output_image.angle = (float*) malloc(rows * cols * sizeof(float));
    output_image.magnitude = (float*) malloc(rows * cols * sizeof(float));

    for (size_t i = 1; i < rows - 1 ; i++){
        for (size_t j = 1; j < cols - 1; j++){
            output_image.angle[i*cols + j] = magnitude_angle.angle[i*cols + j];

            angle = magnitude_angle.angle[i * cols + j]*180.0f/3.1415926f;
            if (angle < 0) angle += 180.0f;


            // printf("valro: %f \n", angle);
            if ((angle >= 0.0f && angle < 22.5f) || (angle >= 157.5f && angle <= 180.0f)){
                if ((magnitude_angle.magnitude[i * cols + (j - 1)] <= magnitude_angle.magnitude[i*cols + j])
                        && (magnitude_angle.magnitude[i * cols + (j + 1)] <= magnitude_angle.magnitude[i*cols + j])){
                    output_image.magnitude[i * cols + j] = magnitude_angle.magnitude[i * cols + j];
                }
                else{
                    output_image.magnitude[i * cols + j] = 0.0;
                }
            }
            else if(angle >= 22.5f && angle < 67.5f){
                if ((magnitude_angle.magnitude[(i-1) * cols + (j + 1)] <= magnitude_angle.magnitude[i*cols + j])
                        && (magnitude_angle.magnitude[(i+1) * cols + (j - 1)] <= magnitude_angle.magnitude[i*cols + j])){
                    output_image.magnitude[i * cols + j] = magnitude_angle.magnitude[i * cols + j];
                }
                else{
                    output_image.magnitude[i * cols + j] = 0.0;
                }
            }
            else if (angle >= 67.5f && angle < 112.5f){
                if ((magnitude_angle.magnitude[(i-1) * cols + j] <= magnitude_angle.magnitude[i*cols + j])
                        && (magnitude_angle.magnitude[(i+1) * cols + j] <= magnitude_angle.magnitude[i*cols + j])){
                    output_image.magnitude[i * cols + j] = magnitude_angle.magnitude[i * cols + j];
                }
                else{
                    output_image.magnitude[i * cols + j] = 0.0;
                }
            }
            else {
                if ((magnitude_angle.magnitude[(i - 1) * cols + (j - 1)] <= magnitude_angle.magnitude[i*cols + j])
                        && (magnitude_angle.magnitude[(i + 1) * cols + (j + 1)] <= magnitude_angle.magnitude[i*cols + j])){
                    output_image.magnitude[i * cols + j] = magnitude_angle.magnitude[i * cols + j];
                }
                else{
                    output_image.magnitude[i * cols + j] = 0.0;
                }    
            }
            
        }
    }
    


    return output_image;
}


// TODO: perform the hysteresis thresholding. Set edges to 255 and non-edges to 0 value.
pgm_file hysteresis_thresholding(magnitude_angle_image max_magnitude_angle, int high_threshold, int low_threshold) {
    pgm_file output_image;

    output_image.rows = max_magnitude_angle.rows;
    output_image.cols = max_magnitude_angle.cols;
    output_image.magic_number = '5';
    output_image.maxval = 255;

    output_image.graymap = (gray*) malloc(output_image.rows * output_image.cols * sizeof(gray));


    for (size_t i = 0; i < output_image.rows * output_image.cols; i++){
        if (max_magnitude_angle.magnitude[i] >= high_threshold){
            output_image.graymap[i] = 255;
        }
        else if (max_magnitude_angle.magnitude[i] >= low_threshold){
            output_image.graymap[i] = 75;
        }
        else{
            output_image.graymap[i] = 0;
        }
        
    }


    for (size_t i = 1; i < output_image.rows - 1; i++){
        for (size_t j = 1; j < output_image.cols - 1; j++){
            if (output_image.graymap[i* output_image.cols + j] == 75 ){


                if (output_image.graymap[(i-1) * output_image.cols + (j-1)] == 255
                    || output_image.graymap[(i-1) * output_image.cols + j] == 255
                    || output_image.graymap[(i-1) * output_image.cols + (j+1)] == 255
                    || output_image.graymap[i * output_image.cols + (j-1)] == 255
                    || output_image.graymap[i * output_image.cols + (j+1)] == 255
                    || output_image.graymap[(i+1) * output_image.cols + (j-1)] == 255
                    || output_image.graymap[(i+1) * output_image.cols + j] == 255
                    || output_image.graymap[(i+1) * output_image.cols + (j+1)] == 255
                ){
                    output_image.graymap[i* output_image.cols + j] = 255;
                }
                else{
                    output_image.graymap[i* output_image.cols + j] = 0;
                }

            }
            
        }
    }

    return output_image;
}

