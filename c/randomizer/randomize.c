#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include <math.h>

#define INPUT_BUFFER_SIZE 50
#define LINE_LIMIT 100000

int main(int argc, char** argv)
{
        uint32_t i, j;
        uint8_t confirmation_char;
        float test_split;
        uint8_t buffer[INPUT_BUFFER_SIZE];
        uint32_t line_count = 0;
        uint32_t choice, possible_index;
        char *ret;
        time_t t;

        /* accept only one file (argument) as input */
        if (argc != 2) {
                printf("[usage] input one data file to randomize\n");
                exit(1);
        }

        /* ask questions *****************************************************/
        printf("test split percentage? \nenter percentage, [0 to 1]: ");
        scanf("%f", &test_split);

        /* confirm file ******************************************************/
        printf("process '%s' ? [y/n] ", argv[1]);
        scanf("\n%c", &confirmation_char);

        if (confirmation_char == 'y') {
                printf("processing '%s'...\n", argv[1]);
        } else if (confirmation_char == 'n') {
                printf("file not processed\n");
                exit(0);
        } else {
                printf("[error] enter y or n.\n");
                exit(0);
        }

        /* process file ******************************************************/

        FILE *input_file = fopen(argv[1], "r");

        /* check file length */
        while (fgets((char*)buffer, INPUT_BUFFER_SIZE, input_file) != NULL) {
                line_count++;
        }
        printf("[info] line count: %d\n", line_count);

        /* generate random mapping */
        uint32_t* indexes = malloc(sizeof(uint32_t) * line_count);
        uint32_t* random_indexes = malloc(sizeof(uint32_t) * line_count);
        /* numbers 1-100 */
        for (i = 0; i < line_count; i++) {
                indexes[i] = i;
        }

        /* generate random list **********************************************/
        /* pick indexes "out of a hat" */
        srand((unsigned)time(&t));
        for (i = 0; i < line_count; i++) {

                /* size of "hat" gets smaller each loop */
                possible_index = line_count - i;
                choice = rand() % (possible_index);
                random_indexes[i] = indexes[choice];

                /* shift the rest of the values into place */
                for (j = choice; j < possible_index; j++) {
                        indexes[j] = indexes[j+1];
                }
        }

        /* debug print */
        //for (i = 0; i < line_count; i++) {
        //        printf("%d\t%d\n", indexes[i], random_indexes[i]);
        //}

        /* open file and write randomized lines ******************************/
        FILE *output_file_train = fopen("randomize_train.txt", "w+");
        if (output_file_train == NULL) {
                perror(NULL);
        }
        FILE *output_file_test = fopen("randomize_test.txt", "w+");
        if (output_file_test == NULL) {
                perror(NULL);
        }

        i = 0;
        while (1) {

                /* get the "random index-th" line */
                fseek(input_file, 0, SEEK_SET);
                for (j = 0; j < random_indexes[i]; j++) {
                        ret = fgets((char*)buffer, INPUT_BUFFER_SIZE, input_file);
                }

                i++;

                if (ret == NULL) break;
                if (i > floorf(test_split * line_count)) {
                        fputs((char*)buffer, output_file_train);
                } else {
                        fputs((char*)buffer, output_file_test);
                }
        }

        printf("done.\n");

        free(random_indexes);
        if (fclose(input_file) != 0) {
                printf("error closing input file?");
        }

        return 0;
}
