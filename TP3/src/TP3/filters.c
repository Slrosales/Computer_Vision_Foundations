#include "filters.h"
#include "../Utils/Util.h"
#include <stdio.h>
#include <stdlib.h>


// apply binomial filter to image
pgm_file binomial_filter(pgm_file image, int filter_size, int iterations) {
    pgm_file output_image;
    output_image = image;
    int sum;

    output_image.graymap = (gray *) malloc(image.rows * image.cols * sizeof(gray));

    if (filter_size == 3){
        float binomial_filter[3][3] =  {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
        for (int k = 1; k <= iterations; k++){
            for (int i = 1; i < image.rows - 1; i++){
                for (int j = 1; j < image.cols -1; j++){
                    sum = 0;
                    sum = binomial_filter[0][0] * image.graymap[(i-1) * image.cols + (j-1)] +
                            binomial_filter[0][1] * image.graymap[(i-1)* image.cols + j] +
                            binomial_filter[0][2] * image.graymap[(i-1) * image.cols + (j+1)] +
                            binomial_filter[1][0] * image.graymap[i * image.cols + (j-1)] +
                            binomial_filter[1][1] * image.graymap[i * image.cols + j] +
                            binomial_filter[1][2] * image.graymap[i * image.cols + (j+1)] +
                            binomial_filter[2][0] * image.graymap[(i+1) * image.cols + (j-1)] +
                            binomial_filter[2][1] * image.graymap[(i+1) * image.cols + j] +
                            binomial_filter[2][2] * image.graymap[(i+1) * image.cols + (j+1)];

                    sum /= 16;
                    output_image.graymap[i * image.cols + j] = sum;
                }
            }
            image = output_image;
        }
    } else if (filter_size == 5){
        float binomial_filter[5][5] = {{1, 4, 6, 4, 1}, {4, 16, 24, 16, 4}, {6, 24, 36, 24, 6}, {4, 16, 24, 16, 4}, {1, 4, 6, 4, 1}};
        for (int k = 1; k <= iterations; k++){
            for (int i = 2; i < image.rows - 2; i++){
                for (int j = 2; j < image.cols - 2; j++){
                    sum = 0;
                    sum = binomial_filter[0][0] * image.graymap[(i-2) * image.cols + (j-2)] +
                            binomial_filter[0][1] * image.graymap[(i-2)* image.cols + (j-1)] +
                            binomial_filter[0][2] * image.graymap[(i-2) * image.cols + j] +
                            binomial_filter[0][3] * image.graymap[(i-2) * image.cols + (j+1)] +
                            binomial_filter[0][4] * image.graymap[(i-2) * image.cols + (j+2)] +

                            binomial_filter[1][0] * image.graymap[(i-1) * image.cols + (j-2)] +
                            binomial_filter[1][1] * image.graymap[(i-1)* image.cols + (j-1)] +
                            binomial_filter[1][2] * image.graymap[(i-1) * image.cols + j] +
                            binomial_filter[1][3] * image.graymap[(i-1) * image.cols + (j+1)] +
                            binomial_filter[1][4] * image.graymap[(i-1) * image.cols + (j+2)] +

                            binomial_filter[2][0] * image.graymap[i * image.cols + (j-2)] +
                            binomial_filter[2][1] * image.graymap[i* image.cols + (j-1)] +
                            binomial_filter[2][2] * image.graymap[i * image.cols + j] +
                            binomial_filter[2][3] * image.graymap[i * image.cols + (j+1)] +
                            binomial_filter[2][4] * image.graymap[i * image.cols + (j+2)] +

                            binomial_filter[3][0] * image.graymap[(i+1) * image.cols + (j-2)] +
                            binomial_filter[3][1] * image.graymap[(i+1)* image.cols + (j-1)] +
                            binomial_filter[3][2] * image.graymap[(i+1) * image.cols + j] +
                            binomial_filter[3][3] * image.graymap[(i+1) * image.cols + (j+1)] +
                            binomial_filter[3][4] * image.graymap[(i+1) * image.cols + (j+2)] +

                            binomial_filter[4][0] * image.graymap[(i+2) * image.cols + (j-2)] +
                            binomial_filter[4][1] * image.graymap[(i+2)* image.cols + (j-1)] +
                            binomial_filter[4][2] * image.graymap[(i+2) * image.cols + j] +
                            binomial_filter[4][3] * image.graymap[(i+2) * image.cols + (j+1)] +
                            binomial_filter[4][4] * image.graymap[(i+2) * image.cols + (j+2)];

                    sum /= 256;
                    output_image.graymap[i * image.cols + j] = sum;
                }
            }
            image = output_image;
        }
    }

    return output_image;
}


int comparator(const void *a, const void *b) {
    return (*(gray *)a - *(gray *)b);
}

// apply median filter to image
pgm_file median_filter(pgm_file image, int filter_size, int iterations) {
    pgm_file output_image;
    output_image = image;

    output_image.graymap = (gray *) malloc(image.rows * image.cols * sizeof(gray));

    if (filter_size == 3){
        gray window[9]; 
        for (int k = 1; k <= iterations; k++){
            for (int i = 1; i < image.rows - 1; i++){
                for (int j = 1; j < image.cols -1; j++){
                    window[0] = image.graymap[(i-1) * image.cols + (j-1)];
                    window[1] = image.graymap[(i-1)* image.cols + j];
                    window[2] = image.graymap[(i-1) * image.cols + (j+1)];
                    window[3] = image.graymap[i * image.cols + (j-1)];
                    window[4] = image.graymap[i * image.cols + j];
                    window[5] = image.graymap[i * image.cols + (j+1)];
                    window[6] = image.graymap[(i+1) * image.cols + (j-1)];
                    window[7] = image.graymap[(i+1) * image.cols + j];
                    window[8] = image.graymap[(i+1) * image.cols + (j+1)];

                    qsort(window, 9, sizeof(gray), comparator);
                    output_image.graymap[i * image.cols + j] = window[4];
                }
            }
            image = output_image;
        }
    } else if (filter_size == 5){
        gray window[25];
        for (int k = 1; k <= iterations; k++){
            for (int i = 2; i < image.rows - 2; i++){
                for (int j = 2; j < image.cols - 2; j++){
                    window[0] = image.graymap[(i-2) * image.cols + (j-2)];
                    window[1] = image.graymap[(i-2)* image.cols + (j-1)];
                    window[2] = image.graymap[(i-2) * image.cols + j];
                    window[3] = image.graymap[(i-2) * image.cols + (j+1)];
                    window[4] = image.graymap[(i-2) * image.cols + (j+2)];
                    window[5] = image.graymap[(i-1) * image.cols + (j-2)];
                    window[6] = image.graymap[(i-1)* image.cols + (j-1)];
                    window[7] = image.graymap[(i-1) * image.cols + j];
                    window[8] = image.graymap[(i-1) * image.cols + (j+1)];
                    window[9] = image.graymap[(i-1) * image.cols + (j+2)];
                    window[10] = image.graymap[i * image.cols + (j-2)];
                    window[11] = image.graymap[i* image.cols + (j-1)];
                    window[12] = image.graymap[i * image.cols + j];
                    window[13] = image.graymap[i * image.cols + (j+1)];
                    window[14] = image.graymap[i * image.cols + (j+2)];
                    window[15] = image.graymap[(i+1) * image.cols + (j-2)];
                    window[16] = image.graymap[(i+1)* image.cols + (j-1)];
                    window[17] = image.graymap[(i+1) * image.cols + j];
                    window[18] = image.graymap[(i+1) * image.cols + (j+1)];
                    window[19] = image.graymap[(i+1) * image.cols + (j+2)];
                    window[20] = image.graymap[(i+2) * image.cols + (j-2)];
                    window[21] = image.graymap[(i+2)* image.cols + (j-1)];
                    window[22] = image.graymap[(i+2) * image.cols + j];
                    window[23] = image.graymap[(i+2) * image.cols + (j+1)];
                    window[24] = image.graymap[(i+2) * image.cols + (j+2)];

                    qsort(window, 25, sizeof(gray), comparator);
                    output_image.graymap[i * image.cols + j] = window[12];
                }
            }
            image = output_image;
        }
    }
    return output_image;
}