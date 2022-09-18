import numpy as np # Import the numpy package
import matplotlib.pyplot as plt # For plotting
from scipy.io import wavfile as wav # Scipy has functions for reading in wav files

from scipy.fft import rfft # SciPy lets us do a fast Fourier Transform (FFT)

rate, data = wav.read('recordings/7_jackson_30.wav') # Read in wav file


data_freq = rfft(data, n=rate) # Take the real FFT of our data.
data_freq = np.abs(data_freq) # We're interested in the magnitude.

print(f'We now have our data in frequency. It has {len(data_freq)} elements.')


frequencies = 4000 #We are able to observe frequencies up to 4000Hz with the FFT

plt.plot(np.linspace(0,frequencies,len(data_freq)),data_freq,'k') # Plot the magnitudes against frequency values.
plt.xlabel('Frequency (Hz)') # Add x label
plt.ylabel('Magnitude') # Add y label 
plt.xticks(size = 10) # Make ticks bigger
plt.yticks(size = 10) # Make ticks bigger
plt.grid() # Add a grid
plt.title('Magnitude vs Frequency plot of audio sample') # Add a title

plt.show()
