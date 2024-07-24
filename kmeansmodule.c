#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "kmeans.h"
#include <math.h>

void freeMemoryModule(double**, int*, double**, int, int);

static PyObject* fit(PyObject *self, PyObject *args)
{
    
    int K, n, d, iter, i, l, j, index;
    double eps, cord;
    double **points;
    int *centroids_indices;
    double ** centroids; 
    double *points_mem;
    PyObject *vector;
    PyObject *py_centroids;
    PyObject *py_points;
    PyObject *py_centroids_indices;
    PyObject *item;

    /* This parses the Python arguments */
    if(!PyArg_ParseTuple(args, "OOiiiid", &py_points, &py_centroids_indices, &K, &n, &d, &iter, &eps)) {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                        PyObject* so it is used to signal that an error has occurred. */
    }

    if (PyObject_Length(py_points) < 0 || PyObject_Length(py_centroids_indices) < 0){
        return NULL;
    }

    centroids_indices = malloc(K * sizeof(int));
    if (centroids_indices == NULL){
        return NULL;
    }

    for(i=0; i < K; i++)
    {
        item = PyList_GetItem(py_centroids_indices, i);
        index = PyLong_AsLong(item);
        centroids_indices[i] = index;
    }
    
    points_mem = (double *)calloc(n * d, sizeof(double)); /* sit continuously in memory */
    points = (double **)calloc(n, sizeof(double *));  

    if(points== NULL || points_mem == NULL){
        freeMemoryModule(points, centroids_indices, NULL, n, K);
        if (points_mem != NULL) {
            free(points_mem);
        }
        return NULL;
    }

     for(i=0; i<n; i++)
    {
        points[i]= malloc(d*sizeof(double));
        if(points[i]== NULL)
        {
             freeMemoryModule(points, centroids_indices, NULL, n, K);
             return NULL;
        }
    }

    
    for (i = 0; i < n; i++) {
        points[i] = points_mem + i * d;
    }


    for(i=0; i<n; i++)
    {
        vector = PyList_GetItem(py_points, i);
        for(l=0; l<d; l++)
        {
         cord= PyFloat_AsDouble(PyList_GetItem(vector, l));
         points[i][l] = cord;
        }
    }

    centroids= kmeans_calc(points, centroids_indices, K, n, d, iter, eps);
    if (centroids == NULL)       /* Something went wrong*/
    {
        freeMemoryModule(points, centroids_indices, centroids, n, K);
        return NULL;
    }

    /* Now convert centroids into python object and return it to the python file for printing the final result as required*/

    py_centroids = PyList_New(K);   
    if (py_centroids == NULL) {
        freeMemoryModule(points, centroids_indices, centroids, n, K);
        return NULL;
    }


    for(i=0; i<K; i++)
    {

        PyObject *centroid_vector = PyList_New(d);
        if (centroid_vector == NULL) {
            freeMemoryModule(points, centroids_indices, centroids, n, K);
            return NULL;
        }

        for(j=0; j<d; j++)
        {
            PyObject *value = PyFloat_FromDouble(centroids[i][j]);
            if (value == NULL) {
                freeMemoryModule(points, centroids_indices, centroids, n, K);
                return NULL;
            }
            PyList_SET_ITEM(centroid_vector, j, value);
        }

        PyList_SET_ITEM(py_centroids, i, centroid_vector);

    }
    /* finished: */
    freeMemoryModule(points, centroids_indices, centroids, n, K);
    return py_centroids;   

}


/*
    help functions
*/

static PyMethodDef kmeansMethods[] = {
    {"fit",                   /* the Python method name that will be used */
      (PyCFunction) fit, /* the C-function that implements the Python function and returns static PyObject*  */
      METH_VARARGS,           /* flags indicating parameters
      accepted for this function */
      PyDoc_STR("gets points, centroids_indices, K, n, d, iter, eps and returns the centroids based on kmeans algorithm")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}     /* The last entry must be all NULL as shown to act as a
                                 sentinel. Python looks for this entry to know that all
                                 of the functions for the module have been defined. */
};


static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    kmeansMethods /* the PyMethodDef array from before containing the methods of the extension */
};


PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&kmeansmodule);
    if (!m) {
        return NULL;
    }
    return m;
}


void freeMemoryModule(double** points, int* centroid_indices, double** centroids, int n, int K){
    int i; 
    if (points != NULL) {
        if (points[0] != NULL) {
            free(points[0]);  /* free the contiguous memory block */
        }
        free(points);
    }

    if (centroid_indices != NULL) {
        free(centroid_indices);
    }

    if (centroids != NULL){
        for (i = 0; i < K; i++){
            free(centroids[i]);
        }
        free(centroids);
    }
}
