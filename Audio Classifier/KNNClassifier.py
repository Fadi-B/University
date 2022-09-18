import numpy as np # import numpy for arrays
import scipy.spatial # scipy spatial for computing distances in parallel
from scipy import stats # stats for mode calculation

import dataProcessor as dp

X_train, y_train, X_val, y_val, X_test, y_test = dp.get_processed_data()

def knn_classifier(X_train, y_train, eval_data, k):
    """
    Performs K-NN classification on eval_data by computing distances of each of its points with
    a training set X_train. The function outputs an array of predictions for each datapoint in eval_data
    Args:
        X_train : training set represented by a matrix (NxD) of N data points of dimensionality D
        y_train : labels for the training set, a vector N where each element i corresponds to the label of X_train[i]
        eval_data : a dataset of test (or validation) points 
    Example use:
        >>> val_predictions = knn_classifier(X_train, y_train, X_val, k=3)
        >>> test_predictions = knn_classifier(X_train, y_train, X_test, k=5)
    """
    
    dist_mat = scipy.spatial.distance_matrix(eval_data,X_train) # Get the distance matrix between all points
    sorted_neighbours = np.argsort(dist_mat) # Sort by distance 
    knns = sorted_neighbours[:,0:k] # Take k nearest neighbours
    neighbour_labels = y_train[knns] # Get the labels for every neighbour for every point
    predictions, _  = stats.mode(neighbour_labels,axis=1) # First output of stats.mode is the actual mode
    return np.squeeze(predictions) # Squeeze gets rid of unnecessary extra dimensions

def compute_accuracy(predictions, true_labels):
    """
    Compares predicted labels in predictions to the actual labels in true_labels and computes an accuracy score.
    Args:
        predictions : an N-D array of predictions obtained from a classifier for some data points
        true_labels : an N-D array of ground-truth labels 
    Example use:
        >>> val_accuracy = compute_accuracy(val_predictions, y_val)
        >>> test_accuracy = compute_accuracy(test_predictions, y_test)
    
    """
    return 100*np.sum(predictions==true_labels)/len(predictions)

def tune_hyperparameter(num_iter):
    """
    Computes a suitable value for the hyperparameter k.
    It works by comparing the accuracy score for different values of k on the validation set. The values of k
    range from 1 to the number of data points N, but if there is no improvement observed in the new accuracy after a specified 
    number of iterations for different k values have been made then the loop terminates. The reason for this is because
    if there is no improvement observed after a certain number of neigbours have been examined then there is no reason
    to look further as it is much more computationally desirable to choose smaller values of k that still yield good accuracy.
    Args: 
        num_iter: Integer specifying the number of iterations that should be performed before terminating if an enhanced
                  hyperparameter is not found
    Example use:
        >>> k = tuned_hyperparameter(num_iter)
    """ 
    
    tunedParameter = 0 #This will hold the tuned hyperparameter that has been found
    tunedAccuracy = 0  #This will keep track of the new accuracy as the hyperparameter is updated
    count = 0          #This will keep track if we've exceeded the specified number of iterations without any improvement
    k = 1              #Loop variable for k

    while(k <= 1000 and count < num_iter):
        predictions = knn_classifier(X_train, y_train, X_val, k) # Gets classifier predictions for X_val for a specific k value
        acc = compute_accuracy(predictions, y_val) # Compares these predictions to the true labels to get an accuracy score
    
        if (tunedAccuracy < acc):  #If tunedAccuracy is smaller than new accuracy then update tuned parameter and accuracy
            tunedParameter = k
            tunedAccuracy = acc
            count = 0              #Reset count iterations as new tuned parameter found
    
        k = k + 1
        count = count + 1
    return tunedParameter

k = tune_hyperparameter(100) # Tune the hyperparameter - the argument has just been chosen to be 
                             # sufficiently reasonable considering that we have 1000 data points

test_predictions = knn_classifier(X_train, y_train, X_test, k) # Gets classifier predictions for X_test
acc = compute_accuracy(test_predictions, y_test) # Compares these predictions to the true labels of test set to get an accuracy score

print(f'Our K-NN classifier with K={k} is {acc:0.2f}% accurate.')
