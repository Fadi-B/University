import numpy as np # import numpy for arrays
import matplotlib.pyplot as plt # For plotting
import scipy.spatial # scipy spatial for computing distances in parallel
from scipy import stats # stats for mode calculation

import dataProcessor as dp

X_train, y_train, X_val, y_val, X_test, y_test = dp.get_processed_data()

np.random.seed(5) # Fix the seed so we have the same random numbers each time we run this.

class LogisticClassifier:
    """
    This class creates a logistic classifer object with randomly initialised weight and bias.
    Its apply function can be used to apply the classifier to a data point
    """
    def __init__(self, num_dimensions=2): #We are dealing with 2 dimensions
        self.w = np.random.normal(size=num_dimensions) # Create a weight vector 
        self.b = np.random.normal(size=1) # Create a bias
        
    def sigmoid(self, z):
        """
        Sigmoid function defined for use in this classifier.
        """
        return 1/(1+np.exp(-z))
        
    def apply(self, x):
        """
        This function takes in a data point x, and applies the classifier to it.
        """
        return self.sigmoid(x@self.w.T + self.b) # Computes the classifier output
    
def train_classifier_using_GD(classifier, X_train, y_train, alpha = 0.1, num_iters = 100):
    """
    This function takes in a logistic classifier object, training data, labels, and a learning
    rate alpha and performs iterations of gradient descent to update the classifier's
    parameters. It uses the gradients of the cross entropy loss. The function will change the
    parameter weights of the classifier object, and outputs the cross entropy loss for each iteration.
    Args:
        classifier : A logistic classifier object as defined in the above class
        X_train : training set represented by a matrix (NxD) of N data points of dimensionality D
        y_train : labels for the training set, a vector N where each element i corresponds to the label of X_train[i]
        alpha : The learning rate, default 0.1
        num_iters : The number of iterations to perform gradient descent, default 100
    Example use:
        >>> loss_array = train_classifier_using_GD(classifier, X_train, y_train)
    """
    
    num_data_points, num_dimensions = X_train.shape # Get num data points, and dims
    loss_array = [] # Empty array to append the MSE loss at each iteration.
    
    for _ in range(num_iters):
        
        z = classifier.apply(X_train) # Apply the classifier to the training points
         
        dL_dw =  np.zeros(num_dimensions) # Create a zero array to populate
        dL_db = 0 # Zero value for bias
        
        for i in range(num_data_points):
            dL_dw += X_train[i]*(z[i]-y_train[i]) # Sum for each data point
            dL_db += (z[i]-y_train[i]) # Sum for each data point

        dL_dw *= 1/num_data_points # Multiply by 1/N
        dL_db *= 1/num_data_points # Multiply by 1/N
        
        # Now, update the parameters
        classifier.w -= alpha * dL_dw 
        classifier.b -= alpha * dL_db
        
        # (Re)compute loss and append to loss array
        z = classifier.apply(X_train) 
        loss = np.mean(-y_train * np.log(z) -(1-y_train)*np.log(1-z)) # Cross entropy loss expression
        loss_array.append(loss)

    return loss_array

def compute_accuracy(outputs, true_labels):
    """
    Compares predicted labels in predictions to the actual labels in true_labels and computes an accuracy score.
    Args:
        predictions : an N-D array of predictions obtained from a classifier for some data points
        true_labels : an N-D array of ground-truth labels 
    Example use:
        >>> val_accuracy = compute_accuracy(val_predictions, y_val)
        >>> test_accuracy = compute_accuracy(test_predictions, y_test)
    """
    
    predictions = np.zeros_like(outputs)
    for i in range(len(outputs)):
        if outputs[i] > 0.5: # Classify as 1 if > 0.5 and as 0 if < 0.5
            predictions[i] = 1
        else:
            predictions[i] = 0

    return 100*np.sum(predictions==true_labels)/len(predictions)


classifier = LogisticClassifier() # Make classifier
loss_array = train_classifier_using_GD(classifier, X_train, y_train) # Perform gradient descent to learn our classifier!

outputs = classifier.apply(X_test) # Apply the learnt classifier on the test set
acc = compute_accuracy(outputs, y_test) # Compute accuracy of test set
print(f'Our trained logistic classifier is {acc:0.2f}% accurate.')

#The test accuracy for the logistic classifier is 94%, which is not as good as the KNN classifier. However, it should be noted 
#that the accuracy of the logistic classifier will depend on how well it is trained through for example gradient descent.
#We are only performing 100 iterations for gradient descent, but increasing the iterations is likely to minimize
#our cross entropy loss further assuming that the minimum of the loss function has not been found yet, 
#which should increase the accuracy of our classifier. For example, let us attempt to
#learn our classifer using 5000 iterations instead of 100 and observe the difference,

new_classifier = LogisticClassifier() # Make new classifier
new_loss_array = train_classifier_using_GD(new_classifier, X_train, y_train, 0.1, 5000) # Perform gradient descent to learn our classifier using 5000 iterations!

new_outputs = new_classifier.apply(X_test) # Apply the new learnt classifier on the test set
new_acc = compute_accuracy(new_outputs, y_test) # Compute new accuracy of test set
print(f'Our new trained logistic classifier is {new_acc:0.2f}% accurate.')

#The new classifier has now an accuracy of 96.50%, which is as good as the KNN classifier!


### Constructing the decision boundary

x1 = np.linspace(-2,4,20) # Generate a set of x1 points

w1, w2, b = new_classifier.w[0], new_classifier.w[1], new_classifier.b # Get classifier params

x2 = -(w1/w2) * x1 - (b/w2) # Compute x2 for our generated set of x1 values, which gives us the decision boundary

### Viewing the classifier's decision boundary

george = np.where(y_test==0)[0] # Entries in the label vector are 0 for george
jackson = np.where(y_test==1)[0] # Entries in the label vector are 1 for jackson
 
plt.scatter(X_test[george,0], X_test[george,1], color='b') # Plot the data for george in blue
plt.scatter(X_test[jackson,0], X_test[jackson,1], color='r') # Plot the data for jackson in red

plt.plot(x1, x2, color='k', alpha=1) # Draw our decision boundary line

plt.xlabel('Dimension 1') # X-axis will represent the first dimension of our dataset
plt.ylabel('Dimension 2') # Y-axis will represent the second dimenstion of our dataset

plt.legend(['Decision Boundary', 'George', 'Jackson'])  # Put class names and decision boundary in the legend
plt.title('Logisitic classification of the audio files') # Add a title

plt.show()
