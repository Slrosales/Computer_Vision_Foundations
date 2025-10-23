#include <stdio.h>
#include "gradients.h"


// TODO: compute the gradients of the input image
gradient_image compute_gradients(pgm_file image) {

    gradient_image output_image;

    printf("PLEASE IMPLEMENT compute_gradients\n");
    output_image.rows = 0;
    output_image.cols = 0;

    return output_image;
}


// TODO: compute the magnitude and angle of the gradients
magnitude_angle_image compute_magnitude_angle(gradient_image gradients){
    magnitude_angle_image output_image;

    printf("PLEASE IMPLEMENT compute_magnitude_angle\n");
    output_image.rows = 0;
    output_image.cols = 0;

    return output_image;
}


// TODO: map gradients into an image that can be viewed [0,255]
// Be careful, note that values can be negative and larger than 255.
// Usually a mapping is performed where the 0 magnitude is mapped to the intensity 127 (255\/2).
pgm_file map_gradients_magnitude(magnitude_angle_image magnitude) {
    pgm_file output_image;

    printf("PLEASE IMPLEMENT map_gradients_magnitude\n");
    output_image.rows = 0;
    output_image.cols = 0;

    return output_image;
}