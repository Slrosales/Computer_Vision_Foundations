#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include <float.h>

#include "Utils/imageFormationUtils.h"
#include "Utils/Util.h"


int main(int argc, char *argv[])
{
    if (argc != 16)
    {
        printf("\nUsage: %s off_file_name.off "
                            "f width height u_0 v_0 alpha_u alpha_v "
                            "gama beta alpha T_x T_y T_z "
                            "use_depth\n",
         argv[0]);
        exit(0);
    }

    // Reading the input parameters

    // Path to the input OFF file containing the 3D point cloud
    char* off_filename = argv[1];

    // Intrinsic parameters
    float f = atof(argv[2]);         // Focal length (if 0, use orthogonal projection)

    int width = atoi(argv[3]);       
    int height = atoi(argv[4]);      

    float u_0 = atof(argv[5]);       // Principal point - image center x
    float v_0 = atof(argv[6]);       // Principal point - image center y

    float alpha_u = atof(argv[7]);   // Scale factor in u direction (pixels per unit)
    float alpha_v = atof(argv[8]);   

    // Extrinsic parameters 
    float gama = atof(argv[9]);      // Rotation around Z axis in degrees/radians
    float beta = atof(argv[10]);    
    float alpha = atof(argv[11]);   
    float T_x = atof(argv[12]);      // Translation along X axis
    float T_y = atof(argv[13]);
    float T_z = atof(argv[14]);      

    // Use depth map
    int use_depth = atoi(argv[15]);

    // The output filename is generated from the input
    char output_filename[512];
    sprintf(output_filename, "results/%c_%.0f_%d_%d_%.1f_%.1f_%.2f_%.2f_%.1f_%.1f_%.1f_%.1f_%.1f_%.1f_%d.pam",
            off_filename[7], 
            f, width, height, u_0, v_0, alpha_u, alpha_v,
            gama, beta, alpha, T_x, T_y, T_z,
            use_depth);

    // Start processing
    // Read the pointcloud
    struct point3d *points;
    int N_v = 0; // stores the number of vertices
    points = readOff(off_filename, &N_v);

    // and center it
    centerThePCL(points, N_v);

    // Allocate what you think you need
    pixel_rgba* rgba_buffer = (pixel_rgba*) malloc(width * height * sizeof(pixel_rgba));

    for (int i = 0; i < width * height; i++) {
        rgba_buffer[i].red   = 0; 
        rgba_buffer[i].green = 0;
        rgba_buffer[i].blue  = 0;
        rgba_buffer[i].alpha = 0; 
    }

    // Print if depth buffer is being used
    float *z_buffer = NULL;
    float depth = 0.0f;


    if (use_depth) {    
        printf("Using Depth Buffer\n");
        
        z_buffer = (float *) malloc(width * height * sizeof(float));

        for (int i = 0; i < width * height; i++){
            z_buffer[i] = FLT_MAX;
        }
    }

    int orthogonal = 0;
    if (f==0) {
        printf("Using Orthogonal camera\n");
        orthogonal = 1;
    }

    float TransMatrix[16];
    computeTrans(gama, beta, alpha, T_x, T_y, T_z, TransMatrix);
    float px, py, pz, pw;
    float x_prime, y_prime, z_prime;
    float u, v;
    int pixel_index; 
    float x_cam, y_cam;
    // Go through the point-cloud
    for (int i = 0; i < N_v; i++) {
        // Convert homogeneous coordinates
        px = points[i].x;
        py = points[i].y;
        pz = points[i].z;
        pw = 1.0f;         

        // Eq 1: Apply the transformation (Rigin transformation)
        x_prime = TransMatrix[0]*px + TransMatrix[1]*py + TransMatrix[2]*pz + TransMatrix[3]*pw;
        y_prime = TransMatrix[4]*px + TransMatrix[5]*py + TransMatrix[6]*pz + TransMatrix[7]*pw;
        z_prime = TransMatrix[8]*px + TransMatrix[9]*py + TransMatrix[10]*pz + TransMatrix[11]*pw;
        t_w = TransMatrix[12]*px + TransMatrix[13]*py + TransMatrix[14]*pz + TransMatrix[15]*pw;


        // Project the point
        if (orthogonal) {
            // Camera plane coordinates
            x_cam = x_prime;
            y_cam = y_prime;

            // Pixel coordinates of the projected point (UV projection)
            u = (x_cam / alpha_u) + u_0;
            v = (y_cam / alpha_v) + v_0;  

        } else {
            // Eq 3: Camera plane coordinates (pinhole projection)
            x_cam = x_prime / (1 + z_prime / f);
            y_cam = y_prime / (1 + z_prime /f);

            // Eq 4: Pixel coordinates of the projected point (UV projection)
            u = (x_cam / alpha_u) + u_0;
            v = (y_cam / alpha_v) + v_0;
        }

        // Check if the point is inside the image
        int u_int = round(u), v_int = round(v);
        if (u_int >= 0 && u_int < width &&
            v_int >= 0 && v_int < height) {

            if (use_depth){
                depth = z_prime;
                pixel_index = v_int * width + u_int;

                if (depth < z_buffer[pixel_index]){
                    z_buffer[pixel_index] = depth;
                    rgba_buffer[pixel_index].red   = points[i].r;
                    rgba_buffer[pixel_index].green = points[i].g;
                    rgba_buffer[pixel_index].blue  = points[i].b;
                    rgba_buffer[pixel_index].alpha = 255;
                }

            }else{
                pixel_index = v_int * width + u_int;
                rgba_buffer[pixel_index].red   = points[i].r;
                rgba_buffer[pixel_index].green = points[i].g;
                rgba_buffer[pixel_index].blue  = points[i].b;
                rgba_buffer[pixel_index].alpha = 255;
            }
        } 
    }

    // Save the image
    printf("Writing output file %s", output_filename);
    write_pam(output_filename, width, height, rgba_buffer);
}