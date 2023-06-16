#include <stdio.h>

#include "ones_zeros_model.h"

#define TEST_DATA_STAT_FILE "../image_process/stat_test.dat"
#define LINE_BUFFER_SIZE 100

int main() {
        FILE *test_data = fopen(TEST_DATA_STAT_FILE, "r");
        char line_buffer[LINE_BUFFER_SIZE];
        int label;
        float input[2] = {0};
        float output = 0;

        int n = 0;
        int total_correct = 0;

        while ( fgets(line_buffer, LINE_BUFFER_SIZE, test_data) != NULL) {
                sscanf(line_buffer, "%f\t%f\t%d\n", &input[0], &input[1], &label);

                hand_try5((float *)&input,(float *)&output);
                if (output >= 0.5 && label >= 0.5) {
                        total_correct++;
                } else if (output <= 0.5 && label <= 0.5) {
                        total_correct++;
                }
                n++;
        }
        printf("tests: \t\t%d\ncorrect: \t%d\n%% correct: \t%.2f\n",
                        n, total_correct, 100*(float)total_correct/n);
}
