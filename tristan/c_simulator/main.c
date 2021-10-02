#define BITS_SM 2
#define BITS_TM 2

#define DEBUG

#include "ppm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWITCH_TEMP_SPAT_BITS
// switch temporal and spatial bits
// the default is spatial -> lsb

/*
 * "video" function, separate function for images -> video (4 frames)
 */

/*
 * return the one dimensional index in image data
 * positions are :
 * 0 1
 * 2 3
 */
static inline
int ind(int width, int tile_x, int tile_y, int color, int position) {
  int x = (2*tile_x + width*2*tile_y + (position & 0b01) + width*((position & 0b10) >> 1))*3 + color;
#ifdef DEBUG
  // printf("Accessing index %i\n", x);
#endif
  return x;
}

int main(int argc, char ** argv) {

  int width, height, maxval;
  uint16_t * data;

  ppm_t image = ppm_read(argv[1], &data);
  // todo: fix this
  width = image.width;
  height = image.height;
  maxval = image.maxval;

  /* Allocate some space for output */
  uint16_t * output[4];
  uint16_t * debug[4];
  uint16_t * debug_spatial[4];
  for (int i = 0; i < 4; i++) {
	output[i] = malloc(width*height*3*sizeof(uint16_t));
	debug[i] = malloc(width*height*3*sizeof(uint16_t));
	debug_spatial[i] = malloc(width*height*3*sizeof(uint16_t));
  }

  /*
   * Generate 8bit base image.
   */
  printf("About to generate base image.\n");
  printf("%i %i\n", width, height);
  for( int i = 0; i < width*height*3; i++) {
	for (int j = 0; j < 4; j++) {
	  output[j][i] = data[i] >> (BITS_TM + 2);
	  debug[j][i] = 0;
	  debug_spatial[j][i] = 0;
	}
  }


  printf("Starting temporal dithering.\n");

  /* 
   * logic for temporal dithering 
   */
  int option_subpixel_scrambling = 1; // integrate with other options
  for( int i = 0; i < width*height; i++) {
	//printf("%i ", ((((i/3) / width) % 2)*2 + (i/3) % width) % 4);
	int offset = (((i / width) % 4) + (i) % width) % 4;
	/*
	 * TODO: comment. this is totally incomprehensible
	 */
	int position = 3*i;
	for (int color = 0; color < 3; color++) {
	  // printf("Writing to position %i\n", position + color);
#ifdef SWITCH_TEMP_SPAT_BITS
	  switch((data[position+color] & 0b0000000000000011)) 
#else
		switch((data[position+color] & 0b0000000000001100) >> 2) 
#endif
		{
		  case 1: 
			printf("Adding 1/4 pixel\n");
			output[(offset + option_subpixel_scrambling*color) % 4][position+color]++; // r

			debug[(offset + option_subpixel_scrambling*color) % 4][position+color]++;
			break;
		  case 2: 
			printf("Adding 1/2 pixel\n");
			output[(offset + option_subpixel_scrambling*color) % 4][position+color]++;
			output[((offset + option_subpixel_scrambling*color) + 2) % 4][position+color]++;

			debug[(offset + option_subpixel_scrambling*color) % 4][position+color]++;
			debug[((offset + option_subpixel_scrambling*color) + 2) % 4][position+color]++;
			break;
		  case 3: 
			printf("Adding 3/4 pixel\n");
			output[(offset + option_subpixel_scrambling*color) % 4][position+color]++;
			output[((offset + option_subpixel_scrambling*color) + 1) % 4][position+color]++;
			output[((offset + option_subpixel_scrambling*color) + 2) % 4][position+color]++;

			debug[(offset + option_subpixel_scrambling*color) % 4][position+color]++;
			debug[((offset + option_subpixel_scrambling*color) + 1) % 4][position+color]++;
			debug[((offset + option_subpixel_scrambling*color) + 2) % 4][position+color]++;
			break;
		  default:
			break;

		}
	}
  }

	/*
	 * logic for spatial dithering
	 */
	printf("About to start spatial dithering.\n");


	// loop over image in tiles of 2x2
	if (width%2 || height % 2) {
	  printf("Image not splitable in tiles of size 2x2\n");
	  exit(1);
	}

	for( int y = 0; y < height/2; y++) {
	  for( int x = 0; x < width/2; x++) {
		for( int color = 0; color < 3; color++) {
#ifdef SWITCH_TEMP_SPAT_BITS
		  switch(data[ind(width, x, y, color, 0)] & 0b0000000000001100 >> 2) 
#else
			switch(data[ind(width, x, y, color, 0)] & 0b0000000000000011) 
#endif
			{
			  case 1:
				output[0][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;

				output[1][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;

				output[2][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;

				output[3][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[0][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;

				debug_spatial[1][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;

				debug_spatial[2][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;

				debug_spatial[3][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;
				break;
			  case 2:
				output[0][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;
				output[0][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;

				output[1][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;
				output[1][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;

				output[2][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;
				output[2][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;

				output[3][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;
				output[3][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[0][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[0][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;

				debug_spatial[1][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[1][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;

				debug_spatial[2][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[2][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;

				debug_spatial[3][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[3][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;
				break;
			  case 3:
				output[0][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;
				output[0][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;
				output[0][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;

				output[1][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;
				output[1][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;
				output[1][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;

				output[2][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;
				output[2][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;
				output[2][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;

				output[3][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;
				output[3][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;
				output[3][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[0][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[0][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[0][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;

				debug_spatial[1][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[1][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[1][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;

				debug_spatial[2][ind(width, x, y, color, (2 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[2][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[2][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;

				debug_spatial[3][ind(width, x, y, color, (3 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[3][ind(width, x, y, color, (0 + option_subpixel_scrambling*color) % 4)]++;
				debug_spatial[3][ind(width, x, y, color, (1 + option_subpixel_scrambling*color) % 4)]++;
				break;
			  default:
				break;
			}
		}
	}
  }
  ppm_write("output_0.ppm", width, height, 255, output[0]);
  ppm_write("output_1.ppm", width, height, 255, output[1]);
  ppm_write("output_2.ppm", width, height, 255, output[2]);
  ppm_write("output_3.ppm", width, height, 255, output[3]);
  ppm_write("debug_temporal_0.ppm", width, height, 1, debug[0]);
  ppm_write("debug_temporal_1.ppm", width, height, 1, debug[1]);
  ppm_write("debug_temporal_2.ppm", width, height, 1, debug[2]);
  ppm_write("debug_temporal_3.ppm", width, height, 1, debug[3]);
  ppm_write("debug_spatial_0.ppm", width, height, 1, debug_spatial[0]);
  ppm_write("debug_spatial_1.ppm", width, height, 1, debug_spatial[1]);
  ppm_write("debug_spatial_2.ppm", width, height, 1, debug_spatial[2]);
  ppm_write("debug_spatial_3.ppm", width, height, 1, debug_spatial[3]);

  return 0;
}
