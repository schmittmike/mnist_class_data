#ifndef READ_WRITE_IMAGE_H
#define READ_WRITE_IMAGE_H

/* width and height of each mnist image */
#define WIDTH 28
#define HEIGHT 28

/* this is defining the length of the input data, I've been fiddling with this
 * so that the amount of data that needs to be processed by matlab is lower
 */
#define TEST_SET_EXAMPLES 5000
#define TRAIN_SET_EXAMPLES 5000

/* this is the actual length of the data files */
//#define TEST_SET_EXAMPLES 10000
//#define TRAIN_SET_EXAMPLES 60000
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

/* this is the number of output columns */
#define LABEL_COUNT 4

/* this is which labels to use of output columns.
 *
 * for now it has to start at zero and go in sequence because i didn't think
 * about choosing arbitrary numbers when i coded it up the first time, but that
 * would be much better.
 */
#define LABELS_IN_USE	(LABEL_0)| \
                        (LABEL_1)| \
                        (LABEL_2)| \
                        (LABEL_3)

/* this only works for my specific directory structure, point these at wherever
 * your data file actually is
 */
#define MNIST_TEST_IMAGES "../data/mnist/t10k-images.idx3-ubyte"
#define MNIST_TEST_LABELS "../data/mnist/t10k-labels.idx1-ubyte"
#define MNIST_TRAIN_IMAGES "../data/mnist/train-images.idx3-ubyte"
#define MNIST_TRAIN_LABELS "../data/mnist/train-labels.idx1-ubyte"


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
	int label;                      //1 or 0 label

        /* stats */
	float column_stddev_sum;        //sum of each columns std deviation
	float row_stddev_sum;           //sum of each columns std deviation
	float avg;                      //average pixel brightness
        int pixel_count;                //number of pixels more on than off
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

void generate_output_label_string(char *string, int output_count, int label_pos);

void record_stats_for_label_bitfield(FILE *image_stream,
				     FILE *label_stream,
                                     int bool_stream_is_test_data,
                                     unsigned int label_bitfield,
				     const char *filename);

#endif //READ_WRITE_IMAGE_H
