#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <math.h>
#include <float.h> 

#include "gradients.h"

gradient_image compute_gradients(pgm_file image) {

    float div = 16.0f;

    gradient_image output_image;
    int rows = image.rows;
    int cols = image.cols;

    output_image.rows = rows;
    output_image.cols = cols;

    output_image.derivative_x = (float*) malloc(rows * cols * sizeof(float));
    output_image.derivative_y = (float*) malloc(rows * cols * sizeof(float));

    memset(output_image.derivative_x, 0, rows * cols * sizeof(float));
    memset(output_image.derivative_y, 0, rows * cols * sizeof(float));

    float h1[3][3] = {{-3, 0, 3}, {-10, 0, 10}, {-3, 0, 3}};
    float h2[3][3] = {{-3, -10, -3}, {0, 0, 0}, {3, 10, 3}};

    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            float sum = 0.0f;

            for (int k = - 1; k <= 1; k++) {
                for (int l = - 1; l <= 1; l++) {
                    sum += h1[k + 1][l + 1] * image.graymap[(i + k) * cols + (j + l)];
                }
            }
            output_image.derivative_x[i * cols + j] = sum / div;
        }
    }

    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            float sum = 0.0f;

            for (int k = - 1; k <= 1; k++) {
                for (int l = - 1; l <= 1; l++) {
                    sum += h2[k + 1][l + 1] * image.graymap[(i + k) * cols + (j + l)];
                }
            }
            output_image.derivative_y[i * cols + j] = sum / div;
        }
    }
    

    return output_image;
}

// TODO: compute the magnitude and angle of the gradients
magnitude_angle_image compute_magnitude_angle(gradient_image gradients){
    magnitude_angle_image output_image;
    int rows = gradients.rows;
    int cols = gradients.cols;

    output_image.rows = rows;
    output_image.cols = cols;

    output_image.angle = (float*) malloc(rows * cols * sizeof(float));
    output_image.magnitude = (float*) malloc(rows * cols * sizeof(float));
        
    for (int i = 0; i < rows * cols; i++){
        float ix = gradients.derivative_x[i];
        float iy = gradients.derivative_y[i];

        output_image.angle[i] = atan2f(iy, ix);

        output_image.magnitude[i] = sqrtf(ix * ix + iy * iy);
    }
        
    return output_image;
}

// TODO: map gradients into an image that can be viewed [0,255]
// Be careful, note that values can be negative and larger than 255.
// Usually a mapping is performed where the 0 magnitude is mapped to the intensity 127 (255\/2).
pgm_file map_gradients_magnitude(magnitude_angle_image magnitude) {

    int rows = magnitude.rows;
    int cols = magnitude.cols;
    int total_pixels = rows * cols;

    pgm_file output_image;
    output_image.rows = rows;
    output_image.cols = cols;
    output_image.maxval = 255;
    output_image.magic_number = '5';

    output_image.graymap = (gray*) malloc(total_pixels * sizeof(gray));

    float max_magnitude = 0.0f; 
    for (int i = 0; i < total_pixels; i++) {
        if (magnitude.magnitude[i] > max_magnitude) {
            max_magnitude = magnitude.magnitude[i];
        }
    }

    if (max_magnitude < 0.0001f) {
        for (int i = 0; i < total_pixels; i++) {
            output_image.graymap[i] = 0;
        }
    } else {
        for (int i = 0; i < total_pixels; i++) {
            float normalized_value = (magnitude.magnitude[i] / max_magnitude) * 255.0f;            
            output_image.graymap[i] = (gray)normalized_value;
        }
    }

    return output_image;
}