#include "ppm.h"

#define DELIMITERS " \n"
ppm_t ppm_read(char * filename, uint16_t ** data) 
{
  int width, height, maxval;
  FILE * fp;
  char * line = NULL; 
  size_t len = 0;
  ssize_t read;
  char* token;
  fp = fopen(filename, "r");
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
  width = atoi(token);
  token = strtok(NULL, DELIMITERS);
  if (token == NULL) {
	printf("Error in header. Aborting.\n");
	exit(1);
  }
  height = atoi(token);
  token = strtok(NULL, DELIMITERS);
  if (token == NULL) {
	printf("Error in header. Aborting.\n");
	exit(1);
  }
  maxval = atoi(token);

#ifdef DEBUG
  printf("Header says: width: %i, height: %i, maxval: %i\n", width, height, maxval);
#endif

  *data = malloc(width*height*3*sizeof(uint16_t));

  /*
   * Logic for decoding image
   */

  // for now just use uint_16 type
  int i = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
	token = strtok(line, DELIMITERS);
	while (token != NULL) {
	  (*data)[i++] = atoi(token);
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
  return (ppm_t) {.width = width, .height = height, .maxval = maxval};
}

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
  free(data);
  return 0;
}
