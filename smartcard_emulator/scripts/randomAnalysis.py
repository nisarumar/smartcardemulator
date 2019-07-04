import logging
import numpy as np
import matplotlib.pyplot as plt
logging.basicConfig(level=logging.DEBUG)
filename="putty.log"
filehndl= open(filename,'r');
randarrRaw = []
randarrRef = []
for line in filehndl:
	str(line)
	if(line[0] == 'V'):
		print(line)
		randarrRaw.append(int(line[3:],16))
	if(line[0] == 'R'):
		print(line)
		randarrRef.append(int(line[3:],16))
print("Size is: %d"%len(randarrRaw))
print("Size is: %d"%len(randarrRef))
npArrRaw = np.array(randarrRaw)
npArrRef = np.array(randarrRef)
plt.figure(1)
plt.subplot(211)
plt.hist(npArrRaw, bins=256)
plt.xlabel('Raw Entropy')
plt.ylabel('Frequency')
plt.subplot(212)
plt.hist(npArrRef, bins=256)
plt.xlabel('PRNG Bytes')
plt.ylabel('Frequency')
#plt.show()




#Size was 365 characters

#Size was 1120 for Raw
#Size was 35682 for Refined
