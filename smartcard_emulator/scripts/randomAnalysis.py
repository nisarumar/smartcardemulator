import logging
import numpy as np
import matplotlib.pyplot as plt
logging.basicConfig(level=logging.DEBUG)
filename="putty.log2"
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
#npArrRaw=np.arange(1000)
k=128
npArrRef = np.array(randarrRef)
Rawacf = 1/sum(npArrRaw) * np.correlate(npArrRaw, npArrRaw, mode='full')
Rawacf = Rawacf[(len(npArrRaw)-1)-(k-1):(len(npArrRaw)-1)+k]
Refacf = 1/sum(npArrRef) * np.correlate(npArrRef, npArrRef, mode='full')
Refacf = Refacf[(len(npArrRef)-1)-(k-1):(len(npArrRef)-1)+k]
kappa = np.arange(-(k-1), k)
plt.figure(2)
plt.subplot(211)
plt.plot(kappa,Rawacf)
plt.xlabel('Lags')
plt.ylabel('AutoCorrelation of Raw Entropy')
plt.subplot(212)
plt.plot(kappa,Refacf)
plt.xlabel('Lags')
plt.ylabel('AutoCorrelation of PRNG')
plt.figure(1)
plt.subplot(211)
plt.hist(npArrRaw, bins=256)
plt.xlabel('Raw Entropy')
plt.ylabel('Frequency')
plt.subplot(212)
plt.hist(npArrRef, bins=256)
plt.xlabel('PRNG Bytes')
plt.ylabel('Frequency')
plt.show()




#Size was 365 characters

#Size was 1120 for Raw
#Size was 35682 for Refined
