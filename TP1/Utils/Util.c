// Functions to read, write PBM, PGM, PPM files

#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

char pm_getc(FILE* file) {
    int ich = getc( file );
    if ( ich == EOF )
        pm_error("EOF / read error" );
    char ch = (char) ich;

    if ( ch == '#' ) {
        do {
            ich = getc( file );
            if ( ich == EOF )
                pm_error("EOF / read error" );
            ch = (char) ich;
        } while ( ch != '\n' && ch != '\r' );
    }

    return ch;
}


bit pm_getbit(FILE* file)
{
    char ch;

    do {
        ch = pm_getc( file );
    } while ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' );

    if ( ch != '0' && ch != '1' ) {
        pm_error("read error : != [0|1]" );
    }

    return ( ch == '1' ) ? 1 : 0;
}


unsigned char pm_getrawbyte(FILE* file)
{
    int iby;

    iby = getc( file );
    if ( iby == EOF ) {
        pm_error("EOF / read error " );
    }
    return (unsigned char) iby;
}

int pm_getint( FILE* file)
{
    char ch;
    int i;

    do {
        ch = pm_getc( file );
    } while ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' );

    if ( ch < '0' || ch > '9' ) {
        pm_error( "read error : != [0-9]" );
    }
    i = 0;
    do {
        i = i * 10 + ch - '0';
        ch = pm_getc( file );
    } while ( ch >= '0' && ch <= '9' );

    return i;
}

void pm_error(char *text) {
  fprintf(stderr, "\n%s \n\n", text);
  exit(1);
}


pgm_file read_pgm(char *filename) {
  FILE *ifp;
  int firstchar, rows, cols;

  int is_raw;

  int i, j;
  pgm_file image;

  /* Opening input file */
  ifp = fopen(filename, "r");
  if (ifp == NULL) {
    printf("error in opening file %s\n", filename);
    exit(1);
  }

  /*  Magic number reading */
  firstchar = getc(ifp);
  if (firstchar == EOF) {
    pm_error("EOF / read error / magic number");
  }

  image.magic_number = getc(ifp);
  if (image.magic_number == EOF) {
    pm_error("EOF /read error / magic number");
  }

  if (image.magic_number != '2' && image.magic_number != '5') {
    pm_error(" wrong file type ");
  }

  if (image.magic_number == '5'){
     is_raw = 1;
  } else{
     is_raw = 0;
  }

  /* Reading image dimensions */
  cols = pm_getint(ifp);
  rows = pm_getint(ifp);
  image.maxval = pm_getint(ifp);

  image.rows = rows;
  image.cols = cols;

  /* Memory allocation  */
  image.graymap = (gray *) malloc(cols * rows * sizeof(gray));

  /* Reading */
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      if (is_raw) {
        image.graymap[i * cols + j] = pm_getrawbyte(ifp);
      } else {
        image.graymap[i * cols + j] = pm_getint(ifp);
      }
    }
  }
  /* Closing input file */
  fclose(ifp);

  return image;
}

void write_pgm(pgm_file image, char *filename) {

  FILE *ofp;
  int is_raw, i, j;

  /* Opening output file */
  ofp = fopen(filename, "w");
  if (ofp == NULL) {
    printf("error in opening file %s\n", filename);
    pm_error(" FILE OPEN ERROR");
  }

  if (image.magic_number == '2') {
    is_raw = 0;
  } else {
    is_raw = 1;
  }

  if (is_raw) {
    fprintf(ofp, "P5\n");
  } else {
    fprintf(ofp, "P2\n");
  }

  fprintf(ofp, "%d %d \n", image.cols, image.rows);
  fprintf(ofp, "%d\n", image.maxval);

  for (i = 0; i < image.rows; i++)
    for (j = 0; j < image.cols; j++)
      if (is_raw)
        fprintf(ofp, "%c", image.graymap[i * image.cols + j]);
      else
        fprintf(ofp, "%d ", image.graymap[i * image.cols + j]);

  /* Closing output file */
  fclose(ofp);
}


ppm_file read_ppm(char *filename) {
    FILE *ifp;

    int firstchar, rows, cols;
    int is_raw;

    int i,j;

    ppm_file image;
    
    /* Opening input file */
    ifp = fopen(filename, "r");
    if (ifp == NULL) {
      printf("error in opening file %s\n", filename);
      exit(1);
    }
    
    /*  Magic number reading */
    firstchar = getc(ifp);
    if (firstchar == EOF) {
      pm_error("EOF / read error / magic number");
    }

    image.magic_number = getc(ifp);
    if (image.magic_number == EOF) {
      pm_error("EOF /read error / magic number");
    }

    if (image.magic_number != '3' && image.magic_number != '6') {
      pm_error(" wrong file type ");
    }

    if (image.magic_number == '6'){
      is_raw = 1;
    } else{
      is_raw = 0;
    }

    /* Reading image dimensions */
    cols = pm_getint(ifp);
    rows = pm_getint(ifp);
    image.maxval = pm_getint(ifp);

    image.rows = rows;
    image.cols = cols;

    /* Memory allocation  */
    image.pixmap = (pixel *) malloc(cols * rows * sizeof(pixel));

    /* Reading */
    for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        if (is_raw) {
          image.pixmap[i * cols + j].red = pm_getrawbyte(ifp);
          image.pixmap[i * cols + j].green = pm_getrawbyte(ifp);
          image.pixmap[i * cols + j].blue = pm_getrawbyte(ifp);
          // printf("%c \n",image.pixmap[i * cols + j].red);
          // printf("%c \n",image.pixmap[i * cols + j].green);
          // printf("%c",image.pixmap[i * cols + j].red);
        } else {
          image.pixmap[i * cols + j].red = pm_getint(ifp);
          image.pixmap[i * cols + j].green = pm_getint(ifp);
          image.pixmap[i * cols + j].blue = pm_getint(ifp);
        }
      }
    }

    /* Closing input file */
    fclose(ifp);


    return image;
}

void write_ppm(ppm_file image, char *filename) {

  FILE *ofp;
  int is_raw, i, j;

  /* Opening output file */
  ofp = fopen(filename, "w");
  if (ofp == NULL) {
    printf("error in opening file %s\n", filename);
    pm_error(" FILE OPEN ERROR");
  }

  if (image.magic_number == '3') {
    is_raw = 0;
  } else {
    is_raw = 1;
  }

  if (is_raw) {
    fprintf(ofp, "P6\n");
  } else {
    fprintf(ofp, "P3\n");
  }

  fprintf(ofp, "%d %d \n", image.cols, image.rows);
  fprintf(ofp, "%d\n", image.maxval);

  for (i = 0; i < image.rows; i++)
    for (j = 0; j < image.cols; j++)
      if (is_raw){
          fprintf(ofp, "%c", image.pixmap[i * image.cols + j].red);
          fprintf(ofp, "%c", image.pixmap[i * image.cols + j].green);
          fprintf(ofp, "%c", image.pixmap[i * image.cols + j].blue);
        }
      else{
        fprintf(ofp, "%d ", image.pixmap[i * image.cols + j].red);
        fprintf(ofp, "%d ", image.pixmap[i * image.cols + j].green);
        fprintf(ofp, "%d ", image.pixmap[i * image.cols + j].blue);
      }

  /* Closing output file */
  fclose(ofp);

}

pam_file read_pam(char *filename) {
    pam_file image;
    FILE *ifp;
    int i, j;

    /* Opening input file */
    ifp = fopen(filename, "r");
    if (ifp == NULL) {
        printf("error in opening file %s\n", filename);
        pm_error("FILE OPEN ERROR");
    }

    /* Magic number reading */
    image.magic_number = getc(ifp);
    if (image.magic_number == EOF) {
        pm_error("EOF / read error / magic number");
    }

    if (image.magic_number != '7') {
        pm_error("wrong file type");
    }

    /* Reading image dimensions */
    image.cols = pm_getint(ifp);
    image.rows = pm_getint(ifp);
    image.depth = pm_getint(ifp);
    image.maxval = pm_getint(ifp);

    /* Memory allocation */
    image.map = (char *) malloc(image.cols * image.rows * image.depth * sizeof(char));
    if (!image.map) {
        pm_error("Memory allocation failed");
    }

    /* Reading */
    for (i = 0; i < image.rows; i++) {
        for (j = 0; j < image.cols; j++) {
            int k;
            for (k = 0; k < image.depth; k++) {
                image.map[(i * image.cols + j) * image.depth + k] = pm_getrawbyte(ifp);
            }
        }
    }

    /* Closing input file */
    fclose(ifp);

    return image;
}

void write_pam(pam_file image, char *filename) {
    FILE *ofp;
    int i, j, k;

    /* Opening output file */
    ofp = fopen(filename, "w");
    if (ofp == NULL) {
        printf("error in opening file %s\n", filename);
        pm_error("FILE OPEN ERROR");
    }

    fprintf(ofp, "P7\n");
    fprintf(ofp, "WIDTH %d\n", image.cols);
    fprintf(ofp, "HEIGHT %d\n", image.rows);
    fprintf(ofp, "DEPTH %d\n", image.depth);
    fprintf(ofp, "MAXVAL %d\n", image.maxval);
    fprintf(ofp, "TUPLETYPE %s\n", image.tuple_type);
    fprintf(ofp, "ENDHDR\n");

    for (i = 0; i < image.rows; i++) {
        for (j = 0; j < image.cols; j++) {
            for (k = 0; k < image.depth; k++) {
                fputc(image.map[(i * image.cols + j) * image.depth + k], ofp);
            }
        }
    }

    /* Closing output file */
    fclose(ofp);
}
