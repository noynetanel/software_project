import math


def k_means(K, input_data, iter=200):
    """
    k_mean clustering algorithm
    """
    # 1: Initialize centroids as first k datapoints: µk = xk, ∀k ∈ K
    data_points = read_data(input_data)
    N = len(data_points)
    max_iter = 1000
    if K < 1 or K > N or not isinstance(K, int):
        print("Invalid number of clusters!")
        return
    if iter < 1 or iter > max_iter or not isinstance(iter, int):
        print("Invalid maximum iteration!")
        return

    epsilon = 0.01
    prev_centroids = data_points[:K]
    X = data_points

    # until convergence
    for _ in range(iter):
        clusters = [[] for _ in range(K)]
        for Xi in X:
            # Assign every xi i to the closest cluster
            distances = [euclidean_distance(Xi, centroid) for centroid in prev_centroids]
            min_index_cluster = distances.index(min(distances))
            clusters[min_index_cluster].append(Xi)
        # Update the centroids to the mean
        new_centroids = []
        for cluster in clusters:
            if cluster:
                new_centroids.append([sum(dim) / len(cluster) for dim in zip(*cluster)])
            else:
                new_centroids.append([0] * len(X[0]))

        delta_centroid = [euclidean_distance(new_centroid, prev_centroid) for new_centroid, prev_centroid in
                          zip(new_centroids, prev_centroids)]
        if max(delta_centroid) < epsilon:
            break
        prev_centroids = new_centroids.copy()
    for centroid in new_centroids:
        print(",".join(['%.4f' % c for c in centroid]))


def euclidean_distance(p, q):
    """
    calculate euclidean distance
    """
    squared_sum = 0
    for i in range(len(p)):
        squared_sum += (p[i] - q[i]) ** 2
    sum = math.sqrt(squared_sum)
    return sum


def read_data(file_name):
    """
    read data from file
    """
    data = []
    with open(file_name, 'r') as file:
        for line in file:
            data.append([float(x) for x in line.split(",")])
    return data


path = "/Users/noy/Desktop/input_1.txt"
k_means(3, path, 600)
