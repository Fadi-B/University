import numpy as np # Import the numpy package
import matplotlib.pyplot as plt # For plotting
from scipy.io import wavfile as wav # Scipy has functions for reading in wav files

rate, data = wav.read('recordings/7_jackson_30.wav') # Read in wav file

print(f'The sample rate is {rate} Hz')
print(f'data has {len(data)} elements.')

time = (1/rate) * len(data) # Length of audio clip is the duration 
                            # for all elements to have been sampled


plt.plot(np.linspace(0,time,len(data)),data,'k') # Plot data against time values.
plt.xlabel('Time (s)') # Add x label
plt.ylabel('Magnitude') # Add y label 
plt.xticks(size = 10) # Make ticks bigger
plt.yticks(size = 10) # Make ticks bigger
plt.grid() # Add a grid
plt.title('Speech Sample') # Add a title
plt.show()
