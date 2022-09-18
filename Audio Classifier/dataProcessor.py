import numpy as np # Import the numpy package
import matplotlib.pyplot as plt # For plotting
from scipy.io import wavfile as wav # Scipy has functions for reading in wav files
from scipy.fft import rfft # SciPy lets us do a fast Fourier Transform (FFT)
import os

def fft_from_audio(filename):
    """This function will take in a wav file and output its FFT."""
    rate, data = wav.read(filename)# Read file
    data_freq = rfft(data, n=rate) # Take the real FFT of our data.
    data_freq = np.abs(data_freq) # We're interested in the magnitude
    return data_freq

def create_dataset_from_audio():

    # Create arrays to store data points and labels
    X = np.zeros((1000,4001)) # Zero array to store vectors
    y = np.zeros((1000)) # Zero array to store labels

    # Don't worry too much about this next series of loops. 
    # It is just a crude way of grabbing all the files by speaker.
    running_index=0
    for speaker in ['george', 'jackson']:
        for num in range(10):
            for file in range(50):
                X[running_index] = fft_from_audio(f'recordings/{num}_{speaker}_{file}.wav') # Read in wav file
            
                if speaker == 'george':
                    y[running_index] = 0
                elif speaker == 'jackson':
                    y[running_index] = 1
                running_index+=1

    return  X, y


def examine_samples(X, y):

    """ Examining how George and Jackson samples differ """
    george_samples = np.where(y==0)[0] # Find entries in y that are equal to 0 (and are therefore George)
    jackson_samples = np.where(y==1)[0] # Find entries in y that are equal to 1 (and are therefore Jackson)

    # Shuffle these for variety
    np.random.seed(42)
    np.random.shuffle(george_samples)
    np.random.shuffle(jackson_samples)

    # Subplot of some George and Jackson frequency spectra
    fig, axs = plt.subplots(4, 2)
    axs[0, 0].set_title('George')
    axs[0, 1].set_title('Jackson')

    for i in range(4):
        axs[i,0].plot(X[george_samples[i]], color='b')
        axs[i,1].plot(X[jackson_samples[i]],color='r')

    for ax in axs.flat:
        ax.set(xlabel='Frequency (Hz)')
        ax.grid()

    for ax in axs.flat:
        ax.label_outer()    
    

def standardise_dataset(X):

    """ Analysis showed that the unnormalized magnitudes will be problematic
    they might dominate so we need to standardise the set """
    column_means = X.mean(0) # Takes the mean across columns for the dataset
    column_stds = X.std(0) # Takes the standard deviation across columns for the dataset

    eps = 1e-8 # A small constant to prevent division by zero

    Xs = (X - column_means)/(column_stds + eps) # Standardise the dataset
    Xs = np.round(Xs, 2) # Round to 2 d.p.

    return Xs


def perform_PCA(Xs, y):

    """ Perform PCA to be able to visualize and analyze the dataset """

    D = 4001
    C = (1/D)*Xs.T@Xs

    if (not os.path.isfile('C_eigenvalues.npy') or
        not os.path.isfile('C_eigenvectors.npy')):
        
        eigenvalues, eigenvectors = np.linalg.eig(C)

        # Take the real parts
        eigenvalues = np.real(eigenvalues)
        eigenvectors = np.real(eigenvectors)

        # Keep the first 10 eigenvalues/vectors
        eigenvalues = eigenvalues[0:10]
        eigenvectors = eigenvectors[:,0:10]

        np.save('C_eigenvalues.npy', eigenvalues)
        np.save('C_eigenvectors.npy', eigenvectors)

    eigenvalues = np.load('C_eigenvalues.npy')
    eigenvectors = np.load('C_eigenvectors.npy')


    """ Reduce the dataset to 2 dimensions and visualize the data """

    W = eigenvectors[:,0:2] # Take the first two columns of the eigenvectors, which are the first two principal componenents of our dataset
    Xd = Xs@W # Use W to reduce X down to a new 2 dimensionsonal dataset Xd

    #george = np.where(y==0)[0] # Entries in the label vector are 0 for george
    #jackson = np.where(y==1)[0] # Entries in the label vector are 1 for jackson

    #plt.scatter(Xd[george,0],Xd[george,1] ,color='b') # Plot the data for george in blue
    #plt.scatter(Xd[jackson,0],Xd[jackson,1], color='r') # Plot the data for jackson in red

    #plt.xlabel('PCA dimension 1') # X-axis will represent the first PCA dimension
    #plt.ylabel('PCA dimension 2') # Y-axis will represent the second PCA dimension
    
    #plt.legend(['George', 'Jackson'])  # Put class names in the legend
    #plt.title('Plot of PCA transformed dataset') # Add a title

    return Xd


def create_train_val_test_sets(Xd, y):
    
    """ Split the dataset into training, validation and test set to
        be able to learn classifiers on it """

    # Re-normalise data (small numbers are nicer)
    Xd = (Xd-Xd.mean(0))/(Xd.std(0)+1e-8)

    # Split into train/val/test at ratio 400/200/400

    np.random.seed(42) # Set a random seed
    indices = np.array([i for i in range(1000)]) # The numbers 0-999
    np.random.shuffle(indices) # Shuffle data before splitting.

    X_train = Xd[indices[0:400]]
    X_val = Xd[indices[400:600]]
    X_test = Xd[indices[600:]]

    y_train = y[indices[0:400]]
    y_val = y[indices[400:600]]
    y_test = y[indices[600:]]

    return X_train, y_train, X_val, y_val, X_test, y_test

def get_processed_data():

    X, y = create_dataset_from_audio()
    Xs = standardise_dataset(X)
    Xd = perform_PCA(Xs, y)

    X_train, y_train, X_val, y_val, X_test, y_test = create_train_val_test_sets(Xd, y)
    
    return X_train, y_train, X_val, y_val, X_test, y_test
    


