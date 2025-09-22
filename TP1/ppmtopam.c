#include <stdlib.h>

#include "Utils/types.h"
#include "Utils/Util.h"

#include "TP1/TP1_funcs.h"

int main(int argc, char* argv[]) {
    // Image definitions
    ppm_file image;
    pgm_file image_gray;
    pam_file image_pam;

    /* Arguments */
    if (argc == 3){        
        /* Opening input file */
        image = read_ppm(argv[1]);
    
        // Conversion
        ppm_to_pam(image, &image_pam, NULL);
    
        /* Write output file */
        write_pam(image_pam, argv[2]);

    } else if (argc == 4){
        /* it also takes a PGM image and creates a PAM image
        with the colors of the PPM image and the alpha value
        (transparency) from the PGM image.*/ 

        /* Opening input file */
        image = read_ppm(argv[1]);
        image_gray = read_pgm(argv[2]);

        // Conversion
        ppm_to_pam(image, &image_pam, &image_gray);
    
        /* Write output file */
        write_pam(image_pam, argv[3]);

    } else {
        printf("\nUsage : %s file_in file_out \n\n", argv[0]);
        exit(0);    
    }

    return 0;
}