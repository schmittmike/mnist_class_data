#ifndef READ_WRITE_IMAGE_H
#define READ_WRITE_IMAGE_H

#define WIDTH 28
#define HEIGHT 28
#define TEST_SET_EXAMPLES 10000
#define TRAIN_SET_EXAMPLES 60000
#define PIXEL_COUNT (WIDTH * HEIGHT)
#define MNIST_TEST_IMAGES "../data/mnist/t10k-images.idx3-ubyte"
#define MNIST_TEST_LABELS "../data/mnist/t10k-labels.idx1-ubyte"
#define MNIST_TRAIN_IMAGES "../data/mnist/train-images.idx3-ubyte"
#define MNIST_TRAIN_LABELS "../data/mnist/train-labels.idx1-ubyte"


struct Pixel {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct Stats {
	int label;                      //1 or 0 label
	float column_stddev_sum;        //sum of each columns std deviation
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
void record_stats_for_all_one_and_zero(FILE *image_stream,
				       FILE *label_stream,
				       const char *filename);

#endif //READ_WRITE_IMAGE_H
