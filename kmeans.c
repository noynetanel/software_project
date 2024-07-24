#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "kmeans.h"


double** kmeans_calc(double**, int*, int, int, int, int, double);
double euclidean_distance(double*, double*, int);
void update_points_to_cluster(double** points, double** centroids, int* closet_cluster_for_vector, int n, int k, int d);
int index_closet_centroid(double* point, double** centroids, int k, int d);
int update_centroids(double** centroids,  int* closet_cluster_for_vector, double** points, int d, int n, int k, double eps);
void freeMemory(double** centroids, int* closet_cluster_for_vector, int n, int k); 


double euclidean_distance(double *p, double *q, int d)
{   int i;
    double squared_sum = 0;
    for (i = 0; i < d; i++) {
        squared_sum += pow((p[i] - q[i]), 2);
    }
    return sqrt(squared_sum);
}



double** kmeans_calc(double** points, int* centroids_indices, int K, int n, int d, int iter, double eps)
{   /* main function of kmeans algorithm */
    int i , c;
    double** centroids;
    int *closet_cluster_for_vector;
    centroids = calloc(K, sizeof(double*));
    closet_cluster_for_vector= calloc(n, sizeof(int));
    if(centroids == NULL || closet_cluster_for_vector == NULL)
    {
        freeMemory(centroids, closet_cluster_for_vector, n, K);
        return NULL; 

    }
    
    for(i=0; i<K; i++)
    {
        centroids[i]= calloc(d, sizeof(double));
        if(centroids[i]== NULL)
        {
            freeMemory(centroids, closet_cluster_for_vector, n, K);
            return NULL;
        }
    }

    /*Initialize centroids as in Algorithm.1*/

    for (i = 0 ; i < K ; i++){
        memcpy(centroids[i], points[centroids_indices[i]], sizeof(double) * d);
    }

    for(i=0; i<iter; i++)
    {
        update_points_to_cluster(points, centroids, closet_cluster_for_vector, n, K, d); 
        c = update_centroids(centroids, closet_cluster_for_vector, points, d, n, K, eps); 
        if(c == -1)   /*failed allocation*/
        {
            freeMemory(centroids, closet_cluster_for_vector, n, K);
            return NULL;
        }
        if(c == 1)    /*finished convergence*/
        {
            break; 
        }   
    }
    
    freeMemory(NULL, closet_cluster_for_vector, n, K);
    return centroids;

}

void update_points_to_cluster(double** points, double** centroids, int* closet_cluster_for_vector, int n, int K, int d)
{
    /* update the new closet cluster */
    int i;
    for(i=0; i<n; i++){
        closet_cluster_for_vector[i] = index_closet_centroid(points[i], centroids, K, d); 
    }

}

int index_closet_centroid(double* point, double** centroids, int K, int d){
    /* return the index of the closet centroid of the given point */
    double min_distance, cur_distance;
    int i, min_distance_index;
    min_distance = INFINITY; 
    min_distance_index = -1;
    for(i=0; i<K; i++)
    {
        cur_distance= euclidean_distance(point, centroids[i], d);
        if(cur_distance < min_distance){
            min_distance = cur_distance;
            min_distance_index = i; 
        }
    }
    return min_distance_index;    
}


int update_centroids(double** centroids,  int* closet_cluster_for_vector, double** points, int d, int n, int K, double eps)
{
    double* new_centroid;
    int i,j,l, is_converged, cnt_vectors_in_cluster;

    new_centroid = calloc(d, sizeof(double));
    is_converged = 1;


    if(new_centroid == NULL)
    {
        return -1;
    }

    for (i = 0 ; i < K ; i++)
    {
        for (j = 0; j < d; j++){
            new_centroid[j] = 0;
        }

        cnt_vectors_in_cluster=0;
        for ( l = 0; l < n; l++)
        {
            if(closet_cluster_for_vector[l]==i)
            {
                for(j=0; j<d; j++)
                {
                    new_centroid[j]+= points[l][j];
                }
                cnt_vectors_in_cluster++;
            }
        }
        /* make average */
        
        for(j=0; j<d; j++)
        {
            new_centroid[j] /= cnt_vectors_in_cluster;
        }

        if(euclidean_distance(centroids[i], new_centroid, d) >= eps)
        {
            is_converged = 0;
        }

        memcpy(centroids[i], new_centroid, sizeof(double) * d);
    }

    free(new_centroid);
    return is_converged;   
}

    

void freeMemory(double** centroids, int* closet_cluster_for_vector, int n, int K)
{  
    int i; 

    if(centroids!=NULL)
    {
        for ( i = 0; i < K; i++)
        {
            if(centroids[i]!=NULL)
            {
                free(centroids[i]);
            }
        }
        free(centroids);  
    }
   
   if(closet_cluster_for_vector!=NULL)
   {
    free(closet_cluster_for_vector);
   }
}

