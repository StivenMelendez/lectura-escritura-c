#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
double sum_subarray(int *array, int start, int end) {
    double sum = 0;
    for (int i = start; i < end; i++) {
        sum += array[i];
    }
    return sum;
}

int main() {
    FILE *input_file = fopen("test2.in", "r");
    if (input_file == NULL) {
        perror("Error al abrir el archivo input.txt");
        return 1;
    }

    int count = 0;
    int value;
    while (fscanf(input_file, "%d", &value) != EOF) {
        count++;
    }

    fseek(input_file, 1, SEEK_SET);

    int *data = malloc(count * sizeof(int));
    if (data == NULL) {
        perror("Error al asignar memoria");
        return 1;
    }
    for (int i = 0; i < count; i++) {
        fscanf(input_file, "%d", &data[i]);
    }
    fclose(input_file);

    int mid = count / 2;

    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("Error al crear el primer proceso hijo");
        return 1;
    } else if (pid1 == 0) {

        double partial_sum1 = sum_subarray(data, 0, mid);
        FILE *output1 = fopen("partial_sum1.txt", "w");
        if (output1 == NULL) {
            perror("Error al abrir el archivo partial_sum1.txt");
            exit(1);
        }
        fprintf(output1, "%.2lf\n", partial_sum1);
        fclose(output1);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("Error al crear el segundo proceso hijo");
        return 1;
    } else if (pid2 == 0) {

        double partial_sum2 = sum_subarray(data, mid, count);
        FILE *output2 = fopen("partial_sum2.txt", "w");
        if (output2 == NULL) {
            perror("Error al abrir el archivo partial_sum2.txt");
            exit(1);
        }
        fprintf(output2, "%.2lf\n", partial_sum2);
        fclose(output2);
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    double partial_sum1, partial_sum2;
    FILE *input1 = fopen("partial_sum1.txt", "r");
    FILE *input2 = fopen("partial_sum2.txt", "r");
    if (input1 == NULL || input2 == NULL) {
        perror("Error al abrir archivos parciales");
        return 1;
    }
    fscanf(input1, "%lf", &partial_sum1);
    fscanf(input2, "%lf", &partial_sum2);
    fclose(input1);
    fclose(input2);

    double total_sum = partial_sum1 + partial_sum2;

    FILE *output_file = fopen("out.txt", "w");
    if (output_file == NULL) {
        perror("Error al abrir el archivo out.txt");
        return 1;
    }
    fprintf(output_file, "%.2lf\n", total_sum);
    fclose(output_file);

    printf("Resultado final: %.2lf\n", total_sum);

    free(data);

    return 0;
}