#include "filters.h"
#include "../Utils/Util.h"
#include <stdio.h>
#include <stdlib.h>


// apply binomial filter to image
pgm_file binomial_filter(pgm_file image, int filter_size, int iterations) {
    pgm_file output_image;
    output_image = image;
    int sum, div;
    int kernel = filter_size/2;
    
    output_image.graymap = (gray *) malloc(image.rows * image.cols * sizeof(gray));
    
    for (int i = 0; i < image.cols; i++){
        for (int j = 0, k = image.rows - kernel; j < kernel; j++, k++){
            output_image.graymap[j * image.cols + i] = image.graymap[j * image.cols + i];
            output_image.graymap[k * image.cols + i] = image.graymap[k * image.cols + i];
        }
    }
    
    for (int i = 0; i < image.rows; i++){
        for (int j = 0, k = image.cols - kernel; j < kernel; j++, k++){
            output_image.graymap[i * image.cols + j] = image.graymap[i * image.cols + j];
            output_image.graymap[i * image.cols + k] = image.graymap[i * image.cols + k];
        }
    }

    
    
    if (filter_size == 3){
        int binomial_filter[3][3] =  {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
        div = 16;
        for (int k = 1; k <= iterations; k++){
        
            for (int i = kernel; i < image.rows - kernel; i++){
                for (int j = kernel; j < image.cols - kernel; j++){
                    sum = 0;

                    for (int k = i-kernel, idx = 0; k <= i+kernel; k++, idx++){
                        for (int l = j-kernel, idy = 0; l <= j+kernel; l++, idy++){
                            sum += binomial_filter[idx][idy] * image.graymap[k * image.cols + l];
                        }
                    }
                    sum /= 256;
                    output_image.graymap[i * image.cols + j] = sum;
                }
            }
            image = output_image;
        }
    }
    else if (filter_size == 5){
        int binomial_filter[5][5] = {{1, 4, 6, 4, 1}, {4, 16, 24, 16, 4},
                                     {6, 24, 36, 24, 6}, {4, 16, 24, 16, 4}, {1, 4, 6, 4, 1}};
        div=256;
        
        for (int k = 1; k <= iterations; k++){
        
            for (int i = kernel; i < image.rows - kernel; i++){
                for (int j = kernel; j < image.cols - kernel; j++){
                    sum = 0;

                    for (int k = i-kernel, idx = 0; k <= i+kernel; k++, idx++){
                        for (int l = j-kernel, idy = 0; l <= j+kernel; l++, idy++){
                            sum += binomial_filter[idx][idy] * image.graymap[k * image.cols + l];
                        }
                    }
                    sum /= 256;
                    output_image.graymap[i * image.cols + j] = sum;
                }
            }
            image = output_image;
        }
    }

    else if(filter_size == 7){
        // int binomial_filter[7][7] = {
        //         {1, 6, 15, 20, 15, 6, 1},
        //         {6, 36, 90, 120, 90, 36, 6},
        //         {15, 90, 225, 300, 225, 90, 15},
        //         {20, 120, 300, 400, 300, 120, 20},
        //         {15, 90, 225, 300, 225, 90, 15},
        //         {6, 36, 90, 120, 90, 36, 6},
        //         {1, 6, 15, 20, 15, 6, 1}
        //     };
        // div = 5120;

        int binomial_filter[7][7] = {
        {0,   0,   1,   2,   1,   0,   0},
        {0,   3,  13,  22,  13,   3,   0},
        {1,  13,  59,  97,  59,  13,   1},
        {2,  22,  97, 159,  97,  22,   2},
        {1,  13,  59,  97,  59,  13,   1},
        {0,   3,  13,  22,  13,   3,   0},
        {0,   0,   1,   2,   1,   0,   0}
        };
        div = 1003;
        for (int k = 1; k <= iterations; k++){
        
            for (int i = kernel; i < image.rows - kernel; i++){
                for (int j = kernel; j < image.cols - kernel; j++){
                    sum = 0;

                    for (int k = i-kernel, idx = 0; k <= i+kernel; k++, idx++){
                        for (int l = j-kernel, idy = 0; l <= j+kernel; l++, idy++){
                            sum += binomial_filter[idx][idy] * image.graymap[k * image.cols + l];
                        }
                    }
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
    int kernel = filter_size/2;
    
    output_image.graymap = (gray *) malloc(image.rows * image.cols * sizeof(gray));

    // Cols border 
    for (int i = 0; i < image.cols; i++){
        for (int j = 0, k = image.rows - kernel; j < kernel; j++, k++){
            output_image.graymap[j * image.cols + i] = image.graymap[j * image.cols + i];
            output_image.graymap[k * image.cols + i] = image.graymap[k * image.cols + i];
        }
    }
    
    // Rows border 
    for (int i = 0; i < image.rows; i++){
        for (int j = 0, k = image.cols - kernel; j < kernel; j++, k++){
            output_image.graymap[i * image.cols + j] = image.graymap[i * image.cols + j];
            output_image.graymap[i * image.cols + k] = image.graymap[i * image.cols + k];
        }
    }

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