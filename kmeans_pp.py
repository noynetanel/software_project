# Noy Netanel & Lior Tsemach 
import numpy as np
import pandas as pd
import sys
import math
import mykmeanssp

DEFAULT_Iter = 300  # default value if iter is not provided 

def main(): 
    argv = sys.argv
    if not IsLegal(argv):
        sys.exit(1) # an error as occurred 
    K = int(argv[1])
    iter = int(argv[2]) if len(argv) == 6 else DEFAULT_Iter # iter is optional, if not provided the default value is 300
    eps= float(argv[3]) if len(argv) ==6 else float(argv[2])
    path_1 = argv[4] if len(argv) == 6 else argv[3]
    path_2 = argv[5] if len(argv) == 6 else argv[4]

    # Read the input files
    df1 = pd.read_csv(path_1, header=None)
    df2 = pd.read_csv(path_2, header=None)

    # Perform an inner join using the first column as a key
    points = df1.join(df2.set_index(0), on=0, how='inner', rsuffix='_df2', lsuffix='_df1')

    # Sort the resulting DataFrame by the key column (first column)
    points = points.sort_values(by=0)

    # Drop the key column as it's no longer needed for the K-means algorithm
    points.drop(columns=points.columns[0], inplace=True)
    
    # Convert the DataFrame to a numpy array
    points = points.to_numpy()

    n = points.shape[0]    # number of data points
    d = points.shape[1]    # dimension of data point

    # K-means++ initialization
    centroids_indices = kmeans_pp_init(points, K , n)
    points = points.tolist()

     # Perform K-means clustering using the C extension.
     #the subsequent code block is responsible for performing
     #the K-means clustering algorithm using a function implemented in a C extension module named mykmeanssp.
    try:
        centroids = mykmeanssp.fit(points, centroids_indices, K, n, d, iter, eps)
    except SystemError:
        print("An Error Has Occurred", end="\r\n")
        sys.exit(1)

    # Print the results
    centroids_indices_print(centroids_indices, K)
    centroids_print(centroids, K, d)

 
##################
# help functions #
##################

def min_dist_x_nearest_center(centroids_indices, points, point):
    """
        calculate the min distance D(X) as described in the K-means++ algorithm 
    """
    min_dist = float('inf')
    for centroid_index in centroids_indices:
        dist_point_to_centroid= euclidean_distance(point, points[centroid_index])
        if min_dist > dist_point_to_centroid: # update new min distance 
            min_dist=dist_point_to_centroid
    return min_dist 
            
def kmeans_pp_init(points, k, n):
    """
        inits the kmeans_pp algorithm
    """
    np.random.seed(1234)
    centroids_indices = [np.random.choice(n)]    #choose one center uniformly at random among the data points.
    for _ in range (1,k):
        dists = np.array([min_dist_x_nearest_center(centroids_indices, points, points[i]) if i not in centroids_indices else 0 for i in range(n)])
        total_dists = np.sum(dists)
        # Choose one new data point at random as a new center, using a weighted probability
        # distribution where a point x is chosen with probability proportional to P(xl)
        probabilities = dists/total_dists
        next_centroid_index = np.random.choice(np.arange(n), p=probabilities)
        centroids_indices.append(next_centroid_index)
    return centroids_indices
                   
def euclidean_distance(p, q):
    """
    calculate euclidean distance between 2 points 
     """
    squared_sum = 0
    for i in range(len(p)):
        squared_sum += (p[i] - q[i]) ** 2
    sum = math.sqrt(squared_sum)
    return sum
        

def centroids_print(centroids, K, d):
    """
        prints the centroids as requested 
    """
    for i in range(K):
        res = ""
        for j in range(d-1):
            res += "{:.4f}".format(centroids[i][j]) + ","
        print(res + "{:.4f}".format(centroids[i][d-1]), end="\r\n")   


def centroids_indices_print(centroids_indices, k):
    """
        prints the centroids indices 
    """
    res = ""
    for j in range(k-1):
        res += str(centroids_indices[j]) + ","
    res += str(centroids_indices[k-1])
    print(res, end="\r\n")

        
def IsLegal(argv):
    """
       checks if the the argv is legal 
    """ 
    if len(argv) < 5 or len(argv) > 6:
        print("An Error Has Occurred \n")
        return False
    
    try:
        k = int(argv[1])
        iter = int(argv[2]) if len(argv) == 6 else DEFAULT_Iter
        eps = float(argv[3]) if len(argv) == 6 else float(argv[2])
        path_1 = argv[4] if len(argv) == 6 else argv[3]
        df1 = pd.read_csv(path_1, header=None)
        path_2 = argv[4] if len(argv) == 6 else argv[3]
        df2 = pd.read_csv(path_1, header=None)
        n = df1.shape[0]

    except: # error reading the parametrs. 
        print("An Error Has Occurred \n")
        return False

    if  k <= 1 or  k >= n:
        print("Invalid number of clusters! \n")
        return False
    
    if iter <= 1 or iter >= 1000:
        print("Invalid maximum iteration! \n")
        return False
    
    if eps < 0:
        print("Invalid epsilon! \n")
        return False
    
    return True # all checks passed 


if __name__ == "__main__":
    main()
    
