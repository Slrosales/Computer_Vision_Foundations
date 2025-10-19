
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../Utils/types.h"


histogram_t compute_histogram(pgm_file image) {
    histogram_t histogram;
    histogram.size = 256;

    histogram.histogram = (int *)calloc(histogram.size, sizeof(int));

    int num_pixels = image.rows * image.cols;
    for (int i = 0; i < num_pixels; i++){
      histogram.histogram[image.graymap[i]]++;
    }

    return histogram;

}

int max_value(pgm_file image) {
    int current_max = -1;
    int num_pixels = image.rows * image.cols;
    
    for (int i = 0; i < num_pixels; i++) {
        if (image.graymap[i] > current_max) {
            current_max = image.graymap[i];
        }
    }
    return current_max;
}

int min_value(pgm_file image) {
    int current_min = 255; 
    int num_pixels = image.rows * image.cols;
    
    for (int i = 0; i < num_pixels; i++) {
        if (image.graymap[i] < current_min) {
            current_min = image.graymap[i];
        }
    }
    return current_min;
}


pgm_file histogram_stretching(pgm_file image) {
    pgm_file output_image;
    output_image = image;
    output_image.graymap = (gray *) malloc(image.rows * image.cols * sizeof(gray));

    float T = 0.0;
    int min = min_value(image);
    int max = max_value(image);

    int num_pixels = image.rows * image.cols;

    for (int i = 0; i < num_pixels; i++) {
        T = ((float)(image.graymap[i] - min) / (max - min)) * 255.0;
        output_image.graymap[i] = (gray)T;
    }

    return output_image;
}

pgm_file histogram_equalization(pgm_file image) {
    pgm_file output_image;
    output_image = image;

    output_image.graymap = (gray *) malloc(image.rows * image.cols * sizeof(gray));

    gray T[256];
    float hist_sum = 0;

    histogram_t histogram = compute_histogram(image);
    int num_pixels  = image.rows * image.cols;

    for (int i = 0; i <= 255; i++){
      hist_sum += histogram.histogram[i];
      T[i] = (gray)roundf( ((float) hist_sum / num_pixels) * 255.0 );
    }

    for (int i = 0; i < num_pixels; i++){
      output_image.graymap[i] = T[image.graymap[i]];
    }

    return output_image;
}


// Stores a histogram into a csv file format
void save_histogram(histogram_t histogram, char *path) {

  FILE *fp;
  fp = fopen(path, "w");

  if (fp == NULL) {
    printf("error in opening file %s\n", path);
    exit(1);
  }

  // first row with headers
  fprintf(fp, "Intensity, Count\n");

  // print the histogram values
  for (int i = 0; i < histogram.size; i++) {
    fprintf(fp, "%d,%d\n", i, histogram.histogram[i]);
  }

  fclose(fp);
}