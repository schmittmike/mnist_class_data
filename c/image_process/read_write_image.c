#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "read_write_image.h"


/* generate ppm data (super simple image format) using pixels from data
 * I USED THIS FOR TESTING, not actually important
 */
void generate_ppm(struct Pixel data[HEIGHT][WIDTH], const char *path)
{
	FILE *out_file = fopen(path, "w+");
	if (out_file == NULL) {
		perror(path);
		exit(1);
	}

	/* this section fills out the magic ppm header */
	fputs("P6\n", out_file);
	char wh[10];
	sprintf(wh, "%d\n", WIDTH);
	fputs(wh, out_file);
	sprintf(wh, "%d\n", HEIGHT);
	fputs(wh, out_file);
	fputs("255\n", out_file);

	/* write the pixel data to the image */
	fwrite(data, WIDTH*HEIGHT, sizeof(struct Pixel), out_file);
	fclose(out_file);

	return;
}


/* write out the actual ppm files (super simple image format)
 * I USED THIS FOR TESTING, not actually important
 *
 * requires (? i think) a directory called "image" in the same location the
 * executable is called, and it will fill up with only the 1 and 0 MNIST digits
 */
void generate_all_one_and_zero_ppm(struct Pixel data[HEIGHT][WIDTH],
				   FILE *image_stream,
				   FILE *label_stream)
{
	int i;
	int ret;
	char name[25];

	printf("[INFO] generating .ppm files in \"image\" directory\n");
	for (i = 0; i < TEST_SET_EXAMPLES; i++) {
		ret = get_nth_label(label_stream, i);

		if (ret == 0) {
			get_nth_image(data, image_stream, i);
			sprintf(name, "images/zero_%04d.ppm", i);
			generate_ppm(data, name);
		}
		if (ret == 1) {
			get_nth_image(data, image_stream, i);
			sprintf(name, "images/one_%04d.ppm", i);
			generate_ppm(data, name);
		}
	}

	return;
}


/* Here's where the actual stuff starts: */

/* fill Pixel struct with pixel values based on MNIST file format */
int get_nth_image(struct Pixel data[HEIGHT][WIDTH], FILE *stream, int n)
{
	int i, j;
	unsigned char pixel_color;

	/* first image at offset 16, so start counting from there */
	fseek(stream, 16 + (n*PIXEL_COUNT), SEEK_SET);

	/* fill image */
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			fread(&pixel_color, sizeof(unsigned char), 1, stream);
			data[i][j].red = pixel_color;
			data[i][j].green = pixel_color;
			data[i][j].blue = pixel_color;
		}
	}

	return 0;
}


/* return label (number from 0 - 9) */
int get_nth_label(FILE *stream, int n)
{
	char label;

	/* first label at offset 8 */
	fseek(stream, 8 + n, SEEK_SET);
	fread(&label, sizeof(char), 1, stream);

	return label;
}


/* finds standard deviation of size n float array */
float stddev(float data[], int n)
{
	int i;
	float avg = 0;
	float mean_squared_error = 0;

	avg = sum(data, n) / n;

	/* calc mean squared error */
	for (i = 0; i < n; i++) {
		mean_squared_error += (data[i] - avg) * (data[i] - avg);
	}

	return sqrtf(mean_squared_error / n);
}


/* calculate sum of size n float array */
float sum(float data[], int n)
{
	int i;
	float total = 0;
	for (i = 0; i < n; i++) total += data[i];

	return total;
}


/* this func does the heavy lifting math wise:
 * populates Stats struct with stats for image selected.
 * assumes that rgb values are all the same, uses r. */
void stats_from_nth_image(struct Stats *s,
			  FILE *image_stream,
			  FILE *label_stream,
			  int n)
{
	int i, j;
	float column_pixel_stddevs[WIDTH] = {0};
	float row_pixel_stddevs[HEIGHT] = {0};
	float column_pixels[HEIGHT] = {0};
	float row_pixels[WIDTH] = {0};
	int total_pixel_count = 0;
	float total_pixel_sum = 0;
	struct Pixel data[HEIGHT][WIDTH];

	get_nth_image(data, image_stream, n);

	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
			column_pixels[j] = data[i][j].red;
			row_pixels[j] = data[j][i].red;

			/* total brighness of image */
			total_pixel_sum += data[i][j].red;

			/* count pixel as more on than off */
			if (data[i][j].red > 128) total_pixel_count += 1;
		}
		column_pixel_stddevs[i] = stddev(column_pixels, HEIGHT);
		row_pixel_stddevs[i] = stddev(row_pixels, WIDTH);

	}

	s->column_stddev_sum = sum(column_pixel_stddevs, WIDTH);
	s->row_stddev_sum = sum(row_pixel_stddevs, HEIGHT);
	s->avg = total_pixel_sum / (WIDTH*HEIGHT);
	s->label = get_nth_label(label_stream, n);
	s->pixel_count = total_pixel_count;
}


/* writes a string that contains tab-spaced output labels based on the actual
 * input label passed to it, and the number of outputs.
 *
 * input "char *string"
 * should have enough room for (output_count*2)+1 characters
 *  EX:
 * -> 3 outputs for labels 0, 1, 2
 *
 *  this function is ALSO unnecessarily compicated but it made writing the
 *  other function much easier.
 *  also probably more reusable for something else who knows.
 *
 *  for output true label = 0, string = "1	0	0"
 *  for output true label = 1, string = "0	1	0"
 *  for output true label = 2, string = "0	0	1"
 */
void generate_output_label_string(char *string, int output_count, int label_pos)
{
	int i;
	/* increment by label and tab (2 chars) */
	for (i = 0; i < output_count; i++) {
		if (i == label_pos) { // *2 to line up with i+=2
			string[i*2] = '1';
			string[i*2+1] = '\t';
		} else {
			string[i*2] = '0';
			string[i*2+1] = '\t';
		}
	}
	/* get rid of last tab, (trailing whitespace in output) */
	string[i*2-1] = '\0';
}


/* writes to a file the stats of image labels corresponding to little endian (?)
 * bitfield defined in the header
 *
 * This is way unnecessarily complicated, but it makes it easier to change stuff
 * in one location in the header file. maybe more reusable? idk
 *
 * ex: (1 << 0) = 0x00000001 = label 0
 * ex: (1 << 3) = 0x00000004 = label 3
 * ex: 0b10010010	     = labels 1, 4, and 7.
 */
void record_stats_for_label_bitfield(FILE *image_stream,
				     FILE *label_stream,
				     int bool_stream_is_test_data,
				     unsigned int label_bitfield,
				     const char *filename)
{
	int i;
	int ret;
	int line_count;
	struct Stats s;
	char line[100];
	char label_string[10*2+1] = {0};

	FILE *stat_file = fopen(filename, "w+");
	if (stat_file == NULL) {
		perror(filename);
		exit(1);
	}

	if (bool_stream_is_test_data) {
		line_count = TEST_SET_EXAMPLES;
	} else {
		line_count = TRAIN_SET_EXAMPLES;
	}

	printf("[INFO] generating %s file in current directory\n", filename);
	for (i = 0; i < line_count; i++) {
		ret = get_nth_label(label_stream, i);
		/* check bitfield for each this label */
		if ((unsigned int)((1u << ret) & label_bitfield) > 0) {
			stats_from_nth_image(&s, image_stream, label_stream, i);
			generate_output_label_string(label_string, LABEL_COUNT, s.label);

			sprintf(line, "%f\t%f\t%d\t%s\n",
						s.column_stddev_sum,
						s.row_stddev_sum,
						s.pixel_count,
						label_string);

			fputs(line, stat_file);
		}
	}

	fclose(stat_file);
	return;
}


int main(int argc, char **argv)
{
	struct Pixel data[HEIGHT][WIDTH];
	(void)data;
	int n;
	char n_input[10];

	/* i used the argc stuff for more testing, ignore this... */
	if (argc == 1) {
		n = 2;
		(void)n; //to avoid unused error
	} else if (argc == 2) {
		/* UNSAFE!(?) but what do I know */
		sprintf(n_input, "%s", argv[1]);
		n = atoi(n_input);
	} else if (argc > 2) {
		printf("usage: first argument is nth image to generate\n");
	}

	/* open files */
	FILE *test_images = fopen(MNIST_TEST_IMAGES, "r");
	FILE *test_labels = fopen(MNIST_TEST_LABELS, "r");
	FILE *train_images = fopen(MNIST_TRAIN_IMAGES, "r");
	FILE *train_labels = fopen(MNIST_TRAIN_LABELS, "r");

	//generate_all_one_and_zero_ppm(data, test_images, test_labels);
	record_stats_for_label_bitfield(train_images, train_labels, 0, LABELS_IN_USE, "stat_train.dat");
	record_stats_for_label_bitfield(test_images, test_labels, 1, LABELS_IN_USE, "stat_test.dat");

	fclose(test_images);
	fclose(test_labels);

	return 0;
}
