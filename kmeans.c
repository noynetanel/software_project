#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int K,N,d,iter=200;
FILE *file;
int Rows;
int Cols;
double *Num_Vectors_In_Cluster;
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
{   int i;
    double squared_sum = 0;
    for (i = 0; i < Cols; i++) {
        squared_sum += pow((p[i] - q[i]), 2);
    }
    return sqrt(squared_sum);

}
void print_2d_array(double **array, int rows, int cols) {
    int i,j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%f ", array[i][j]);
        }
        printf("\n");
    }
}
double **Read_Data(void)
{  
    
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    int i, j, rows = 0, cols=0;
    double n;
    char c;
    double **data; 
    double *mem;
    
    file=stdin;


    head_cord = malloc(sizeof(struct cord));
  
    if (head_cord == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
        
    }

    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    if (head_vec == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

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
               if (curr_vec->next == NULL) {
                printf("An Error Has Occurred\n");
                exit(1);
            }
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            if (head_cord == NULL) {
                printf("An Error Has Occurred\n");
                exit(1);
            }
            curr_cord = head_cord;
            curr_cord->next = NULL;
            rows++; 
        } else {
            curr_cord->next = malloc(sizeof(struct cord));
            if (curr_cord->next == NULL) {
                printf("An Error Has Occurred\n");
                exit(1);
            }
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
            if (rows == 0) {
                cols++; 
            }
        }
    }
    Rows=rows;
    Cols=cols;
    if(K>Rows){
     printf("Invalid number of clusters!\n");
		exit(1);
    }
    mem=(double *)malloc(Rows*Cols*sizeof(double));
    data=(double **)malloc(Rows*sizeof(double *));
        if (data == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }
    
    for (i = 0; i < rows; i++) {
        data[i] = mem+cols*i;

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

    return data;

}

double **copy_2d_array(double **array, int rows, int cols) {
    int i, j;
    double **copy = (double **)malloc(rows * sizeof(double *));
    if (copy == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }
    for (i = 0; i < rows; i++) {
        copy[i] = (double *)malloc(cols * sizeof(double));
        if (copy[i] == NULL) {
            printf("An Error Has Occurred\n");
            exit(1);
        }
        for (j = 0; j < cols; j++) {
            copy[i][j] = array[i][j];
        }
    }
    return copy;
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


int kmeans(int k, int iter)
{
    int counter =0;
    double epsilon= 0.001;
    double *mem1=(double *)malloc(k*Cols*sizeof(double));
     double *mem2=(double *)malloc(k*Cols*sizeof(double));
    double ** data_points = Read_Data();
    double **prev_centroids = (double **)malloc(k * sizeof(double *));
    double **new_centroids = (double **)malloc(k * sizeof(double *));
    int min_index_cluster;
    double* vector;
    int *sort_vectors_via_cluster = (int *)calloc(Rows, sizeof(int));
    double *mem_sum_vec=(double *)malloc(k*Cols*sizeof(double));
    int i,c,j,p,m,g,r,h;
    double maxim;
    double dist;
    double *distances=(double *)malloc(sizeof(double)*k);
    Num_Vectors_In_Cluster= (double *)calloc(k, sizeof(double));
    Sum_Vectors_In_Cluster=(double **)malloc(k * sizeof(double *));
    for (i = 0; i < k; i++) {
        Sum_Vectors_In_Cluster[i] = mem_sum_vec+Cols*i;
         }

        
    if (prev_centroids == NULL || new_centroids == NULL || sort_vectors_via_cluster == NULL) {
        
        return -1;
    }


    for (i = 0; i < k; i++) {
        prev_centroids[i] = mem1+Cols*i;
        new_centroids[i]=mem2+Cols*i;

    }

    for ( i = 0; i <k; i++)
    {
         for (j = 0; j < Cols; j++) {

            prev_centroids[i][j] = data_points[i][j];
           
           }
    }

  
    for (c=0; c<iter; c++)
    {
        maxim=0;
        counter +=1;
        for(r=0; r<k; r++)
        {
            for(i=0; i<Cols; i++)
            {
                Sum_Vectors_In_Cluster[r][i]=0.0;
               
            }
            Num_Vectors_In_Cluster[r]=0.0;
        }


        if (Num_Vectors_In_Cluster == NULL || Sum_Vectors_In_Cluster == NULL) {
            return -1;
        }

        for (i=0; i<Rows; i++)
        {
            
            vector=data_points[i];
            for(j=0; j<k; j++)
            {
                distances[j]=euclidean_distance( vector, prev_centroids[j]);
            }
 
            min_index_cluster = index_of_min(distances,k);
            sort_vectors_via_cluster[i]=min_index_cluster;
            Num_Vectors_In_Cluster[min_index_cluster]+=1.0;
            for(h=0; h<Cols; h++)
            {

                Sum_Vectors_In_Cluster[min_index_cluster][h]+=vector[h];
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
            if(dist > maxim)
            {
                maxim = dist;

            } 
        }
        
        prev_centroids=copy_2d_array(new_centroids,k,Cols);
       
        if (maxim < epsilon)
        {
        for (i=0; i<k; i++)
        {
                for(j=0; j<Cols-1; j++)
                    {
                    printf("%.4f ,",new_centroids[i][j]);
                    }
            printf("%.4f ",new_centroids[i][Cols-1]);
            printf("\n");

        }
        printf("%d",counter);
        
        goto finish;
        }      
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
    finish:
    printf("%d",counter);
    return 0;

}

int main(int argc, char **argv)
{   
    
    
        if(argc<2)
    {
        printf( "An Error Has Occurred");
        return 1;
    }

	K = atoi(argv[1]);
	if (K<=1)
	    {   
            printf("Invalid number of clusters!\n");
		    return 1;
        }
    if (argc == 3)
    {
            iter = atoi(argv[2]);
            if( 1> iter || iter > 1000)
            {
                printf("Invalid maximum iteration!");
                return 1;
            }
    }

    if(kmeans(K,iter)==-1)
    {
        printf("An Error Has Occurred");
    }
    printf("success");

	return 0;
}
