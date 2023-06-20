#ifndef READ_WRITE_IMAGE_H
#define READ_WRITE_IMAGE_H

/* width and height of each mnist image */
#define WIDTH 28
#define HEIGHT 28


/* this is the actual length of the data files */
#define TEST_SET_EXAMPLES 10000
#define TRAIN_SET_EXAMPLES 60000
#define PIXEL_COUNT (WIDTH * HEIGHT)

#define LABEL_0 (1u << 0)
#define LABEL_1 (1u << 1)
#define LABEL_2 (1u << 2)
#define LABEL_3 (1u << 3)
#define LABEL_4 (1u << 4)
#define LABEL_5 (1u << 5)
#define LABEL_6 (1u << 6)
#define LABEL_7 (1u << 7)
#define LABEL_8 (1u << 8)
#define LABEL_9 (1u << 9)

/* this is the total possible number of columns, shouldn't change*/
#define LABEL_COUNT_MAX 10

/* BEGIN ** CHANGE THIS SECTION **********************************************/

/* use this to limit data volume for testing */
#define DEBUG_OVERRIDE_LINE_COUNT 4000
/* the number of output columns. */
#define LABEL_COUNT 4
/* which labels to use of in the output columns. */
#define LABELS_IN_USE	(LABEL_0)| \
			(LABEL_1)| \
			(LABEL_3)| \
			(LABEL_8)

/* this only works for my specific directory structure, point these at wherever
 * your data file actually is
 */
#define MNIST_TEST_IMAGES "../data/mnist/t10k-images.idx3-ubyte"
#define MNIST_TEST_LABELS "../data/mnist/t10k-labels.idx1-ubyte"
#define MNIST_TRAIN_IMAGES "../data/mnist/train-images.idx3-ubyte"
#define MNIST_TRAIN_LABELS "../data/mnist/train-labels.idx1-ubyte"

/* END ** CHANGE THIS SECTION ************************************************/

/* keeps the RGB data together, but this isn't really necessary for this use
 * case because its all black and white so the pixels are all the same.
 */
struct Pixel {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

/* contains that stats that go to each label */
struct Stats {
	/* label */
	int label;			//1 or 0 label

	/* stats */
	float column_stddev_sum;	//sum of each columns std deviation
	float row_stddev_sum;		//sum of each columns std deviation
	float avg;			//average pixel brightness
	int pixel_count;		//number of pixels more on than off
};


void generate_ppm(struct Pixel data[HEIGHT][WIDTH], const char *path);

int get_nth_image(struct Pixel data[HEIGHT][WIDTH], FILE *stream, int n);

int get_nth_label(FILE *stream, int n);

float average(float data[], int n);

float stddev(float data[], int n);

float sum(float data[], int n);

void stats_from_nth_image(struct Stats *s,
			  FILE *image_stream,
			  FILE *label_stream,
			  int n);

void generate_all_one_and_zero_ppm(struct Pixel data[HEIGHT][WIDTH],
				   FILE *image_stream,
				   FILE *label_stream);

void generate_output_label_string(char *string, unsigned int label_bitfield, int label_pos);

void record_stats_for_label_bitfield(FILE *image_stream,
				     FILE *label_stream,
				     unsigned int label_bitfield,
				     const char *filename);

#endif //READ_WRITE_IMAGE_H
