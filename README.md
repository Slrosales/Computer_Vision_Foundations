# Visual Computing - Practical Work

Course: **Visual Computing** - Master of Science in Informatics (MoSIG 1)

Authors:
*   Laura Gómez  [![GitHub Slrosales](https://img.shields.io/badge/by-Slrosales-purple)](https://github.com/Slrosales)
*   Francisco Lugo [![GitHub jlugon](https://img.shields.io/badge/by-jlugon-red)](https://github.com/jlugon)
  
This repository contains the five practical assignments (TPs) developed for the course. All the programs are written in C and work with Netpbm image formats (PGM, PPM and PAM). The image I/O utilities are self-contained, so no external imaging library is required.

## Contents

| TP | Topic | Main programs |
|----|-------|---------------|
| [TP1](TP1) | Image formats and color spaces | `pgmtopgm`, `ppmtorgb`, `ppmtopgm`, `ppmtopam` |
| [TP2](TP2) | Image formation and 3D point-cloud projection | `project_points`, `project_points_bonus` |
| [TP3](TP3) | Spatial filtering and histogram processing | `smoothimage`, `medianfilter`, `createhistogram`, `histogramstretching`, `histogramequalization` |
| [TP4](TP4) | Image gradients and Canny edge detection | `displayGradientMagnitudes`, `cannyEdges` |
| [TP5](TP5) | K-means color segmentation | `kmeans`  |

### TP1 - Image formats and color

- `pgmtopgm`: converts a PGM image between ASCII (`P2`) and binary (`P5`) representations.
- `ppmtorgb`: splits a PPM image into its red, green and blue channels.
- `ppmtopgm`: converts RGB to grayscale, with an optional brightness coefficient.
- `ppmtopam`: converts a PPM image (and its grayscale version) to PAM.

The example images in `TP1/images` are the Mandrill picture

### TP2 - Image formation and 3D projection

Projects a 3D point cloud from an `.off` file onto the image plane using a pinhole or orthographic camera.

- Intrinsic parameters: focal length, image size, principal point, pixel scale factors.
- Extrinsic parameters: yaw/pitch/roll rotation and 3D translation.
- Optional depth buffer for correct occlusion.
- `project_points` writes a PPM image; `project_points_bonus` writes a PAM image with an alpha channel.

Sample point clouds (`human.off`, `frameCube.off`) are in `TP2/src/models`.

### TP3 - Spatial filtering and histograms

- `smoothimage`: binomial smoothing with 3x3, 5x5 or 7x7 kernels and iterative application.
- `medianfilter`: median filtering with configurable kernel size and iterations.
- `createhistogram`: computes a grayscale histogram and saves it as CSV.
- `histogramstretching`: linear contrast stretching.
- `histogramequalization`: global histogram equalization.

### TP4 - Gradients and Canny edges

- `displayGradientMagnitudes`: Sobel-style 3x3 derivatives, gradient magnitude and angle, with visualization.
- `cannyEdges`: full Canny pipeline - smoothing, gradients, non-maximum suppression and hysteresis thresholding.

### TP5 - K-means color segmentation

The command-line interface is defined in `TP5/src/kmeans.c` (clusters, initialization and stopping strategies, optional spatial information and random seed). The PPM I/O and the `kmeans` function are still scaffolds, so this TP is not functional yet.

## Building

Requirements: `gcc` and `make`.

TP1 builds from its own directory; the remaining TPs build from their `src` directory:

```bash
# TP1
cd TP1 && make

# TP2
cd TP2/src && make

# TP3
cd TP3/src && make

# TP4
cd TP4/src && make

# TP5
cd TP5/src && make
```

Each Makefile also provides a `clean` target (for example, `make clean`).

## Usage examples

```bash
# TP1
./ppmtorgb images/mandrill.ppm results/R.ppm results/G.ppm results/B.ppm
./ppmtopgm images/mandrill.ppm results/gray.pgm 0.7

# TP2 (off_file f width height u0 v0 alpha_u alpha_v gamma beta alpha Tx Ty Tz use_depth)
./project_points models/human.off 300 200 150 100 0.01 0.01 0 0 0 0 0 -2 1

# TP3
./smoothimage images/boat.pgm results/boat_smooth.pgm 3 1
./medianfilter images/boat_noise1.pgm results/boat_median.pgm 5 1
./createhistogram images/moon.pgm results/moon_histogram.csv
./histogramstretching images/landscape.pgm results/landscape_stretched.pgm
./histogramequalization images/moon.pgm results/moon_equalized.pgm

# TP4
./displayGradientMagnitudes images/dog.pgm results/dog_gradient.pgm 5 1
./cannyEdges images/dog.pgm results/dog_canny.pgm 3 1 15 10

# TP5
./kmeans images/frog.ppm results/frog_kmeans.ppm 5 0 0 0 42
```