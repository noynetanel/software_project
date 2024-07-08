#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int K,N,d,iter=200;
FILE *file;
int Rows;
int Cols;
int *Num_Vectors_In_Cluster;
double **Sum_Vectors_In_Cluster;
struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};

double euclidean_distance(double *p, double *q)
{
    double squared_sum = 0;
    double sum=0;
    int i=0;
    while (p[i] != '\0' && q[i] != '\0') {
        squared_sum += pow((p[i] - q[i]), 2);
        i++;
    }
    sum = sqrt(squared_sum);
    return sum;
}


double **Read_Data()
{

    struct vector *head_vec, *curr_vec, *next_vec;
    struct cord *head_cord, *curr_cord, *next_cord;
    int i, j, rows = 0, cols;
    double n;
    char c;
    double **data = (double **)malloc(rows * sizeof(double *));
    file=stdin;
    
    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;

    while (scanf("%lf%c", &n, &c) == 2) {
        if (rows == 0 && cols == 0) {
            cols++;
        }

        curr_cord->value = n;
        if (c == '\n') {
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            rows++; 
        } else {
            curr_cord->next = malloc(sizeof(struct cord));
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
            if (rows == 0) {
                cols++; 
            }
        }
    }

     
    
    for (i = 0; i < rows; i++) {
        data[i] = (double *)malloc(cols * sizeof(double));
    }

    curr_vec = head_vec;
    for (i = 0; i < rows; i++) {
        curr_cord = curr_vec->cords;
        for (j = 0; j < cols; j++) {
            data[i][j] = curr_cord->value;
            curr_cord = curr_cord->next;
        }
        curr_vec = curr_vec->next;
    }
    Rows=rows;
    Cols=cols;
    return data;

}

int index_of_min(double *array, int size) {
    int min_index = 0;
    int i;
    for (i = 1; i < size; i++) {
        if (array[i] < array[min_index]) {
            min_index = i;
        }
    }
    return min_index;
}

void copy_2d_array(double **source, double **destination, int rows, int cols) {
    int i,j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            destination[i][j] = source[i][j]; 
        }
    }
}

int kmeans(int k, int iter)
{
    double epsilon= 0.01;
    double ** data_points = Read_Data();
    double **prev_centroids = (double **)malloc(k * sizeof(double *));
    double **new_centroids = (double **)malloc(k * sizeof(double *));
    int min_index_cluster;
    int *sort_vectors_via_cluster = calloc(Rows, sizeof(int));
    int i,c,j,p,m,g;
    int maxim=0;
    double dist;
    for (i = 0; i < k; i++)
     {
        prev_centroids[i] = (double *)malloc(k * sizeof(double));
        new_centroids[i]=(double *)malloc(k * sizeof(double));
        prev_centroids[i]=data_points[i];
     }

    for (c=0; c<iter; c++)
    {
        Num_Vectors_In_Cluster=calloc(k, sizeof(int));
        Sum_Vectors_In_Cluster=(double **)calloc(k, sizeof(double *));
        for (i = 0; i < k; i++) {
            Sum_Vectors_In_Cluster[i] = (double *)calloc(Cols, sizeof(double));
        }

        for (i=0; i<Rows; i++)
        {
            double* vector=data_points[i];
            double *distances=(double *)malloc(sizeof(double)*k);
            for(j=0; j<K; j++)
            {
                distances[j]=euclidean_distance( vector, prev_centroids[j]);
            }
            min_index_cluster = index_of_min(distances,k);
            sort_vectors_via_cluster[i]=min_index_cluster;
            Num_Vectors_In_Cluster[min_index_cluster]+=1;
            for(i=0; i<Cols; i++)
            {
                Sum_Vectors_In_Cluster[min_index_cluster][i]+=vector[i];

            }
        }

        for(g=0; g<k; g++)
        {
            for(m=0; m<Cols; m++)
            {
            new_centroids[g][m]=(Sum_Vectors_In_Cluster[g][m])/Num_Vectors_In_Cluster[g];
            }
            
        }
        

        for (p=0; p<k; p++)
        {
            dist=euclidean_distance(new_centroids[p], prev_centroids[p]);
            if(dist>maxim)
            {
                maxim=dist;
            }
        }

        if (maxim < epsilon)
        {
            break;
        }

        copy_2d_array(new_centroids, prev_centroids, k, Cols);

    }
    
    for (i=0; i<k; i++)
    {
        for(j=0; j<Cols-1; j++)
        {
            printf("%.4f ,",new_centroids[i][j]);
        }
            printf("%.4f ",new_centroids[i][Cols-1]);
        printf("\n");

    }
    return 0;
}

int main(int argc, char **argv)
{    int K, iter;  
     printf("hi");
	K = atoi(argv[1]);
	if (K<=1 && K> Rows)
	    {   
            printf("%s", "Invalid number of clusters!\n");
		    return 1;
        }
    if (argc == 3)
    {
            iter = atoi(argv[2]);
            if( 1> iter || iter > 1000)
            {
                printf("%s", "Invalid maximum iteration!");
                return 1;
            }
    }
    if(argc<2)
    {
        printf( "An Error Has Occurred");
        return 1;
    }
    kmeans(K,iter);
  
	return 0;

}

	
    

    




