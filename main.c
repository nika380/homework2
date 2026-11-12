#include <stdio.h>
#include <stdlib.h>

#define MAX_RECORDS 100

typedef struct {
    int id;
    int score;
} Result;

int is_duplicate_id(const Result data[], int size, int id) {
    for (int i = 0; i < size; i++) {
        if (data[i].id == id) {
            return 1;
        }
    }
    return 0;
}

int load_results(const char *filename, Result data[], int *size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    int id, score;
    int status;

    while ((status = fscanf(file, "%d", &id)) != EOF) {
        if (status != 1) {
            fclose(file);
            return 0;
        }

        if (fscanf(file, "%d", &score) != 1) {
            fclose(file);
            return 0;
        }

        if (id < 1 || id > 9999 || score < 0 || score > 100) {
            fclose(file);
            return 0;
        }

        if (is_duplicate_id(data, count, id)) {
            fclose(file);
            return 0;
        }

        if (count >= MAX_RECORDS) {
            fclose(file);
            return 0;
        }

        data[count].id = id;
        data[count].score = score;
        count++;
    }

    if (fclose(file) != 0) {
        return 0;
    }

    *size = count;
    return 1;
}

void show_results(const Result data[], int size) {
    if (size == 0) {
        printf("No data\n");
        return;
    }
    for (int i = 0; i < size; i++) {
        printf("%d %d\n", data[i].id, data[i].score);
    }
}

void show_statistics(const Result data[], int size) {
    if (size == 0) {
        printf("No data\n");
        return;
    }

    int sum = 0;
    int passed = 0;
    int min_idx = 0;
    int max_idx = 0;

    for (int i = 0; i < size; i++) {
        sum += data[i].score;
        if (data[i].score >= 60) {
            passed++;
        }
        if (data[i].score < data[min_idx].score) {
            min_idx = i;
        }
        if (data[i].score > data[max_idx].score) {
            max_idx = i;
        }
    }

    double average = (double)sum / size;
    printf("Count: %d\n", size);
    printf("Average: %.2f\n", average);
    printf("Passed (score >= 60): %d\n", passed);
    printf("Min: %d:%d\n", data[min_idx].id, data[min_idx].score);
    printf("Max: %d:%d\n", data[max_idx].id, data[max_idx].score);
}

void find_result(const Result data[], int size, int target_id) {
    for (int i = 0; i < size; i++) {
        if (data[i].id == target_id) {
            printf("Found: %d %d\n", data[i].id, data[i].score);
            return;
        }
    }
    printf("Not found\n");
}

void individual_variant(const Result data[], int size) {
    if (size == 0) {
        printf("No data\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < size; i++) {
        if (data[i].id % 2 != 0) {
            printf("%d %d\n", data[i].id, data[i].score);
            found = 1;
        }
    }

    if (!found) {
        printf("No matches\n");
    }
}

int save_results(const char *filename, const Result data[], int size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (fprintf(file, "%d %d\n", data[i].id, data[i].score) < 0) {
            fclose(file);
            return 0;
        }
    }

    if (fclose(file) != 0) {
        return 0;
    }

    return 1;
}

void print_menu(void) {
    printf("\nMenu:\n");
    printf("1 - Show all records\n");
    printf("2 - Show statistics\n");
    printf("3 - Find record by id\n");
    printf("4 - Individual task (odd id)\n");
    printf("5 - Save to report.txt\n");
    printf("0 - Exit\n");
    printf("Command: ");
}

int main(void) {
    Result data[MAX_RECORDS];
    int size = 0;

    if (!load_results("results.txt", data, &size)) {
        printf("Load error\n");
        return 1;
    }

    int command;
    while (1) {
        print_menu();
        if (scanf("%d", &command) != 1) {
            printf("Input error\n");
            return 1;
        }

        if (command == 0) {
            break;
        } else if (command == 1) {
            show_results(data, size);
        } else if (command == 2) {
            show_statistics(data, size);
        } else if (command == 3) {
            int search_id;
            printf("Enter id: ");
            if (scanf("%d", &search_id) != 1) {
                printf("Input error\n");
                return 1;
            }
            find_result(data, size, search_id);
        } else if (command == 4) {
            individual_variant(data, size);
        } else if (command == 5) {
            if (save_results("report.txt", data, size)) {
                printf("Saved to report.txt\n");
            } else {
                printf("Save error\n");
            }
        } else {
            printf("Unknown command\n");
        }
    }

    return 0;
}
