#include <string.h>
void add_vectors(double *v1, double *v2, int size, double *result);
void mul_vector_number(double *v1, double num, int size, double *result);
void score(double * input, double * output);

void add_vectors(double *v1, double *v2, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] + v2[i];
}
void mul_vector_number(double *v1, double num, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] * num;
}
void score(double * input, double * output) {
    double var0[3];
    double var1[3];
    double var2[3];
    double var3[3];
    if (input[3] <= 0.800000011920929) {
        memcpy(var3, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
    } else {
        if (input[2] <= 4.8500001430511475) {
            if (input[2] <= 4.75) {
                memcpy(var3, (double[]){0.0, 0.9777777777777777, 0.022222222222222223}, 3 * sizeof(double));
            } else {
                memcpy(var3, (double[]){0.0, 0.75, 0.25}, 3 * sizeof(double));
            }
        } else {
            if (input[3] <= 1.699999988079071) {
                memcpy(var3, (double[]){0.0, 0.375, 0.625}, 3 * sizeof(double));
            } else {
                memcpy(var3, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
            }
        }
    }
    double var4[3];
    if (input[2] <= 2.449999988079071) {
        memcpy(var4, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
    } else {
        if (input[2] <= 4.8500001430511475) {
            if (input[3] <= 1.6500000357627869) {
                memcpy(var4, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var4, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
            }
        } else {
            if (input[2] <= 5.049999952316284) {
                memcpy(var4, (double[]){0.0, 0.4166666666666667, 0.5833333333333334}, 3 * sizeof(double));
            } else {
                memcpy(var4, (double[]){0.0, 0.029411764705882353, 0.9705882352941176}, 3 * sizeof(double));
            }
        }
    }
    add_vectors(var3, var4, 3, var2);
    double var5[3];
    if (input[3] <= 0.75) {
        memcpy(var5, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
    } else {
        if (input[3] <= 1.600000023841858) {
            if (input[2] <= 4.950000047683716) {
                memcpy(var5, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var5, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
            }
        } else {
            memcpy(var5, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        }
    }
    add_vectors(var2, var5, 3, var1);
    mul_vector_number(var1, 0.3333333333333333, 3, var0);
    memcpy(output, var0, 3 * sizeof(double));
}
