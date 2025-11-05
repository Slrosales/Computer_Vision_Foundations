#include "filters.h"
#include "../Utils/Util.h"
#include <stdio.h>
#include <stdlib.h>


// apply binomial filter to image
pgm_file binomial_filter(pgm_file image, int filter_size, int iterations) {
    pgm_file output_image;
    output_image = image;
    int sum, div, div_sum = 0;
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

        for (int i = 0; i < filter_size; i++){
            div_sum += binomial_filter[0][i];
        }
        
        div = div_sum * div_sum;
        
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
        for (int i = 0; i < filter_size; i++){
            div_sum += binomial_filter[0][i];
        }
        
        div = div_sum * div_sum;
        
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

        for (int i = 0; i < filter_size; i++){
            div_sum += binomial_filter[0][i];
        }
        
        div = div_sum * div_sum;

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

    gray window[filter_size*filter_size];
    for (int k = 1; k <= iterations; k++){
        for (int i = kernel; i < image.rows - kernel; i++){
            for (int j = kernel; j < image.cols - kernel; j++){

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
    

    return output_image;
}