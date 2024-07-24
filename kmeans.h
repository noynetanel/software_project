# ifndef KMEANS_H_
# define KMEANS_H_

double** kmeans_calc(double** points, int* centroids_indices, int K, int n, int d, int iter, double eps);

# endif