#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "read_write_image.h"


/* generate ppm using pixels from data */
void generate_ppm(struct Pixel data[HEIGHT][WIDTH], const char *path)
{
	FILE *out_file = fopen(path, "w+");
	if (out_file == NULL) {
		perror(path);
		exit(1);
	}

	fputs("P6\n", out_file);
	char wh[10];
	sprintf(wh, "%d\n", WIDTH);
	fputs(wh, out_file);
	sprintf(wh, "%d\n", HEIGHT);
	fputs(wh, out_file);
	fputs("255\n", out_file);

	fwrite(data, WIDTH*HEIGHT, sizeof(struct Pixel), out_file);
	fclose(out_file);

	return;
}


//char flip_endian_char(char input)
//{
//}

/* fill data with pixel values */
int get_nth_image(struct Pixel data[HEIGHT][WIDTH], FILE *stream, int n)
{
	int i, j;
	unsigned char pixel_color;

	/* first image at offset 16 */
	fseek(stream, 16 + (n*PIXEL_COUNT), SEEK_SET);

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


/* return label */
int get_nth_label(FILE *stream, int n)
{
	char label;

	/* first label at offset 8 */
	fseek(stream, 8 + n, SEEK_SET);
	fread(&label, sizeof(char), 1, stream);

	return label;
}


/* takes average of array of n floats */
float average(float data[], int n)
{
	int i;
	float sum = 0;
	for (i = 0; i < n; i++) {
		sum += data[i];
	}
	return sum / n;
}


/* finds standard deviation of array of n floats */
float stddev(float data[], int n)
{
	int i;
	float avg = 0;
	float sum = 0;

	avg = average(data, n);

	/* calc mean squared error */
	for (i = 0; i < n; i++) {
		sum += (data[i] - avg) * (data[i] - avg);
	}

	return sqrtf(sum / n);
}

float sum(float data[], int n)
{
        int i;
        float total = 0;
        for (i = 0; i < n; i++) total += data[i];

        return total;
}


/* populates Stats struct for image data
 * assumes that rgb values are all the same, uses r.*/
void stats_from_nth_image(struct Stats *s,
			  FILE *image_stream,
			  FILE *label_stream,
			  int n)
{
	int i, j;
	float column_pixel_stddevs[WIDTH] = {0};
        float column_pixels[HEIGHT] = {0};
        int total_pixel_count = 0;
	float total_pixel_sum = 0;
	struct Pixel data[HEIGHT][WIDTH];

	get_nth_image(data, image_stream, n);

	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
                        /* sum of brighnesses in a column */
			column_pixels[j] += data[i][j].red;

                        /* total brighness of image */
			total_pixel_sum += data[i][j].red;

                        /* count pixel as more on than off */
                        if (data[i][j].red > 128) total_pixel_count += 1;
		}
                column_pixel_stddevs[i] = stddev(column_pixels, HEIGHT);

	}

	s->column_stddev_sum = sum(column_pixel_stddevs, WIDTH);
	s->avg = total_pixel_sum / (WIDTH*HEIGHT);
	s->label = get_nth_label(label_stream, n);
        s->pixel_count = total_pixel_count;
}


/* requires (?) a directory called "image" where the executable is called,
 * and it will fill up with ppms for the 1 and 0 MNIST digits */
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


/* writes to a file called the stats of only the 1 and 0 images */
void record_stats_for_all_one_and_zero(FILE *image_stream,
				       FILE *label_stream,
				       const char *filename)
{
	int i;
	int ret;
	struct Stats s;
	char line[100];

	FILE *stat_file = fopen(filename, "w+");
	if (stat_file == NULL) {
		perror(filename);
		exit(1);
	}

	printf("[INFO] generating %s file in current directory\n", filename);
	for (i = 0; i < TEST_SET_EXAMPLES; i++) {
		ret = get_nth_label(label_stream, i);

		if (ret == 0 || ret == 1) {
			stats_from_nth_image(&s, image_stream, label_stream, i);
			sprintf(line, "%f\t%d\t%d\n",
				s.column_stddev_sum, s.pixel_count, s.label);
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

	if (argc == 1) {
		n = 2;
		(void)n; //to avoid unused error
	} else if (argc == 2) {
		/* UNSAFE!(?) */
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
	if (test_images == NULL) {
		perror("can't open MNIST_TEST_IMAGES");
		exit(1);
	}
	if (test_labels == NULL) {
		perror("can't open MNIST_TEST_LABELS");
		exit(1);
	}
	if (train_images == NULL) {
		perror("can't open MNIST_TRAIN_IMAGES");
		exit(1);
	}
	if (train_labels == NULL) {
		perror("can't open MNIST_TRAIN_LABELS");
		exit(1);
	}

	//get_nth_image(data, test_images, n);
	//generate_ppm(data, "test.ppm");

	//generate_all_one_and_zero_ppm(data, test_images, test_labels);

	record_stats_for_all_one_and_zero(train_images, train_labels, "stat_train.dat");
	record_stats_for_all_one_and_zero(test_images, test_labels, "stat_test.dat");

	fclose(test_images);
	fclose(test_labels);

	return 0;
}
