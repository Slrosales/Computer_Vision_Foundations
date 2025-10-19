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
                    sum /= div;
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
                    sum /= div;
                    output_image.graymap[i * image.cols + j] = sum;
                }
            }
            image = output_image;
        }
    }

    else if(filter_size == 7){
        int binomial_filter[7][7] = {
                {1, 6, 15, 20, 15, 6, 1},
                {6, 36, 90, 120, 90, 36, 6},
                {15, 90, 225, 300, 225, 90, 15},
                {20, 120, 300, 400, 300, 120, 20},
                {15, 90, 225, 300, 225, 90, 15},
                {6, 36, 90, 120, 90, 36, 6},
                {1, 6, 15, 20, 15, 6, 1}
            };
        div = 4096;

        for (int k = 1; k <= iterations; k++){
        
            for (int i = kernel; i < image.rows - kernel; i++){
                for (int j = kernel; j < image.cols - kernel; j++){
                    sum = 0;

                    for (int k = i-kernel, idx = 0; k <= i+kernel; k++, idx++){
                        for (int l = j-kernel, idy = 0; l <= j+kernel; l++, idy++){
                            sum += binomial_filter[idx][idy] * image.graymap[k * image.cols + l];
                        }
                    }
                    sum /= div;
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

                    // // SER
                    for (int k = i-kernel, idx = 0; k <= i+kernel; k++, idx++){
                        for (int l = j-kernel, idy = 0; l <= j+kernel; l++, idy++){
                            int index = idx*filter_size + idy; 
                            window[index] = image.graymap[k * image.cols + l];
                            // printf("%d ahora si \n", idx+idy+1);
                        }
                    }
                    qsort(window, filter_size*filter_size, sizeof(gray), comparator);
                    output_image.graymap[i * image.cols + j] = window[filter_size*filter_size/2];
                }
            }
            image = output_image;
        }
    } else if (filter_size == 5){
        gray window[25];
        for (int k = 1; k <= iterations; k++){
            for (int i = 2; i < image.rows - 2; i++){
                for (int j = 2; j < image.cols - 2; j++){

                     for (int k = i-kernel, idx = 0; k <= i+kernel; k++, idx++){
                        for (int l = j-kernel, idy = 0; l <= j+kernel; l++, idy++){
                            int index = idx*filter_size + idy; 
                            window[index] = image.graymap[k * image.cols + l];
                        }
                    }
                    qsort(window, filter_size*filter_size, sizeof(gray), comparator);
                    output_image.graymap[i * image.cols + j] = window[filter_size*filter_size/2];
                }
            }
            image = output_image;
        }
    }

    return output_image;
}