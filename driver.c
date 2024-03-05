#include <time.h>
#include "parser.h"

// Function prototypes

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    printf("FIRST and FOLLOW set automated\n");
    printf("Both lexical and syntax analysis modules implemented\n");

    int option;
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds = -1;

    do {
        printf("\n0: For exit\n");
        printf("1: For removal of comments\n");
        printf("2: For printing the token list\n");
        printf("3: For parsing to verify the syntactic correctness\n");
        printf("4: For getting time it takes for code to verify\n");

        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 0:
                break;
            case 1:
                removeComments(argv[1], argv[2]);
                break;
            case 2:
                Tokenize(argv[1]);
                break;
            case 3:
                start_time = clock();
                parseInputSourceCode(argv[1]);
                end_time = clock();

                total_CPU_time = (double)(end_time - start_time);
                total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

                break;
            case 4:
                if(total_CPU_time_in_seconds == -1) parseInputSourceCode(argv[1]);
                printf("Total CPU time in seconds: %.2f\n", total_CPU_time_in_seconds);
                break;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    } while (option != 0);

    return 0;
}
