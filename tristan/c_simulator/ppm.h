#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int width;
  int height;
  int maxval;
} ppm_t;

ppm_t ppm_read(char * filename, uint16_t ** data) ;
int ppm_write(char * filename, int width, int height, int maxval, uint16_t * data) ;
