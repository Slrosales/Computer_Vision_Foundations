
#include <stdio.h>
#include <stdlib.h>

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

int max_value(pgm_file image){
  int max_value = -1;
  histogram_t histogram;
  histogram.size = 256;

    histogram.histogram = (int *)calloc(histogram.size, sizeof(int));
    int num_pixels = image.rows * image.cols;
    
    for (int i = 0; i < num_pixels; i++){
      histogram.histogram[image.graymap[i]]++;
      if (histogram.histogram[image.graymap[i]] != 0 && image.graymap[i] > max_value){
        max_value = image.graymap[i];
      }
    }

    return max_value;
}

int min_value(pgm_file image){
  int min_value = 257;
  histogram_t histogram;
  histogram.size = 256;

    histogram.histogram = (int *)calloc(histogram.size, sizeof(int));
    int num_pixels = image.rows * image.cols;
    
    for (int i = 0; i < num_pixels; i++){
      histogram.histogram[image.graymap[i]]++;
      if (histogram.histogram[image.graymap[i]] != 0 && image.graymap[i] < min_value){
        min_value = image.graymap[i];
      }
    }

    return min_value;
}


pgm_file histogram_stretching(pgm_file image) {
    pgm_file output_image;
    output_image = image;

    int max = 256;

    int max_value_xd = min_value(image);
    printf("%d\n", max_value_xd);

    return output_image;
}

pgm_file histogram_equalization(pgm_file image) {

    printf("PLEASE IMPLEMENT histogram_equalization\n");

    pgm_file output_image;

    output_image = image;

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