#define BITS_SM 2
#define BITS_TM 2

#define DEBUG

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pixel {
  uint16_t r;
  uint16_t g;
  uint16_t b;
};

/*
 * use lsb for temporal or spatial???
 * -> switch around
 */

uint8_t temporal_mask(struct pixel p) {
  return (p.r & 0b000000000011) << 4 & (p.g & 0b000000000011) << 2 & (p.b & 0b000000000011);
}

/*
 * "video" function, separate function for images -> video (4 frames)
 */
uint16_t spatial_mask_video(struct pixel p[4]) {
  /*
   * only apply spatial dithering, if values over 4 frames don't change
   */
  if (
	  (p[0].r & 0b000000001100) !=
	  (p[1].r & 0b000000001100) !=
	  (p[2].r & 0b000000001100) !=
	  (p[3].r & 0b000000001100)  ||
	  (p[0].b & 0b000000001100) !=
	  (p[1].b & 0b000000001100) !=
	  (p[2].b & 0b000000001100) !=
	  (p[3].b & 0b000000001100)  ||
	  (p[0].r & 0b000000001100) !=
	  (p[1].r & 0b000000001100) !=
	  (p[2].r & 0b000000001100) !=
	  (p[3].r & 0b000000001100)
	 ) {
	return 0;
  } else {
	return (p[0].r & 0b000000001100) << 4 & (p[0].g & 0b000000001100) << 2 & (p[0].b & 0b000000001100);
  }
}

/*
 * assemble 4 subsequent 8-bit frames
 */
void assemble_frames() {}

/*
 * TODO: functions for writing the masks in bw for debugging
 */


  /*
   * logic for writing image data:
   */

// careful: will free pointer automatically
int ppm_write(char * filename, int width, int height, int maxval, uint16_t * data) 
{
  FILE * fp = fopen(filename, "w");
  fprintf(fp, "%s %i %i %i\n", "P3", width, height, maxval);
  for( int i = 0; i < width*height*3; i++) {
	fprintf(fp, "%i ", data[i]);
	if (((i + 1) % (3*width)) == 0){
	  fprintf(fp, "\n");
	  //printf("\nNEWLINE\n");
	}
	if ((i % 3) == 0) 
	{
	  //printf("%i ", ((((i/3) / width) % 2)*2 + (i/3) % width) % 4);
	}
  }

  fclose(fp);
  //free(data);
  return 0;
}

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

#define DELIMITERS " \n"

int main(int argc, char ** argv) {
  FILE * fp;
  char * line = NULL; 
  size_t len = 0;
  ssize_t read;
  char* token;

  fp = fopen(argv[1], "r");
  if (fp == NULL)  {
	printf("Couldn't open file.\n");
	exit(1);
  }

  /*
   * 'Logic' for decoding ppm header
   */
  read = getline(&line, &len, fp);
#ifdef DEBUG
  printf("%s", line);
#endif
  if (read == -1) {
	printf("Empty first line of file. Aborting.\n");
	exit(1);
  }
  token = strtok(line, DELIMITERS);
  if ( strcmp(token, "P3" ) != 0 ) {
	printf("Wrong file format. Aborting.\n");
	exit (1);
  }
  token = strtok(NULL, DELIMITERS);
  if (token == NULL) {
	printf("Error in header. Aborting.\n");
	exit(1);
  }
  int width = atoi(token);
  token = strtok(NULL, DELIMITERS);
  if (token == NULL) {
	printf("Error in header. Aborting.\n");
	exit(1);
  }
  int height = atoi(token);
  token = strtok(NULL, DELIMITERS);
  if (token == NULL) {
	printf("Error in header. Aborting.\n");
	exit(1);
  }
  int maxval = atoi(token);

#ifdef DEBUG
  printf("Header says: width: %i, height: %i, maxval: %i\n", width, height, maxval);
#endif


  /*
   * Logic for decoding image
   */

  // for now just use uint_16 type
  uint16_t * data = malloc(width*height*3*sizeof(uint16_t));
  int i = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
	token = strtok(line, DELIMITERS);
	while (token != NULL) {
	  data[i++] = atoi(token);
	  token = strtok(NULL, DELIMITERS);
	}
  }

#ifdef DEBUG
  printf("Wrote %i numbers.\n", i);
#endif
  if (i != width*height*3) {
	printf("Error in image format. Width * Height * 3 does not equal number of pixels.\n");
	exit(1);
  }

  fclose(fp);

  /*
   * TODO:
   * move above into read function
   * write function
   * logic for temporal dithering
   *	i % 4 determines temporal offset for vertical line
   *	((i / width) % 2 * 2 + i) % 4 <- general expression
   * logic for spatial dithering
   *	j is index for 2x2 tile
   *	((i / width) % 4 + i) % 4
   *
   * Problem: 1d resolution not divisible by 2
   *
   */



  /* 
   * logic for temporal dithering 
   */

  uint16_t * output[4];
  for (i = 0; i < 4; i++) {
	output[i] = malloc(width*height*3*sizeof(uint16_t));
  }

  uint16_t * debug[4];
  for (i = 0; i < 4; i++) {
	debug[i] = malloc(width*height*3*sizeof(uint16_t));
  }
  uint16_t * debug_spatial[4];
  for (i = 0; i < 4; i++) {
	debug_spatial[i] = malloc(width*height*3*sizeof(uint16_t));
  }

  /*
   * Generate 8bit base image.
   */

  printf("About to generate base image.\n");

  for( int i = 0; i < width*height*3; i++) {
	output[0][i] = data[i] >> (BITS_TM + 2);
	output[1][i] = data[i] >> (BITS_TM + 2);
	output[2][i] = data[i] >> (BITS_TM + 2);
	output[3][i] = data[i] >> (BITS_TM + 2);
  }
  for( int i = 0; i < width*height*3; i++) {
	debug[0][i] = 0;
	debug[1][i] = 0;
	debug[2][i] = 0;
	debug[0][i] = 0;
  }

  for( int i = 0; i < width*height*3; i++) {
	debug_spatial[0][i] = 0;
	debug_spatial[1][i] = 0;
	debug_spatial[2][i] = 0;
	debug_spatial[0][i] = 0;
  }



  printf("Starting temporal dithering.\n");

  for( int i = 0; i < width*height; i++) {
	//printf("%i ", ((((i/3) / width) % 2)*2 + (i/3) % width) % 4);
	int offset = ((((i) / width) % 2)*2 + (i) % width) % 4;
	/*
	 * TODO: comment. this is totally incomprehensible
	 * support triple color, not just one
	 */
	switch((data[i] & 0b0000000000001100) >> 2) {
	  case 1: 
		printf("Adding 1/4 pixel\n");
		output[offset][i]++; // r
		output[offset][i+1]++; // g 
		output[offset][i+2]++; // b

		debug[offset][i]++;
		debug[offset][i+1]++;
		debug[offset][i+2]++;
		break;
	  case 2: 
		printf("Adding 1/2 pixel\n");
		output[offset][i]++;
		output[offset][i+1]++;
		output[offset][i+2]++;
		output[(offset + 2) % 4][i]++;
		output[(offset + 2) % 4][i+1]++;
		output[(offset + 2) % 4][i+2]++;

		debug[offset][i]++;
		debug[offset][i+1]++;
		debug[offset][i+2]++;
		debug[(offset + 2) % 4][i]++;
		debug[(offset + 2) % 4][i+1]++;
		debug[(offset + 2) % 4][i+2]++;
		break;
	  case 3: 
		printf("Adding 3/4 pixel\n");
		output[offset][i]++;
		output[offset][i+1]++;
		output[offset][i+2]++;
		output[(offset + 1) % 4][i]++;
		output[(offset + 1) % 4][i+1]++;
		output[(offset + 1) % 4][i+2]++;
		output[(offset + 2) % 4][i]++;
		output[(offset + 2) % 4][i+1]++;
		output[(offset + 2) % 4][i+2]++;

		debug[offset][i]++;
		debug[offset][i+1]++;
		debug[offset][i+2]++;
		debug[(offset + 1) % 4][i]++;
		debug[(offset + 1) % 4][i+1]++;
		debug[(offset + 1) % 4][i+2]++;
		debug[(offset + 2) % 4][i]++;
		debug[(offset + 2) % 4][i+1]++;
		debug[(offset + 2) % 4][i+2]++;
		break;
	  default:
		break;

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
	  // are all 4 pixels equal?
	  // if no -> do nothing
	  // if yes -> switch statement
	  // TODO !!!!!!!
		if ( 1 ) {
		  /*
			data[ind(width, x, y, color, 0)]
			== data[ind(width, x, y, color, 1)]
			== data[ind(width, x, y, color, 2)]
			== data[ind(width, x, y, color, 3)]
		   ) {
		   */
		  printf("Found equal tile!\n");
		  switch(data[ind(width, x, y, color, 0)] & 0b0000000000000011) {
			case 1:
			  output[0][ind(width, x, y, color, 0)]++;

			  output[1][ind(width, x, y, color, 1)]++;

			  output[2][ind(width, x, y, color, 2)]++;

			  output[3][ind(width, x, y, color, 3)]++;
			  debug_spatial[0][ind(width, x, y, color, 0)]++;

			  debug_spatial[1][ind(width, x, y, color, 1)]++;

			  debug_spatial[2][ind(width, x, y, color, 2)]++;

			  debug_spatial[3][ind(width, x, y, color, 3)]++;
			  break;
			case 2:
			  output[0][ind(width, x, y, color, 0)]++;
			  output[0][ind(width, x, y, color, 3)]++;

			  output[1][ind(width, x, y, color, 1)]++;
			  output[1][ind(width, x, y, color, 2)]++;

			  output[2][ind(width, x, y, color, 0)]++;
			  output[2][ind(width, x, y, color, 3)]++;

			  output[3][ind(width, x, y, color, 1)]++;
			  output[3][ind(width, x, y, color, 2)]++;
			  debug_spatial[0][ind(width, x, y, color, 0)]++;
			  debug_spatial[0][ind(width, x, y, color, 3)]++;

			  debug_spatial[1][ind(width, x, y, color, 1)]++;
			  debug_spatial[1][ind(width, x, y, color, 2)]++;

			  debug_spatial[2][ind(width, x, y, color, 0)]++;
			  debug_spatial[2][ind(width, x, y, color, 3)]++;

			  debug_spatial[3][ind(width, x, y, color, 1)]++;
			  debug_spatial[3][ind(width, x, y, color, 2)]++;
			  break;
			case 3:
			  output[0][ind(width, x, y, color, 0)]++;
			  output[0][ind(width, x, y, color, 1)]++;
			  output[0][ind(width, x, y, color, 2)]++;

			  output[1][ind(width, x, y, color, 1)]++;
			  output[1][ind(width, x, y, color, 2)]++;
			  output[1][ind(width, x, y, color, 3)]++;

			  output[2][ind(width, x, y, color, 2)]++;
			  output[2][ind(width, x, y, color, 3)]++;
			  output[2][ind(width, x, y, color, 0)]++;

			  output[3][ind(width, x, y, color, 3)]++;
			  output[3][ind(width, x, y, color, 0)]++;
			  output[3][ind(width, x, y, color, 1)]++;
			  debug_spatial[0][ind(width, x, y, color, 0)]++;
			  debug_spatial[0][ind(width, x, y, color, 1)]++;
			  debug_spatial[0][ind(width, x, y, color, 2)]++;

			  debug_spatial[1][ind(width, x, y, color, 1)]++;
			  debug_spatial[1][ind(width, x, y, color, 2)]++;
			  debug_spatial[1][ind(width, x, y, color, 3)]++;

			  debug_spatial[2][ind(width, x, y, color, 2)]++;
			  debug_spatial[2][ind(width, x, y, color, 3)]++;
			  debug_spatial[2][ind(width, x, y, color, 0)]++;

			  debug_spatial[3][ind(width, x, y, color, 3)]++;
			  debug_spatial[3][ind(width, x, y, color, 0)]++;
			  debug_spatial[3][ind(width, x, y, color, 1)]++;
			  break;
			default:
			  break;
		  }
		}



	  }
	}
	}
	ppm_write("output_0.ppm", width, height, 255, output[0]);
	ppm_write("output_1.ppm", width, height, 255, output[1]);
	ppm_write("output_2.ppm", width, height, 255, output[2]);
	ppm_write("output_3.ppm", width, height, 255, output[3]);
	ppm_write("debug_0.ppm", width, height, 1, debug[0]);
	ppm_write("debug_1.ppm", width, height, 1, debug[1]);
	ppm_write("debug_2.ppm", width, height, 1, debug[2]);
	ppm_write("debug_3.ppm", width, height, 1, debug[3]);
	ppm_write("debug_spatial_0.ppm", width, height, 1, debug_spatial[0]);
	ppm_write("debug_spatial_1.ppm", width, height, 1, debug_spatial[1]);
	ppm_write("debug_spatial_2.ppm", width, height, 1, debug_spatial[2]);
	ppm_write("debug_spatial_3.ppm", width, height, 1, debug_spatial[3]);





	return 0;





  }
