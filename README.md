The K-means algorithm is a popular clustering method for finding a partition of N unlabeled
observations into K distinct clusters, where K is a parameter of the method. In this assignment,
you will implement this algorithm in both Python and C.
1.1 K-means
Given a set of N datapoints x1, x2, . . . , xN ∈ Rd
, the goal is to group the data into K ∈ N
clusters, each datapoint is assigned to exactly one cluster and the number of clusters K is such
that 1 < K < N. Each cluster k is represented by its centroid, which is the mean µk ∈ Rd of the
cluster’s members.
